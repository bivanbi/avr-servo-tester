/*
 *
 *
 *	switch configuration
 *
 *
 *	THERE IS NO DEFAULT pin configuration.
 *
 *
*/


#ifndef SWITCH_HEADER_LOADED
#define SWITCH_HEADER_LOADED

/* 
 *
 *	to conserve memory, we multiplex eigth switch states to a single byte.
 * 
 *	if switch state changes (comparing PINx to switch_state), switch_state_changed
 *      will be set
 *
 */

extern uint8_t switch_state;
extern uint8_t switch_state_changed;


// for waiting repeat delay and adjusting repeat rate
extern uint8_t	switch_button_press_timer;


#define SWITCH_FLAG_DO_DEBOUNCE 0
#define SWITCH_FLAG_DELAY_WAIT	1
#define SWITCH_FLAG_REPEAT_WAIT	2
// SWITCH_FLAG_DO_REPEAT is set by switch.c and should be cleared by application when
// it processed this information.
#define SWITCH_FLAG_DO_REPEAT	3
extern uint8_t	switch_flag;

extern uint8_t switch_repeat_delay_initial;
extern uint8_t switch_repeat_delay_repeat;


void switch_init(void);
void switch_refresh_all_state(void);
void switch_query(uint8_t switch_id, volatile uint8_t *switch_pin, uint8_t switch_bit);
uint8_t switch_pressed_button_count(void);
inline void switch_clear_button_changed(void);

void switch_timer_start(void);
inline void switch_timer_stop(void);

// void switch_repeat_delay_start(void);
// inline void switch_repeat_delay_stop(void);

// check if all the variables defined

#ifndef SWITCH_UP_ID
#error "SWITCH_UP_ID not defined"
#endif
#ifndef SWITCH_UP_PORT
#error "SWITCH_UP_PORT not defined"
#endif
#ifndef SWITCH_UP_DIR
#error "SWITCH_UP_DIR not defined"
#endif
#ifndef SWITCH_UP_BIT
#error "SWITCH_UP_BIT not defined"
#endif
#ifndef SWITCH_UP_PIN
#error "SWITCH_UP_PIN not defined"
#endif
#ifndef SWITCH_UP_PULLUP
#error "SWITCH_UP_PULLUP not defined"
#endif

#ifndef SWITCH_DOWN_ID
#error "SWITCH_DOWN_ID not defined"
#endif
#ifndef SWITCH_DOWN_PORT
#error "SWITCH_DOWN_PORT not defined"
#endif
#ifndef SWITCH_DOWN_DIR
#error "SWITCH_DOWN_DIR not defined"
#endif
#ifndef SWITCH_DOWN_BIT
#error "SWITCH_DOWN_BIT not defined"
#endif
#ifndef SWITCH_DOWN_PIN
#error "SWITCH_DOWN_PIN not defined"
#endif
#ifndef SWITCH_DOWN_PULLUP
#error "SWITCH_DOWN_PULLUP not defined"
#endif

#ifndef SWITCH_LEFT_ID
#error "SWITCH_LEFT_ID not defined"
#endif
#ifndef SWITCH_LEFT_PORT
#error "SWITCH_LEFT_PORT not defined"
#endif
#ifndef SWITCH_LEFT_DIR
#error "SWITCH_LEFT_DIR not defined"
#endif
#ifndef SWITCH_LEFT_BIT
#error "SWITCH_LEFT_BIT not defined"
#endif
#ifndef SWITCH_LEFT_PIN
#error "SWITCH_LEFT_PIN not defined"
#endif
#ifndef SWITCH_LEFT_PULLUP
#error "SWITCH_LEFT_PULLUP not defined"
#endif

#ifndef SWITCH_RIGHT_ID
#error "SWITCH_RIGHT_ID not defined"
#endif
#ifndef SWITCH_RIGHT_PORT
#error "SWITCH_RIGHT_PORT not defined"
#endif
#ifndef SWITCH_RIGHT_DIR
#error "SWITCH_RIGHT_DIR not defined"
#endif
#ifndef SWITCH_RIGHT_BIT
#error "SWITCH_RIGHT_BIT not defined"
#endif
#ifndef SWITCH_RIGHT_PIN
#error "SWITCH_RIGHT_PIN not defined"
#endif
#ifndef SWITCH_RIGHT_PULLUP
#error "SWITCH_RIGHT_PULLUP not defined"
#endif

#ifndef SWITCH_CENTER_ID
#error "SWITCH_CENTER_ID not defined"
#endif
#ifndef SWITCH_CENTER_PORT
#error "SWITCH_CENTER_PORT not defined"
#endif
#ifndef SWITCH_CENTER_DIR
#error "SWITCH_CENTER_DIR not defined"
#endif
#ifndef SWITCH_CENTER_BIT
#error "SWITCH_CENTER_BIT not defined"
#endif
#ifndef SWITCH_CENTER_PIN
#error "SWITCH_CENTER_PIN not defined"
#endif
#ifndef SWITCH_CENTER_PULLUP
#error "SWITCH_CENTER_PULLUP not defined"
#endif

#ifndef SWITCH_SOURCE_ID
#error "SWITCH_SOURCE_ID not defined"
#endif
#ifndef SWITCH_SOURCE_PORT
#error "SWITCH_SOURCE_PORT not defined"
#endif
#ifndef SWITCH_SOURCE_DIR
#error "SWITCH_SOURCE_DIR not defined"
#endif
#ifndef SWITCH_SOURCE_BIT
#error "SWITCH_SOURCE_BIT not defined"
#endif
#ifndef SWITCH_SOURCE_PIN
#error "SWITCH_SOURCE_PIN not defined"
#endif
#ifndef SWITCH_SOURCE_PULLUP
#error "SWITCH_SOURCE_PULLUP not defined"
#endif




#ifndef SWITCH_TIMER_PRESCALER
#error "SWITCH_TIMER_PRESCALER not defined"
#endif
#ifndef SWITCH_DEBOUNCER_DELAY
#error "SWITCH_DEBOUNCER_DELAY not defined"
#endif
#ifndef SWITCH_REPEATER_DELAY_GENERAL
#error "SWITCH_REPEATER_DELAY_GENERAL not defined"
#endif
#ifndef SWITCH_REPEATER_REPEAT_GENERAL
#error "SWITCH_REPEATER_REPEAT_GENERAL not defined"
#endif
#ifndef SWITCH_REPEATER_REPEAT_NAME
#error "SWITCH_REPEATER_REPEAT_NAME not defined"
#endif


#ifndef SWITCH_TIMER_RESOLUTION
#error "SWITCH_TIMER_RESOLUTION not defined"
#endif


#endif

