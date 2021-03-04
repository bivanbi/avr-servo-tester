#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include "switch.h"


uint8_t switch_state			= 0;
uint8_t switch_state_changed		= 0;
uint8_t switch_button_debounce_timer	= 0;
uint8_t switch_button_press_timer	= 0;
uint8_t	switch_flag			= 0;

uint8_t	switch_repeat_delay_initial	= SWITCH_REPEATER_DELAY_GENERAL;
uint8_t	switch_repeat_delay_repeat	= SWITCH_REPEATER_REPEAT_GENERAL;


/*
 *
 * SWITCH DEBOUNCER/REPEAtER: timer0 A
 * normal mode: WGM02:0 = 0
 *	TCCR0A
		7	COM0A1	0 0: normal, OC0A disconnected
		6	COM0A0
		5	COM0B1
		4	COM0B0	
		-
		-
		1	WGM01
		0	WGM00

 *	TCCR0B
		7	FOC0A
		6	FOC0B
		-
		-
		3	WGM02
		2	CS02	
		1	CS01	
		0	CS00	


	WGM02 1 0
	0   0 0 0	TOP: FF, TOV flag set: MAX

	CS20
	    0 0 0	timer stopped
	    0 0 1	no prescale
	    0 1 0       /8
            0 1 1       /64
            1 0 0       /256
            1 0 1       /1024

	TIFR0 Interrupt Flag Register
		2	OCF0B match flag
		1	OCF0A match flag
		0	TOV0 timer overflow flag *cleared by writing 1 or executing interrupt

 */

void switch_init(void)
{

	SWITCH_UP_DIR		&= ~(1<<SWITCH_UP_BIT);
#if SWITCH_UP_PULLUP > 0
	SWITCH_UP_PORT		|= (1<<SWITCH_UP_BIT);
#endif

	//SWITCH_CENTER
	SWITCH_CENTER_DIR	&= ~(1<<SWITCH_CENTER_BIT);
#if SWITCH_CENTER_PULLUP > 0
	SWITCH_CENTER_PORT	|= (1<<SWITCH_CENTER_BIT);
#endif

	//SWITCH_DOWN
	SWITCH_DOWN_DIR		&= ~(1<<SWITCH_DOWN_BIT);
#if SWITCH_DOWN_PULLUP > 0
	SWITCH_DOWN_PORT	|= (1<<SWITCH_DOWN_BIT);
#endif

	//SWITCH_LEFT
	SWITCH_LEFT_DIR		&= ~(1<<SWITCH_LEFT_BIT);
#if SWITCH_LEFT_PULLUP > 0
	SWITCH_LEFT_PORT	|= (1<<SWITCH_LEFT_BIT);
#endif

	//SWITCH_RIGHT
	SWITCH_RIGHT_DIR	&= ~(1<<SWITCH_RIGHT_BIT);
#if SWITCH_RIGHT_PULLUP > 0
	SWITCH_RIGHT_PORT	|= (1<<SWITCH_RIGHT_BIT);
#endif

	//SWITCH_SOURCE
	SWITCH_SOURCE_DIR	&= ~(1<<SWITCH_SOURCE_BIT);
#if SWITCH_SOURCE_PULLUP > 0
	SWITCH_SOURCE_PORT	|= (1<<SWITCH_SOURCE_BIT);
#endif

}


uint8_t switch_pressed_button_count(void)
{
    uint8_t count = 0;
    if (switch_state & (1<<SWITCH_UP_ID)){
	count++;
    }
    if (switch_state & (1<<SWITCH_DOWN_ID)){
	count++;
    }
    if (switch_state & (1<<SWITCH_LEFT_ID)){
	count++;
    }
    if (switch_state & (1<<SWITCH_RIGHT_ID)){
	count++;
    }
    if (switch_state & (1<<SWITCH_CENTER_ID)){
	count++;
    }
    return count;
}

inline void switch_clear_button_changed(void)
{
    switch_state_changed &= ~ (SWITCH_UP_ID|SWITCH_DOWN_ID|SWITCH_LEFT_ID|SWITCH_RIGHT_ID|SWITCH_CENTER_ID); 
}


void switch_refresh_all_state(void)
{
    // DEBOUNCE please
   
    // switch_button_debouncer_timer
    if(switch_flag & (1<<SWITCH_FLAG_DO_DEBOUNCE))
    {
        // check if OCF0A registered an overflow which means debounce delay has been passed
	// FIXME to play safe, we will consider TOV0 too
	//if (TIFR0 & ((1<<OCF0A)|(1<<TOV0)))
	if (TIFR0 & (1<<OCF0A))
        {
            switch_button_debounce_timer++;
            // reset flag
            TIFR0 |= (1<<OCF0A);
        }

	if (switch_button_debounce_timer >= SWITCH_DEBOUNCER_DELAY)
        {
            switch_timer_stop();
            switch_flag &= ~(1<<SWITCH_FLAG_DO_DEBOUNCE);
        } else {
            return;
        }
    }
   

    switch_query(SWITCH_UP_ID,&SWITCH_UP_PIN,SWITCH_UP_BIT);
    switch_query(SWITCH_DOWN_ID,&SWITCH_DOWN_PIN,SWITCH_DOWN_BIT);
    switch_query(SWITCH_LEFT_ID,&SWITCH_LEFT_PIN,SWITCH_LEFT_BIT);
    switch_query(SWITCH_RIGHT_ID,&SWITCH_RIGHT_PIN,SWITCH_RIGHT_BIT);
    switch_query(SWITCH_CENTER_ID,&SWITCH_CENTER_PIN,SWITCH_CENTER_BIT);
    switch_query(SWITCH_SOURCE_ID,&SWITCH_SOURCE_PIN,SWITCH_SOURCE_BIT);

    if (switch_state_changed)
    {
        switch_timer_start();
	// set DO_DEBOUNCE flag
        switch_flag |= (1<<SWITCH_FLAG_DO_DEBOUNCE);
	// clear flags and counters
        switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
        switch_flag &= ~(1<<SWITCH_FLAG_DELAY_WAIT);
        switch_flag &= ~(1<<SWITCH_FLAG_REPEAT_WAIT);
	switch_button_press_timer = 0;
	switch_button_debounce_timer = 0;
    }

    // 
    // if there is a button pressed and state has not changed, begin pressed time count
    // SOURCE select switch should be excluded here since it is not a button per se
    //
    else if (switch_state & ~(1<<SWITCH_SOURCE_ID))
    {
	//
	// delay wait: when the button is pressed, there is an additional delay before first repeat
	// occurs: firt repeat after delay_wait + repeat wait
	// DELAY_WAIT is set when this initial delay has passed
	//
	if ((switch_flag & (1<<SWITCH_FLAG_DELAY_WAIT)))
        {
	    //
	    // see if timer2 has reached SWITCH_REPEATER_DELAY_RESOLUTION
            //
            if (TIFR0 & (1<<OCF0A))
            {
		//
		// since 8bit timer resolution is not enough to wait more than 25 msec at 1Mhz,
		// we need to "pre-scale" it. total delay = SWITCH_REPEATER_DELAY_GENERAL * switch_button_press_timer
		//
                switch_button_press_timer++;
                // reset flag
	        TIFR0 |= (1<<OCF0A);
            }
            if ((switch_flag & (1<<SWITCH_FLAG_REPEAT_WAIT)) && (switch_button_press_timer >= switch_repeat_delay_repeat))
            {
		// normal repeat after first repeat
                switch_flag |= (1<<SWITCH_FLAG_DO_REPEAT);
                switch_button_press_timer = 0;

            } else if (switch_button_press_timer >= switch_repeat_delay_initial){
            	// initial delay passed, first repeat occurs
                switch_flag |= (1<<SWITCH_FLAG_DO_REPEAT)|(1<<SWITCH_FLAG_REPEAT_WAIT);
                switch_button_press_timer = 0;
            }
        }
        else
        {
	    // initial delay before repeat delay
	    switch_timer_start();
            switch_flag |= (1<<SWITCH_FLAG_DELAY_WAIT);
        }
    }
}


void switch_query(uint8_t switch_id, volatile uint8_t *switch_pin, uint8_t switch_bit)
{
    /*
     * compare actual switch state to switch_state by XORing the corresponding bits.
     * if both bits equal, result will be 0, otherwise 1.
     * since we are using low-active switches, we need to negate each bits read from PINx
     *
     *  0 0 > 0
     *  0 1 > 1
     *  1 0 > 1
     *  1 1 > 0
     */
    
    // ((switch_state>>switch_id)&1)
    //if ((switch_state & (1<<switch_id)) ^ ((~(*switch_pin)) & (1<<switch_bit)))
    // we need to put both bits to rightmost to be able to xor them
    if (((switch_state>>switch_id)&1) ^ (((~(*switch_pin))>>switch_bit)&1))
    {
        // we have a changed bit! set the variables
        switch_state_changed |= (1<<switch_id);
    }



    // zero out corresponding bit
    switch_state &= ~(1<<switch_id);
    // we need to invert low-active switches, so switch_state will be set if PINx is low
    // pinb.x = 0 -> switch state.x = 1
    // pinb.x = 1 -> switch state.x = 0
    if ((~(*switch_pin)) & (1<<switch_bit)){
	    switch_state |= (1<<switch_id);
    }
}


/*
	
	when a button is pressed, initiate switch debounce
	user input will be ignored till debounce delay passes
 */
void switch_timer_start(void)
{
    // set TCNT0 to 0
    TCNT0 = 0;
    // set OCR0A to SWITCH_REPEATER_DELAY_RESOLUTION
    OCR0A = SWITCH_TIMER_RESOLUTION;
    // clear TIFR0 OCF0A bit
    TIFR0 |= (1<<OCF0A)|(1<<TOV0);
    // set up timer0 normal mode and start timer
    // WGM01: clear timer on match, TOP: OCR0A
    TCCR0A = (1<<WGM01);
    TCCR0B = SWITCH_TIMER_PRESCALER;
}

inline void switch_timer_stop(void)
{
    // stop timer0 - actually this is only a power-saving matter
    TCCR0A = 0;
    TCCR0B = 0;
}


/*
void switch_repeat_delay_start(void)
{
    switch_button_press_timer = 0;
    // set TCNT0 to 0
    TCNT2 = 0;
    // set OCR2A to SWITCH_REPEATER_DELAY_RESOLUTION
    OCR2A = SWITCH_REPEATER_DELAY_RESOLUTION;
    // clear TIFR0 OCF0A bit
    TIFR2 |= (1<<OCF2A)|(1<<TOV0);
    // set up timer0 CTC mode and start timer
    // WGM21: clear timer on match, TOP: OCR2A
    TCCR2A = (1<<WGM21);
    TCCR2B = SWITCH_REPEATER_PRESCALER;
}

inline void switch_repeat_delay_stop(void)
{
    // stop timer2 - actually this is only a power-saving matter
    TCCR2A = 0;
    TCCR2B = 0;
}
*/


