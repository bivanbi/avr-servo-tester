#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
//#include <avr/pgmspace.h>

#include "lcd.h"
#include "adc.h"
#include "switch.h"
#include "main.h"
#include "eeprom_data.h"
#include "pwm_generator.h"

//sprintf(adcResultString,"P:"BYTETOBINARYPATTERN, BYTETOBINARY(PINB));

int main(void){
    // disable watchdog timer, in case we got here via software reset
    MCUSR = 0;
    wdt_disable();

    // set system clock prescaler
    // enable prescaler change
    CLKPR = (1<<CLKPCE); 
    // set prescaler
    CLKPR = CLOCK_PRESCALER; 


    // am i in menu and if so in wich menu i am
    // 0      = PWM display (no menu selected)
    // 1 .. x = menus
    //
    // 0 = startup (neither pwm source has been initiated)
    // 1 = ADC
    // 2 = RX

    // set pullup to all ports that arent used
    DDRB   = 0b00000000;
    PORTB |= 0b11111011;


    // set error led port to output
    ERROR_LED_DIR  |=  (1<<ERROR_LED_BIT);
    ERROR_LED_PORT &=  ~(1<<ERROR_LED_BIT);


    // init ports, ADC, LCD etc.
    lcd_init();
    switch_init();
    adc_init();
    // adc will be enabled by default for now
    adc_enable();
    adc_start_conversion();

    //lcd_write(1,0,LCD_COMMAND_CLEAR_SCREEN);

    // welcome with some delay
    lcd_print_addrstr(&mystring[STRING_WELCOME]);
    lcd_print_addrstr(&mystring[STRING_FWVER]);
    superdelay();

    // load servo data
    load_settings();
    //refVoltageCorrected = VOLTMETER_BASE_VOLTAGE + testerConfig.voltageCorrection * VOLTMETER_CORRECTION_MAX * 2 / VOLTMETER_CORRECTION_RESOLUTION;
    refVoltageCorrected = (VOLTMETER_BASE_VOLTAGE*VOLTMETER_CORRECTION_RESOLUTION) + (testerConfig.voltageCorrection * VOLTMETER_CORRECTION_MAX * 2);

    load_servo_data(1);
    clearscreen();
    lcd_printstring("Loaded servo 01:");
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),servoAttrib.name);
    superdelay();
    // uint8_t loop = 0;

    // check for pressed buttons
    startup_check_for_stuck_buttons();
    // clear

    // init screen on first loop
    menu_change_general_todo();





    /*
     *
     *
     *
     *
     *		DO THE WORK
     *
     *
     *
     *
     *
     */
    while(1)
    {
	// do the duties

	switch_refresh_all_state();

	//switch_flag
        //snprintf(PrintString,sizeof(PrintString),"OCR0A:%3d", OCR0A);
        //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|20),PrintString);
        //snprintf(PrintString,sizeof(PrintString),"SF:"BYTETOBINARYPATTERN, BYTETOBINARY(switch_flag));
        //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);
        //_delay_ms(25);

	//debug(2);

	/*
         *	should we have more than one button pressed, do not do anything.
         *
         */
       
     
	if(switch_pressed_button_count() > 1)
        {
	    //    make display blink to notify user of multiple buttons pressed
	    //    lcd_blinkscreen(1);
            global_flag |= (1<<MULTIPLE_BUTTONS_PRESSED);
	    switch_clear_button_changed();
	    //snprintf(PrintString,sizeof(PrintString),"%1d",switch_pressed_button_count());
            //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|15),PrintString);
	    //debug(4);
	    continue;
        } else if (global_flag & (1<<MULTIPLE_BUTTONS_PRESSED)){
	    //    lcd_blinkscreen(0);
            global_flag &= ~(1<<MULTIPLE_BUTTONS_PRESSED);
	    // clear switch_state_change flags, else it will cause trouble
	    switch_clear_button_changed();
            //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|15),"#");
	    //debug(44);
        }
    
	// need adc result for voltmeter, servo drive and excercise, servo name etc.
        adc_get_result();
        

	//debug(3);

	/*
         * 
	 * determine if menu button has been pressed - if so, behave accordingly
	 * do the math depending on where we are
	 *
	 */
	// uint8_t switch_state;
	// uint8_t switch_state_changed;
	handle_menu_button();
	//debug(5);


	// call the appropriate handler routine
	(*menu_handlers[current_function])();
	//debug(6);

	/*
	 *
	 * Drive the servos
	 *
	 */

	uint8_t source_id = 0;
	// this feature is not yet available
        //source_id = ((switch_state>>SWITCH_SOURCE_ID)&1);

	if (switch_state_changed & (1<<SWITCH_SOURCE_ID))
        {
            global_flag |= (1<<FLAG_PWM_ATTRIB_INVALID);
	    (*pwm_source_exit_handlers[source_id])();
        }

	(*pwm_source_handlers[source_id])();
	
	// notify switch_refresh that we received switch state change by clearing state_changed
	switch_state_changed = 0;
        switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);

	//debug(99);

	//superdelay();
    }
}



/*
 *
 *
 *
 *	MENU BUTTONS
 *
 *
 *
 */
void handle_menu_button(void)
{

    // menu up
    if ((switch_state & (1<<SWITCH_UP_ID)) & (switch_state_changed & (1<<SWITCH_UP_ID)))
    {
	(*menu_exit_handlers[current_function])();
        if (current_function >= MENU_MAX){
	    current_function = 0;
    	} else {
	    current_function++;
        }
	menu_change_general_todo();
    }

    if ((switch_state & (1<<SWITCH_DOWN_ID)) & (switch_state_changed & (1<<SWITCH_DOWN_ID)))
    {
	(*menu_exit_handlers[current_function])();
        if (current_function == 0){
	    current_function = MENU_MAX;
    	} else {
	    current_function--;
        }
	menu_change_general_todo();
    }
    //switch_state_changed &= ~(1<<SWITCH_UP_ID);
    //switch_state_changed &= ~(1<<SWITCH_DOWN_ID);

}

void menu_change_general_todo(void)
{
    clearscreen();
    menu_local_flag		= 0;
    switch_repeat_delay_initial	= SWITCH_REPEATER_DELAY_GENERAL;
    switch_repeat_delay_repeat	= SWITCH_REPEATER_REPEAT_GENERAL;
    (*menu_init_handlers[current_function])();
}

/*
 *
 *
 *
 *	PWM SOURCE: RX
 *
 *
 *
 */
void do_rx_passthrough(void)
{
    if (!(global_flag & (1<<FLAG_RX_PASSTHROUGH_INITIATED)))
    {
        // init passthrough here
        global_flag |= (1<<FLAG_RX_PASSTHROUGH_INITIATED);
    }
}
void exit_rx_passthrough(void)
{
    // exit rx passthrough
    global_flag &= ~(1<<FLAG_RX_PASSTHROUGH_INITIATED);
}
  


/*
 *
 *
 *
 *	PWM SOURCE: ADC
 *
 *
 *
 */
void do_generate_pwm(void)
{

    /*
     *
     *	we need to set pwm generator values before enabling pwm
     *  but only if we have valid servo attributes and adc to pwm enabled
     *
     */
    if (global_flag & (1<<FLAG_PWM_ATTRIB_INVALID))
    {
        validate_servo_data();
        if (global_flag & (1<<FLAG_PWM_ATTRIB_INVALID))
        {
	    ERROR_LED_PORT |= (1<<ERROR_LED_BIT);
            return;
        }
    }
    ERROR_LED_PORT &= ~(1<<ERROR_LED_BIT);

    /*
     *
     *  PWMAttrib.adcResult will be used to calculate actual PWM duty cycle
     *
     *  switches always have precedence over adc
     *
     */
    if (global_flag & (1<<FLAG_ADC_TO_PWM_ENABLED)){
	PWMAttrib.adcResult = adcResult;
    }

    // when in live data, treat left/ok/right switches as servo commands
    if (current_function == 0)
    {
        if (switch_state & (1<<SWITCH_RIGHT_ID))
        {
            PWMAttrib.adcResult = 1023;
        }
        if (switch_state & (1<<SWITCH_LEFT_ID))
        {
	    PWMAttrib.adcResult = 0;
        }
        if (switch_state & (1<<SWITCH_CENTER_ID))
        {
	    PWMAttrib.adcResult = 512;
        }
    }
    //
    else if (current_function == 1){
	if (global_flag2 & (1<<FLAG2_SERVO_EXCERCISE_ENABLED)){
	    PWMAttrib.adcResult = servoExcercisePosition;
	} else {
	    PWMAttrib.adcResult = adcResult;
	    servoExcercisePosition = adcResult;
        }
    }

    uint16_t ocr_duty_cycle	= 0xffff;
    uint16_t ocr_period		= 0xffff;
    /*
     *
     *
     * do the math
     *
     * 
     */

    // FIXME
    ocr_duty_cycle	= ((PWMAttrib.pwm_min + (((uint32_t)PWMAttrib.adcResult * PWMAttrib.pwm_range) / 1023)) * CLOCK_MULTIPLIER) - 1 ;
    ocr_period		= (PWMAttrib.pwm_period * CLOCK_MULTIPLIER) - 1;
    PWMAttrib.pwm	= (ocr_duty_cycle + 1) / CLOCK_MULTIPLIER;




    OCR1A		= ocr_period;
    OCR1B		= ocr_duty_cycle;

    if (!(global_flag & (1<<FLAG_PWM_GENERATOR_INITIATED)))
    {
	// turn off rx
        // adc_off();
        // rx_init();
        //pwm_init();
	pwm_generator_enable();
        global_flag |= (1<<FLAG_PWM_GENERATOR_INITIATED);
    }
}
void exit_generate_pwm(void)
{
    pwm_generator_disable();
    global_flag &= ~(1<<FLAG_PWM_GENERATOR_INITIATED);
}
  




/*
 *
 *
 *
 *	display live data
 *
 *
 *
 */
void init_live_data(void)
{
    lcd_print_addrstr(&mystring[STRING_LIVE_DATA]);
}

void screen_live_data(void)
{

    /*
    snprintf(PrintString,sizeof(PrintString),"CLKPR:"BYTETOBINARYPATTERN, BYTETOBINARY(CLKPR));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);

    snprintf(PrintString,sizeof(PrintString),"A:"BYTETOBINARYPATTERN, BYTETOBINARY(switch_flag));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|0),PrintString);
    //snprintf(PrintString,sizeof(PrintString),"B:"BYTETOBINARYPATTERN, BYTETOBINARY(TCCR2B));
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);


    snprintf(PrintString,sizeof(PrintString),"P:%5uPWM:%5d",ocr_period,PWMAttrib.pwm_period);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);

    snprintf(PrintString,sizeof(PrintString),"T:%3dD:%3dR:%3d",switch_button_press_timer,switch_repeat_delay_initial,switch_repeat_delay_repeat);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);
    
    switch_state_changed = 0;
    switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);

    return;
    */

    //snprintf(PrintString,sizeof(PrintString),"R:%3d",adcPowerSupplyResult);
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|20),PrintString);

    //snprintf(PrintString,sizeof(PrintString),"A:"BYTETOBINARYPATTERN, BYTETOBINARY(ADMUX));
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);

    //snprintf(PrintString,sizeof(PrintString),"F:"BYTETOBINARYPATTERN, BYTETOBINARY(global_flag));
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|27),PrintString);

    //snprintf(PrintString,sizeof(PrintString),"C:%3d",adcSampleCount);
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|94),PrintString);

    //char filler = "*";
    // STRING_SETTINGS_INVALID
    if (global_flag & (1<<FLAG_PWM_ATTRIB_INVALID))
    {
        lcd_print_addrstr(&mystring[STRING_SETTINGS_INVALID]);
    } else {
	// to improve responsiveness, we only display part of data at a time
	ERROR_LED_PORT &= ~(1<<ERROR_LED_BIT);
        menu_local_flag++;
        switch ( menu_local_flag ) {
            case 1:
		// display pwm duty percentage 
	        //snprintf(PrintString,sizeof(PrintString),"%3d",PWMAttrib.pwm_freq);
                //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_LIVE_DATA_PWM_FREQ_INTEGER_POSITION),PrintString);
	        snprintf(PrintString,sizeof(PrintString),"%3ld",(((uint32_t)PWMAttrib.pwm - PWMAttrib.pwm_min) * 100 / PWMAttrib.pwm_range));
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_LIVE_DATA_CURRENT_PWM_PERCENTAGE_POSITION),PrintString);
                break;

            case 2:
		// display pwm duty cycle

                // 
	        snprintf(PrintString,sizeof(PrintString),"%4d",PWMAttrib.pwm);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_LIVE_DATA_CURRENT_PWM_INTEGER_POSITION),PrintString);
                break;

            case 3:
		// display pwm min center max
	        snprintf(PrintString,sizeof(PrintString)," %4d %4d %4d ",PWMAttrib.pwm_min,PWMAttrib.pwm_center,PWMAttrib.pwm_max);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_LIVE_DATA_PWM_MIN_CENTER_MAX_POSITION),PrintString);
                break;

            default:

		voltageCalculated = refVoltageCorrected / adcPowerSupplyResult;
		if (voltageCalculatedAvg == 0)
		{
		    voltageCalculatedAvg = voltageCalculated;
		} else {
		    voltageCalculatedAvg = ((VOLTMETER_SAMPLE_AVERAGE - 1) * voltageCalculatedAvg + voltageCalculated) / VOLTMETER_SAMPLE_AVERAGE;
		}


		int d1		= voltageCalculatedAvg;            // Get the integer part (678).
		float f2	= voltageCalculatedAvg - d1;   	   // Get fractional part (678.0123 - 678 = 0.0123).
		int d2		= (int)(f2 * 100);		   // Turn into integer (123).

		// Print as parts, note that you need 0-padding for fractional bit.

	        snprintf(PrintString,sizeof(PrintString),"%d.%02d",d1,d2);
		lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_LIVE_DATA_VOLTAGE_INTEGER_POSITION),PrintString);

		/*
		d1	= voltageCorrectionCalculated;
		f2	= voltageCorrectionCalculated - d1;
		d2	= (int)(f2 * 1000);
	        snprintf(PrintString,sizeof(PrintString),"c:%d.%03d",d1,d2);
		lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|31),PrintString);

		d1	= VOLTMETER_BASE_VOLTAGE;            // Get the integer part (678).
		f2	= VOLTMETER_BASE_VOLTAGE - d1;	// Get fractional part (678.0123 - 678 = 0.0123).
		d2	= (int)(f2 * 1000);		// Turn into integer (123).
	        snprintf(PrintString,sizeof(PrintString),"B:%d.%03d",d1,d2);
		lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);

		d1	= refVoltageCorrected;            // Get the integer part (678).
		f2	= refVoltageCorrected - d1;	// Get fractional part (678.0123 - 678 = 0.0123).
		d2	= (int)(f2 * 1000);		// Turn into integer (123).
	        snprintf(PrintString,sizeof(PrintString),"CB:%d.%03d",d1,d2);
		lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|91),PrintString);
		*/

                /*
                snprintf(PrintString,sizeof(PrintString),"A:"BYTETOBINARYPATTERN, BYTETOBINARY(ADMUX));
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|84),PrintString);

                snprintf(PrintString,sizeof(PrintString),"F:"BYTETOBINARYPATTERN, BYTETOBINARY(global_flag));
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|27),PrintString);
		*/

                menu_local_flag = 0;
                break;
        }
    }

    _delay_ms(2);
}

void exit_live_data(void)
{
}


/*
 *
 *
 *
 *	excercise servo
 *
 *
 *
 */
// menu_local_flag bit 0: 0 means minimum PW, 1 means maximum pw
#define SERVO_EXCERCISE_DRIVE_POSTION	0
void init_excercise_servo(void)
{
    lcd_print_addrstr(&mystring[STRING_EXCERCISE_SERVO]);
    lcd_print_addrstr(&mystring[STRING_EXCERCISE_SERVO_LINE2]);
    global_flag &= ~(1<<FLAG_ADC_TO_PWM_ENABLED);
    //adc_get_result();
    servoExcercisePosition = adcResult;
    adcPowerSupplyResult_min = adcPowerSupplyResult;
    adcPowerSupplyResult_max = adcPowerSupplyResult;
    // edit_name_current_char_index = adcResult * (sizeof(servoNameCharSet) - 1) / 1024;
    if (servoExcercisePosition >= 512){
	global_flag2 |= (1<<FLAG2_SERVO_EXCERCISE_POSITION);
    } else {
	global_flag2 &= (1<<FLAG2_SERVO_EXCERCISE_POSITION);
    }

    servo_excercise_timer_start();
    servoExcerciseTimer = servoExcerciseDelay;
}

void screen_excercise_servo(void)
{

    // minimum delay = 1

    // increase delay
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (servoExcerciseDelay >= SERVO_EXCERCISE_MAXIMUM_DELAY){
            servoExcerciseDelay = SERVO_EXCERCISE_MAXIMUM_DELAY;
        } else {
            servoExcerciseDelay++;
	}
        //menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }
    // decrease delay
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (servoExcerciseDelay <= SERVO_EXCERCISE_MINIMUM_DELAY){
            servoExcerciseDelay = SERVO_EXCERCISE_MINIMUM_DELAY;
        } else {
            servoExcerciseDelay--;
	}
    }

    //adc_get_result();
    //servoExcerciseRange = adcResult * servoAttrib.pwm_range / 1023;
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
        global_flag2 ^= (1<<FLAG2_SERVO_EXCERCISE_ENABLED);
	// reset voltmeter minimum and maximum readouts when enabling excercise
	if (global_flag2 & (1<<FLAG2_SERVO_EXCERCISE_ENABLED)){
            adcPowerSupplyResult_min = adcPowerSupplyResult;
            adcPowerSupplyResult_max = adcPowerSupplyResult;
        }
    }


    if (global_flag2 & (1<<FLAG2_SERVO_EXCERCISE_ENABLED))
    {
        uint16_t diff;
        // delay have passed, change direction
        if (servoExcerciseTimer >= servoExcerciseDelay){
            diff = abs(adcResult - 512);
            if (global_flag2 & (1<<FLAG2_SERVO_EXCERCISE_POSITION))
            {
                if (diff > 512)
                {
                    diff = 512;
                }
                servoExcercisePosition = 512 - diff;
                global_flag2 &= ~(1<<FLAG2_SERVO_EXCERCISE_POSITION);
            }
            else
            {
                if (diff > 511)
                {
                    diff = 511;
                }
                servoExcercisePosition = 512 + diff;
                global_flag2 |= (1<<FLAG2_SERVO_EXCERCISE_POSITION);
            }
            servoExcerciseTimer = 0;
        }
        else
        {
            // delay not yet passed, check for timer compare flag
            if (TIFR2 & (1<<OCF2A))
            {
                servoExcerciseTimer++;
                TIFR2 |= (1<<OCF2A);
            }
        }
    }

    // !!! adcPowerSupplyResult is inversely proportional to voltage !!!
    if (adcPowerSupplyResult > adcPowerSupplyResult_min)
    {
        adcPowerSupplyResult_min = adcPowerSupplyResult;
    }
    if (adcPowerSupplyResult < adcPowerSupplyResult_max)
    {
        adcPowerSupplyResult_max = adcPowerSupplyResult;
    }

    uint8_t percent;

    //snprintf(PrintString,sizeof(PrintString),"OCR2A:%3dOCR2B:%3d",OCR2A,OCR2B);
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|20),PrintString);

    menu_local_flag++;
    switch ( menu_local_flag ) {
        case 1:
            // display excercise delay
	    // delay in microseconds: servoExcerciseDelay * 1024 * SERVO_EXCERCISE_RESOLUTION / 1000000 

            snprintf(PrintString,sizeof(PrintString),"%5ld", (uint32_t)SERVO_EXCERCISE_RESOLUTION_DISPLAY * servoExcerciseDelay * 1024 / 1000 );
            lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_DELAY_POSITION),PrintString);
            break;

        case 2:
            // minimum %
            percent = (uint32_t)adcResult * 100 / 1023;
            if (percent > 50)
            {
                snprintf(PrintString,sizeof(PrintString),"%3d",100-percent);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_PERCENT_MIN),PrintString);
            }
            else 
            {
                snprintf(PrintString,sizeof(PrintString),"%3d",percent);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_PERCENT_MIN),PrintString);
            }
          
            break;
        case 3:
            // maximum %

            percent = (uint32_t)adcResult * 100 / 1023;
            if (percent > 50)
            {
                snprintf(PrintString,sizeof(PrintString),"%3d",percent);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_PERCENT_MAX),PrintString);

            }
            else 
            {
                snprintf(PrintString,sizeof(PrintString),"%3d",100-percent);
                lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_PERCENT_MAX),PrintString);
            }
          
            break;

        case 4:

            // highest voltage
            voltageCalculated = refVoltageCorrected * VOLTMETER_CORRECTION_RESOLUTION / adcPowerSupplyResult_max;
            voltageCalculated = refVoltageCorrected / adcPowerSupplyResult_max;


            int d1          = voltageCalculated;            // Get the integer part (678).
            float f2        = voltageCalculated- d1;       // Get fractional part (678.0123 - 678 = 0.0123).
            int d2          = (int)(f2 * 100);                 // Turn into integer (123).

            snprintf(PrintString,sizeof(PrintString),"%d.%02d",d1,d2);
            lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_HIGHEST_VOLTAGE_POSITION),PrintString);


            break;

        /*
        case 5:

            snprintf(PrintString,sizeof(PrintString),"SDR:%3d",switch_repeat_delay_repeat);
            lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2|20),PrintString);
            break;

        case 6:
            snprintf(PrintString,sizeof(PrintString),"SF:"BYTETOBINARYPATTERN, BYTETOBINARY(switch_flag));
            lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|20),PrintString);

            break;
        */

        default:
            // lowest voltage
            // voltageCalculated = refVoltageCorrected * 1023 / adcPowerSupplyResult_min;
            voltageCalculated = refVoltageCorrected / adcPowerSupplyResult_min;

            d1          = voltageCalculated;            // Get the integer part (678).
            f2        = voltageCalculated- d1;       // Get fractional part (678.0123 - 678 = 0.0123).
            d2          = (int)(f2 * 100);                 // Turn into integer (123).

            snprintf(PrintString,sizeof(PrintString),"%d.%02d",d1,d2);
            lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_EXCERCISE_LOWEST_VOLTAGE_POSITION),PrintString);

            menu_local_flag = 0;
            break;
    }

        
}

void exit_excercise_servo(void)
{
    global_flag |= (1<<FLAG_ADC_TO_PWM_ENABLED);
    global_flag2 &= ~(1<<FLAG2_SERVO_EXCERCISE_ENABLED);
    
    servo_excercise_timer_stop();
}


/*
 *
 *
 *
 *	LOAD SERVO SETTINGS SCREEN
 *
 *
 *
 */
#define TEMP_FLAG_ID_CHANGE	1
#define TEMP_FLAG_DELAY_PASSED	2
void init_servo_load(void)
{
    lcd_print_addrstr(&mystring[STRING_SERVO_LOAD]);
    menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
    menu_servo_id = servoAttrib.id;
}

void screen_servo_load(void)
{

    /*
     * previous/next servo
     */

    //next servo
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (menu_servo_id >= SERVO_MEMORY_SLOTS){
	    menu_servo_id = 1;
    	} else {
	    menu_servo_id++;
        }
        menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
    }

    //previous servo
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (menu_servo_id <= 1){
	    menu_servo_id = SERVO_MEMORY_SLOTS;
    	} else {
	    menu_servo_id--;
        }
        menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
    }

    // if currently selected servo id changes, load name and display
    if (menu_local_flag & (1<<TEMP_FLAG_ID_CHANGE)){
	// display servo id
	print_settings_servo_id_and_name(SERVO_SETTINGS_LOAD_SERVO_ID_POSTION);
        // clear id change flag
        menu_local_flag &= ~(1<<TEMP_FLAG_ID_CHANGE);
    }

    //load seleced servo
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
	// load servo settings
	load_servo_data(menu_servo_id);
        // display loaded message
        lcd_print_addrstr(&mystring[STRING_SERVO_LOADED]);
        superdelay(); superdelay();

        global_flag |= (1<<FLAG_PWM_ATTRIB_INVALID);
	//quit to screen 0
	(*menu_exit_handlers[current_function])();
	current_function = 0;
	menu_change_general_todo();
    }

    //switch_state_changed &= ~(1<<SWITCH_LEFT_ID);
    //switch_state_changed &= ~(1<<SWITCH_RIGHT_ID);
    //switch_state_changed &= ~(1<<SWITCH_CENTER_ID);

    //switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_servo_load(void)
{
}


/*
 *
 *
 *
 *	SERVO NAME
 *
 *
 *
 */
void init_settings_servo_name(void)
{
    lcd_print_addrstr(&mystring[STRING_SETTINGS_SERVO_NAME]);
    snprintf(PrintString,sizeof(PrintString),"%2d",servoAttrib.id);
    lcd_printstring(PrintString);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),servoAttrib.name);
    lcd_write(1,0,(LCD_ONOFF_SELECT|LCD_ONOFF_DISPLAY|LCD_ONOFF_CURSOR|LCD_ONOFF_CURSOR_BLINK));
    // detach adc from PWM generator, so servo will not move when selecting characters
    global_flag &= ~(1<<FLAG_ADC_TO_PWM_ENABLED);
    edit_name_cursor_position = 0;
    //adc_get_result();

    switch_repeat_delay_repeat	= SWITCH_REPEATER_REPEAT_NAME;

    edit_name_current_char_index = adcResult * (sizeof(servoNameCharSet) - 1) / 1024;
    lcd_write(1,0,(LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_NAME_SELECTED_CHAR_POSITION));
    lcd_print_character(servoNameCharSet[edit_name_current_char_index]);
}
//menu_local_flag
#define FLAG_SERVO_NAME_CHAR_INDEX_CHANGED	0
#define FLAG_SERVO_NAME_CHAR_CHANGED		0
void screen_settings_servo_name(void)
{
    /*
     * previous/next character
     */
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (edit_name_cursor_position >= 15){
	    edit_name_cursor_position = 0;
    	} else {
	    edit_name_cursor_position++;
        }
    }

    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (edit_name_cursor_position){
	    edit_name_cursor_position--;
    	} else {
	    edit_name_cursor_position = 15;
        }
    }

    // calculate selected character by adc input
    uint8_t selected_character_index;
    //adc_get_result();
    selected_character_index = adcResult * (sizeof(servoNameCharSet) - 1) / 1024;

    //        1024 / 64


    // if center button pressed, insert character 
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
	//servoAttrib.name[edit_name_cursor_position] = adcCharacter
	servoAttrib.name[edit_name_cursor_position] = servoNameCharSet[selected_character_index];
	lcd_print_character(servoNameCharSet[selected_character_index]);
#ifdef SERVO_NAME_EDITOR_AUTO_ADVANCE_CURSOR
	if (edit_name_cursor_position < 16){
	    edit_name_cursor_position++;
	} else {
	    edit_name_cursor_position = 0;
        }
#endif
    }


    if (edit_name_current_char_index != selected_character_index)
    {
        lcd_write(1,0,(LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_NAME_SELECTED_CHAR_POSITION));
        lcd_print_character(servoNameCharSet[selected_character_index]);
	edit_name_current_char_index = selected_character_index;
    }

    // move cursor to position
    lcd_write(1,0,(LCD_COMMAND_MOVE_CURSOR|LINE_2|edit_name_cursor_position));

    //switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_settings_servo_name(void)
{
    lcd_write(1,0,(LCD_ONOFF_SELECT|LCD_ONOFF_DISPLAY));
    global_flag |= (1<<FLAG_ADC_TO_PWM_ENABLED);
}


/*
 *
 *
 *
 *	PWM CENTER SCREEN
 *
 *
 *
 */
#define	PWM_ATTRIB_CHANGED	0
void init_settings_pwm_center(void)
{
    lcd_print_addrstr(&mystring[STRING_SETTINGS_PWM_CENTER]);
    menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
}
void screen_settings_pwm_center(void)
{
    //snprintf(PrintString,sizeof(PrintString),"A:"BYTETOBINARYPATTERN, BYTETOBINARY(switch_flag));
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|0),PrintString);

    /*
     * increase/decrease
     */
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_center += SERVO_PWM_CENTER_STEPPING;
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
        if (servoAttrib.pwm_center >= SERVO_PWM_CENTER_MAXIMUM){
            servoAttrib.pwm_center = SERVO_PWM_CENTER_MAXIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }
    //
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_center -= SERVO_PWM_CENTER_STEPPING;
        if (servoAttrib.pwm_center <= SERVO_PWM_CENTER_MINIMUM){
            servoAttrib.pwm_center = SERVO_PWM_CENTER_MINIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }

    if (menu_local_flag & (1<<PWM_ATTRIB_CHANGED)){
        print_servo_attrib_pwm_center();
        print_servo_attrib_pwm_min_center_max();
        global_flag |= (1<<FLAG_PWM_ATTRIB_INVALID);
        menu_local_flag &= ~(1<<PWM_ATTRIB_CHANGED);
    }

    /*
    snprintf(PrintString,sizeof(PrintString),"A:"BYTETOBINARYPATTERN, BYTETOBINARY(switch_flag));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|0),PrintString);

    snprintf(PrintString,sizeof(PrintString),"T:%3dD:%3dR:%3d",switch_button_press_timer,switch_repeat_delay_initial,switch_repeat_delay_repeat);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);
    */

    // center button: exit to live data
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
        //quit to screen 0
        (*menu_exit_handlers[current_function])();
        current_function = 0;
        menu_change_general_todo();
    }
 
    //switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_settings_pwm_center(void)
{
}


/*
 *
 *
 *
 *	PWM RANGE SCREEN
 *
 *
 *
 */
void init_settings_pwm_range(void)
{
    lcd_print_addrstr(&mystring[STRING_SETTINGS_PWM_RANGE]);
    menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
}
void screen_settings_pwm_range(void)
{
    /*
     * increase/decrease
     */
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_range += SERVO_PWM_RANGE_STEPPING;
        if (servoAttrib.pwm_range >= SERVO_PWM_RANGE_MAXIMUM){
            servoAttrib.pwm_range = SERVO_PWM_RANGE_MAXIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }
    //
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_range -= SERVO_PWM_RANGE_STEPPING;
        if (servoAttrib.pwm_range <= SERVO_PWM_RANGE_MINIMUM){
            servoAttrib.pwm_range = SERVO_PWM_RANGE_MINIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }

    // center button: exit to live data
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
        //quit to screen 0
        (*menu_exit_handlers[current_function])();
        current_function = 0;
        menu_change_general_todo();
    }

    if (menu_local_flag & (1<<PWM_ATTRIB_CHANGED)){
        print_servo_attrib_pwm_range();
        print_servo_attrib_pwm_min_center_max();
        menu_local_flag &= ~(1<<PWM_ATTRIB_CHANGED);
        global_flag |= (1<<FLAG_PWM_ATTRIB_INVALID);
    }

    //switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_settings_pwm_range(void)
{
}



/*
 *
 *
 *
 *	PWM FREQUENCY SCREEN
 *
 *
 *
 */
void init_settings_pwm_freq(void)
{
    lcd_print_addrstr(&mystring[STRING_SETTINGS_PWM_FREQ]);
    lcd_print_addrstr(&mystring[STRING_SETTINGS_PWM_PERIOD]);
    menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
}
void screen_settings_pwm_freq(void)
{
    /*
     * increase/decrease
     */
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_freq += SERVO_PWM_FREQ_STEPPING;
        if (servoAttrib.pwm_freq >= SERVO_PWM_FREQ_MAXIMUM){
            servoAttrib.pwm_freq = SERVO_PWM_FREQ_MAXIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }
    //
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        servoAttrib.pwm_freq -= SERVO_PWM_FREQ_STEPPING;
        if (servoAttrib.pwm_freq <= SERVO_PWM_FREQ_MINIMUM){
            servoAttrib.pwm_freq = SERVO_PWM_FREQ_MINIMUM;
        }
        menu_local_flag |= (1<<PWM_ATTRIB_CHANGED);
    }

    // center button: exit to live data
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
        //quit to screen 0
        (*menu_exit_handlers[current_function])();
        current_function = 0;
        menu_change_general_todo();
    }
 
    if (menu_local_flag & (1<<PWM_ATTRIB_CHANGED)){
        print_servo_attrib_pwm_frequency();
        print_servo_attrib_pwm_period();
        menu_local_flag &= ~(1<<PWM_ATTRIB_CHANGED);
        global_flag |= (1<<FLAG_PWM_ATTRIB_INVALID);
    }
    //switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_settings_pwm_freq(void)
{
}


void init_settings_voltmeter_calibrate(void)
{
    lcd_print_addrstr(&mystring[STRING_VOLTMETER_CALIBRATE]);
}

void screen_settings_voltmeter_calibrate(void)
{
    // FIXME save setting
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (testerConfig.voltageCorrection < VOLTMETER_CORRECTION_RESOLUTION)
        {
            testerConfig.voltageCorrection++;
        }
        else
        {
            testerConfig.voltageCorrection = VOLTMETER_CORRECTION_RESOLUTION;
        }
    }
    //
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (testerConfig.voltageCorrection > 0)
        {
            testerConfig.voltageCorrection--;
        }
    }
    // center button: exit to live data
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
        //quit to screen 0
        (*menu_exit_handlers[current_function])();
        current_function = 0;
        menu_change_general_todo();
    }

    refVoltageCorrected = (VOLTMETER_BASE_VOLTAGE*VOLTMETER_CORRECTION_RESOLUTION) + (testerConfig.voltageCorrection * VOLTMETER_CORRECTION_MAX * 2);
    voltageCalculated = refVoltageCorrected / adcPowerSupplyResult;

    if (voltageCalculatedAvg == 0)
    {
        voltageCalculatedAvg = voltageCalculated;
    }
    else
    {
        voltageCalculatedAvg = ((VOLTMETER_SAMPLE_AVERAGE - 1) * voltageCalculatedAvg + voltageCalculated) / VOLTMETER_SAMPLE_AVERAGE;
    }


    int d1		= voltageCalculatedAvg;            // Get the integer part (678).
    float f2		= voltageCalculatedAvg - d1;   	   // Get fractional part (678.0123 - 678 = 0.0123).
    int d2		= (int)(f2 * 100);		   // Turn into integer (123).

    // Print as parts, note that you need 0-padding for fractional bit.

    snprintf(PrintString,sizeof(PrintString),"%d.%02dV",d1,d2);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|VOLTMETER_CALIBRATE_INTEGER_POSITION),PrintString);

    //snprintf(PrintString,sizeof(PrintString),"Corr:%4d",testerConfig.voltageCorrection);
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2|20),PrintString);

}

void exit_settings_voltmeter_calibrate(void)
{
    save_settings();

}


/*
 *
 *
 *
 *	SETTINGS SAVE SCREEN
 *
 *
 *
 */
void init_servo_save(void)
{
    lcd_print_addrstr(&mystring[STRING_SETTINGS_SAVE]);
    menu_servo_id = servoAttrib.id;
    menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
}
void screen_servo_save(void)
{
    /*
     * previous/next servo
     */
    //next servo
    if ( (switch_state & (1<<SWITCH_RIGHT_ID)) && ( (switch_state_changed & (1<<SWITCH_RIGHT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (menu_servo_id >= SERVO_MEMORY_SLOTS){
	    menu_servo_id = 1;
    	} else {
	    menu_servo_id++;
        }
        menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
    }

    //previous servo
    if ( (switch_state & (1<<SWITCH_LEFT_ID)) && ( (switch_state_changed & (1<<SWITCH_LEFT_ID)) || (switch_flag & (1<<SWITCH_FLAG_DO_REPEAT))))
    {
        if (menu_servo_id <= 1){
	    menu_servo_id = SERVO_MEMORY_SLOTS;
    	} else {
	    menu_servo_id--;
        }
        menu_local_flag |= (1<<TEMP_FLAG_ID_CHANGE);
    }


    // if currently selected servo id changes, load name and display
    if (menu_local_flag & (1<<TEMP_FLAG_ID_CHANGE)){
	// display servo id
	print_settings_servo_id_and_name(SERVO_SETTINGS_SAVE_SERVO_ID_POSTION);
        // clear id change flag
        menu_local_flag &= ~(1<<TEMP_FLAG_ID_CHANGE);
    }

    //save seleced servo
    if ((switch_state & (1<<SWITCH_CENTER_ID)) & (switch_state_changed & (1<<SWITCH_CENTER_ID)))
    {
	// load servo settings
	save_servo_data(menu_servo_id);
        // display loaded message
        lcd_print_addrstr(&mystring[STRING_SERVO_SAVED]);
        lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),servoAttrib.name);
        superdelay(); superdelay();

	//quit to screen 0
	(*menu_exit_handlers[current_function])();
	current_function = 0;
	menu_change_general_todo();
    }
    switch_flag &= ~(1<<SWITCH_FLAG_DO_REPEAT);
}

void exit_servo_save(void)
{
}



void load_settings(void)
{

    testerConfigStruct testerConfigTmp;
    eeprom_read_block (&testerConfigTmp , &nv_testerConfig, sizeof(testerConfigStruct));

    if(testerConfigTmp.configVersion < CONFIG_VERSION && (testerConfigTmp.voltageCorrection == 0 || testerConfigTmp.voltageCorrection == 1023))
    {
        // if EEPROM config version is lower than current and there seem to be no valid correction factor,
        // ignore EEPROM contents and leave default values as they are
    }
    else
    {
        // we seem to have found valid voltage correction factor
        testerConfig = testerConfigTmp;
    }
}

void save_settings(void)
{
    eeprom_update_block (&testerConfig , &nv_testerConfig, sizeof(testerConfigStruct));
}


void load_servo_data(uint8_t servo_id)
{
    if (servo_id)
    {
        servo_id--;
    }

    if (servo_id > SERVO_MEMORY_SLOTS-1){
	servo_id = SERVO_MEMORY_SLOTS-1;
    }

    eeprom_read_block (&servoAttrib , &nv_servoAttrib[servo_id], sizeof(servoAttribStruct));

    if(servoAttrib.id > 0 && servoAttrib.id <= SERVO_MEMORY_SLOTS){
	// we loaded a valid servo definition
	return;
    }

    lcd_print_addrstr(&mystring[STRING_ERROR_EEPROM_LOAD_FAIL]);
    superdelay(); superdelay();
    wdt_enable(WDTO_15MS);
    while(1)
    {
    }
}


void save_servo_data(uint8_t servo_id)
{
    if (servo_id)
    {
	servoAttrib.id = servo_id;
        servo_id--;
    }

    if (servo_id > SERVO_MEMORY_SLOTS-1){
	servoAttrib.id = SERVO_MEMORY_SLOTS;
	servo_id = SERVO_MEMORY_SLOTS-1;
    }

    eeprom_update_block (&servoAttrib , &nv_servoAttrib[servo_id], sizeof(servoAttribStruct));

}



void superdelay(void)
{
        _delay_ms(255);
        _delay_ms(255);
        _delay_ms(255);
        _delay_ms(255);
        _delay_ms(255);
}


/*
 *
 * do not let program begin until all button released
 *
 */
void startup_check_for_stuck_buttons(void)
{
    //read initial switch states
    switch_refresh_all_state();
    // check for stuck keys
    while(switch_pressed_button_count())
    {
        // notify keys pressed 
    	switch_refresh_all_state();
	// blink screen
	//snprintf(PrintString,sizeof(PrintString),"P:"BYTETOBINARYPATTERN",L:%3d", BYTETOBINARY(switch_state),loop++);
        //snprintf(PrintString,sizeof(PrintString),">>%4d<<",switch_pressed_button_count());
        //lcd_write(1,0,(LCD_COMMAND_MOVE_CURSOR|0));
        //lcd_printstring(PrintString);
        lcd_print_addrstr(&mystring[STRING_RELASE_BUTTONS]);
	// print release button message
    }
    switch_clear_button_changed();
}


inline void clearscreen(void)
{
    lcd_write(1,0,LCD_COMMAND_CLEAR_SCREEN);
}



void debug(uint8_t step)
{
    //snprintf(PrintString,sizeof(PrintString),BYTETOBINARYPATTERN,BYTETOBINARY(~(PINB)));
    //lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR),PrintString);
    snprintf(PrintString,sizeof(PrintString),"s%2d m%2d",step,current_function);
    //lcd_printstring(PrintString);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|8),PrintString);
    snprintf(PrintString,sizeof(PrintString),BYTETOBINARYPATTERN,BYTETOBINARY(switch_state));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);
    snprintf(PrintString,sizeof(PrintString),BYTETOBINARYPATTERN,BYTETOBINARY(switch_state_changed));
    lcd_printstring(PrintString);
    superdelay();
}



/*

	adc will alternatively user adc0 and 1.1v as input, writing
	the result in the corresponding variable.
	This divides the effective sample rate to half, but it does not really matter as
	sample rate is about 100 kHz anyway.


 */
void adc_get_result(void)
{
    if(!(ADCSRA & (1<<ADSC))){
        // if ADSC not set, conversion is over.
        theLowADC = ADCL;

        if (global_flag & (1<<FLAG_ADC_INPUT)){

         /*
            // result is input voltage
            adcSampleCount++;
	    //ERROR_LED_PORT |= (1<<ERROR_LED_BIT);

            adcPowerSupplyResult = ADCH<<8 | theLowADC;
            // bandgap 1.1v input needs some time to stabilize,
            // so we discard the first VOLTMETER_SAMPLE_COUNT-1 samples
            if (adcSampleCount >= VOLTMETER_SAMPLE_COUNT){
	        // set input to potmeter for next round
	        adc_set_input_adc0();
	        global_flag &= ~(1<<FLAG_ADC_INPUT);
                adcSampleCount = 0;
            }
         */

            adcPowerSupplyResult = ADCH<<8 | theLowADC;
            adc_set_input_adc0();
	    global_flag &= ~(1<<FLAG_ADC_INPUT);

        } else {
            // result is potmeter position
            adcResult = ADCH<<8 | theLowADC;
	    // set input to band gap voltage for next round
	    adc_set_input_1_1v();
            _delay_ms(VOLTMETER_SAMPLE_DELAY);
	    global_flag |= (1<<FLAG_ADC_INPUT);
	    //ERROR_LED_PORT &= ~(1<<ERROR_LED_BIT);
        }

        //snprintf(PrintString,sizeof(PrintString),"%4d",adcResult);
        //lcd_write(1,0,(LCD_COMMAND_MOVE_CURSOR|11));
        //lcd_printstring(adcResultString);
	//superdelay();
        adc_start_conversion();
    }
}


void print_settings_servo_id_and_name(uint8_t id_position)
{
    snprintf(PrintString,sizeof(PrintString),"%2d",menu_servo_id);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_LOAD_SERVO_ID_POSTION),PrintString);
    // load servo name (id)
    eeprom_read_block (&PrintString , &nv_servoAttrib[menu_servo_id-1].name, sizeof(servoAttrib.name));
    // display servo name
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);
}


inline void print_servo_attrib_pwm_min_center_max(void)
{
    snprintf(PrintString,sizeof(PrintString)," %4d %4d %4d",(servoAttrib.pwm_center - servoAttrib.pwm_range),servoAttrib.pwm_center,(servoAttrib.pwm_center + servoAttrib.pwm_range));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2),PrintString);
}

inline void print_servo_attrib_pwm_center(void)
{
    snprintf(PrintString,sizeof(PrintString),"%4d",servoAttrib.pwm_center);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_PWM_CENTER_INTEGER_POSITION),PrintString);
}

inline void print_servo_attrib_pwm_range(void)
{
    snprintf(PrintString,sizeof(PrintString),"%4d",servoAttrib.pwm_range);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_PWM_RANGE_INTEGER_POSITION),PrintString);
}

inline void print_servo_attrib_pwm_frequency(void)
{
    snprintf(PrintString,sizeof(PrintString),"%3d",servoAttrib.pwm_freq);
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|SERVO_SETTINGS_PWM_FREQ_INTEGER_POSITION),PrintString);
}

inline void print_servo_attrib_pwm_period(void)
{
    snprintf(PrintString,sizeof(PrintString),"%5ld",(1000000 / servoAttrib.pwm_freq));
    lcd_cmd_printstring((LCD_COMMAND_MOVE_CURSOR|LINE_2|SERVO_SETTINGS_PWM_PERIOD_INTEGER_POSITION),PrintString);
}


void validate_servo_data(void)
{
    /*
     *
     *		period length in usec = 1 000 000 / freq, example: 50 Hz, period: 1000000/20 = 50000 usec
     *                                                            300 Hz, period:               3333 usec
     *		max duty cycle length: pwm_center + pwm_range 
     *
     *		minimum period length: max_duty_cycle_length + SERVO_PWM_MINIMUM_PADDING
     *
     *
     */

    if ((servoAttrib.pwm_center + servoAttrib.pwm_range + SERVO_PWM_MINIMUM_PADDING) > (1000000 / servoAttrib.pwm_freq))
    {
        // return means data is not validated and not copied into live servo data
        return;
    }

    if (servoAttrib.pwm_range > (servoAttrib.pwm_center - SERVO_PWM_MINIMUM_DUTY)){
        return;
    } 

    //if data valid, copy into PWM

    PWMAttrib.pwm_min		= servoAttrib.pwm_center - servoAttrib.pwm_range; 
    PWMAttrib.pwm_center	= servoAttrib.pwm_center; 
    PWMAttrib.pwm_max		= servoAttrib.pwm_center + servoAttrib.pwm_range; 
    PWMAttrib.pwm_range		= servoAttrib.pwm_range * 2; 
    PWMAttrib.pwm_freq		= servoAttrib.pwm_freq;
    PWMAttrib.pwm_period	= (1000000 / servoAttrib.pwm_freq);
    PWMAttrib.pwm		= servoAttrib.pwm_center; 

    global_flag &= ~(1<<FLAG_PWM_ATTRIB_INVALID);
}



void servo_excercise_timer_start(void)
{
    servoExcerciseTimer = 0;
    // set TCNT0 to 0
    TCNT2 = 0;
    // set OCR2A to SWITCH_REPEATER_DELAY_RESOLUTION
    OCR2A = SERVO_EXCERCISE_RESOLUTION;
    // clear TIFR0 OCF2A bit
    TIFR2 |= (1<<OCF2A)|(1<<TOV2);
    // set up timer2 CTC mode and start timer
    // WGM21: clear timer on match, TOP: OCR2A
    TCCR2A = (1<<WGM21);
    TCCR2B = SERVO_EXCERCISE_DELAY_PRESCALER;
}

inline void servo_excercise_timer_stop(void)
{
    // stop timer2 - actually this is only a power-saving matter
    TCCR2A = 0;
    TCCR2B = 0;
}


