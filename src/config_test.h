#ifndef CONFIG
#define CONFIG


/*


	CONFIGURATION OPTIONS


*/
/*         GENERAL         */
/* CPU speed 1 MHZ (CLK/8) */
#define F_CPU			1000000UL
#define CLOCK_MULTIPLIER        (F_CPU / 1000000)

#define FIRMWARE_VERSION	"0.0.12"



// this much EEPROM memory we have
#define MEM_EEPROM 2048


// editor settings
#define SERVO_NAME_EDITOR_AUTO_ADVANCE_CURSOR

/*


	SERVO SETTINGS



 */
#define SERVO_PWM_CENTER_MINIMUM	760
#define SERVO_PWM_CENTER_MAXIMUM	1520
#define SERVO_PWM_CENTER_STEPPING	10

#define SERVO_PWM_RANGE_MINIMUM		100
#define SERVO_PWM_RANGE_MAXIMUM		1000
#define SERVO_PWM_RANGE_STEPPING	10

#define SERVO_PWM_FREQ_MINIMUM		50
#define SERVO_PWM_FREQ_MAXIMUM		300
#define SERVO_PWM_FREQ_STEPPING		10

#define SERVO_PWM_MINIMUM_DUTY		50 // minimum duty cycle length
#define SERVO_PWM_MINIMUM_PADDING	50 // usec between two duty cycles



/*


	     LCD

	LCD DATA PORT



*/
#define LCD_DATA_PORT PORTD
#define LCD_DATA_DIR DDRD

// define LCD_DATABITS_4 if LCD is connected in 4bit mode.
#define LCD_DATABITS_4
// currently 4 bits must be connected to PORTx pins 7-4 (high bits) or
// pins 0-3. If LCD data is connected to low pins, define LCD_DATABITS_4_LOW
#define LCD_DATABITS_4_LOW

/*
	LCD CONTROL PORT + PINS
*/
#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_DIR DDRD

#define LCD_CTRL_ENABLE_BIT     7
#define LCD_CTRL_RW_BIT         6
#define LCD_CTRL_RS_BIT         5

#define LCD_HIDE_CURSOR


/*


	   ADC


*/

#define ADC_CLOCK_DIV_BIT_2_HIGH     //  div 16
#define ADC_PORT        PORTC
#define ADC_PORT_PIN    PC0

/*





	SWITCH





*/


/*

	DEBOUNCER - on timer0 

        CS02:0 clock prescaler (CS02 CS01 and CS00 bits)
            0 0 0       timer stopped
            0 0 1       no prescale
            0 1 0       /8
            0 1 1       /64
            1 0 0       /256
            1 0 1       /1024

*/

#define SWITCH_DEBOUNCER_PRESCALER	((1<<CS02)|(1<<CS00))		// clock prescaler can be either 0, 8, 64, 256, 1024
#define SWITCH_DEBOUNCER_DELAY		(250 * CLOCK_MULTIPLIER)	// in timer ticks, must not exceed 255


/*

	REPEATER - on timer2 

        CS22:0 clock prescaler (CS22 CS21 and CS20 bits)
            0 0 0       timer stopped
            0 0 1       no prescale
            0 1 0       /8
            0 1 1       /64
            1 0 0       /256
            1 0 1       /1024

	will activate after switch debouncer threshold
*/

#define SWITCH_REPEATER_PRESCALER		((1<<CS22)|(1<<CS20))		// clock prescaler can be either 0, 8, 64, 256, 1024
/*
	Since the built-n 8bit timer can only count up to 255, wich is about 255 ms when running at 1 MHz and only 127 when 2 MHz,
	we will count the number of overflows so we can define something like: 100 clock ticks * CLOCK_MULTIPLIER * overflows ~= 100 * x ms 

	actual delays and repeat rates are up to the actual menu handling routine
*/
#define SWITCH_REPEATER_DELAY_RESOLUTION	(200 * CLOCK_MULTIPLIER)	// about 25 msec when 1024 prescaler set

#define SWITCH_REPEATER_DELAY_GENERAL		18			// 25 * 40 ~= 1000 msec
#define SWITCH_REPEATER_REPEAT_GENERAL		6			// 25 * 12 ~=  300 msec


#define SWITCH_UP_ID		0
#define SWITCH_UP_PORT		PORTB
#define SWITCH_UP_PIN		PINB
#define SWITCH_UP_DIR		DDRB
#define SWITCH_UP_BIT		0
#define SWITCH_UP_PULLUP	1

// PB0: RX input (ICP1)

#define SWITCH_DOWN_ID		1
#define SWITCH_DOWN_PORT	PORTB
#define SWITCH_DOWN_PIN		PINB
#define SWITCH_DOWN_DIR		DDRB
#define SWITCH_DOWN_BIT		1
#define SWITCH_DOWN_PULLUP	1

// PB2: PWM output (OC1B)

#define SWITCH_LEFT_ID		2
#define SWITCH_LEFT_PORT	PORTB
#define SWITCH_LEFT_PIN		PINB
#define SWITCH_LEFT_DIR		DDRB
#define SWITCH_LEFT_BIT		3
#define SWITCH_LEFT_PULLUP	1

#define SWITCH_CENTER_ID	3
#define SWITCH_CENTER_PORT	PORTB
#define SWITCH_CENTER_PIN	PINB
#define SWITCH_CENTER_DIR	DDRB
#define SWITCH_CENTER_BIT	4
#define SWITCH_CENTER_PULLUP	1

#define SWITCH_RIGHT_ID		4
#define SWITCH_RIGHT_PORT	PORTB
#define SWITCH_RIGHT_PIN	PINB
#define SWITCH_RIGHT_DIR	DDRB
#define SWITCH_RIGHT_BIT	5
#define SWITCH_RIGHT_PULLUP	1

#define SWITCH_SOURCE_ID	5
#define SWITCH_SOURCE_PORT	PORTC
#define SWITCH_SOURCE_PIN	PINC
#define SWITCH_SOURCE_DIR	DDRC
#define SWITCH_SOURCE_BIT	4
#define SWITCH_SOURCE_PULLUP	0




#endif


