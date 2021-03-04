#ifndef CONFIG
#define CONFIG


/*


	CONFIGURATION OPTIONS


*/
/*         GENERAL         */
/* CPU speed 1 MHZ (CLK/8) */
#define F_CPU			3000000UL
#define CLOCK_MULTIPLIER        (F_CPU / 1000000)
//  prescaler: 4 (12 MHz crystal / 4 = 3 MHz)
#define CLOCK_PRESCALER         (1<<CLKPS1) 

#define FIRMWARE_VERSION	"0.2.1"
// to store in eeprom for future reference
#define CONFIG_VERSION		1



// this much EEPROM memory we have
#define MEM_EEPROM 2048


// editor settings
#define SERVO_NAME_EDITOR_AUTO_ADVANCE_CURSOR


// voltmeter settings: base voltage and correction factor
#define VOLTMETER_MEDIAN_REF_VOLTAGE	1.1
#define VOLTMETER_CORRECTION_MAX	0.2
#define VOLTMETER_BASE_VOLTAGE		(VOLTMETER_MEDIAN_REF_VOLTAGE - VOLTMETER_CORRECTION_MAX)

#define VOLTMETER_SAMPLE_DELAY		2    // ms to delay after setting reference to bandgap and before starting conversion
					     // adc circuitry needs some time to stabilize 1.1 volt

#define VOLTMETER_SAMPLE_AVERAGE	8     // average samples to provide consystent display. should be power of two  so
					      // compiler may optimize

#define VOLTMETER_CORRECTION_RESOLUTION	1023   // 0 - 1023, default voltmeter correction if one does not exist in EEPROM
#define VOLTMETER_DEFAULT_CORRECTION	512   // 0 - 1023, default voltmeter correction if one does not exist in EEPROM


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

#define LCD_CTRL_ENABLE_BIT     4
#define LCD_CTRL_RW_BIT         5
#define LCD_CTRL_RS_BIT         6

#define LCD_HIDE_CURSOR


/*


	   ADC


*/

#define ADC_CLOCK_DIV_BIT_2_HIGH     // 
#define ADC_CLOCK_DIV_BIT_0_HIGH     //  div 32 - 3MHz/32 = 93750 Hz

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

#define SWITCH_TIMER_PRESCALER	((1<<CS02)|(1<<CS00))		// /1024 at 3 MHz =~ 341 us =~ 1/3 ms
#define SWITCH_TIMER_RESOLUTION	(25 * CLOCK_MULTIPLIER)		// ~1 ms * 25 =~ 25 ms, maximum 255

#define SWITCH_DEBOUNCER_DELAY		10			// 25 ms * 20 ~=  500 msec 

#define SWITCH_REPEATER_DELAY_GENERAL		20		// 25 ms * 20 ~=  500 msec
#define SWITCH_REPEATER_REPEAT_GENERAL		7		// 25 ms * 7  ~=  175 msec
#define SWITCH_REPEATER_REPEAT_NAME		12		// 25 ms * 12 ~=  300 msec


/*

	SERVO EXCERCISER DELAY - on timer2

        CS22:0 clock prescaler (CS22 CS21 and CS20 bits)
            0 0 0       timer stopped
            0 0 1       no prescale
            0 1 0       /8
            0 1 1       /32
            1 0 0       /64
            1 0 1       /128
            1 1 0       /256
            1 1 1       /1024

	will activate after switch debouncer threshold
*/

#define SERVO_EXCERCISE_DELAY_PRESCALER		((1<<CS22)|(1<<CS21)|(1<<CS20))		// 1024 at 3 MHz =~ 341 us =~ 1/3 ms
/*
	Since the built-n 8bit timer can only count up to 255, wich is about 255 ms when running at 1 MHz and only 127 when 2 MHz,
	we will count the number of overflows so we can define something like: 100 clock ticks * CLOCK_MULTIPLIER * overflows ~= 100 * x ms 

*/
#define SERVO_EXCERCISE_RESOLUTION_DISPLAY	25		// ~1 ms * 25 =~ 25 ms, maximum ~ 80 ms
#define SERVO_EXCERCISE_RESOLUTION	(SERVO_EXCERCISE_RESOLUTION_DISPLAY * CLOCK_MULTIPLIER)		// ~1 ms * 25 =~ 25 ms, maximum ~ 80 ms

#define SERVO_EXCERCISE_MAXIMUM_DELAY	200	// total delay = SERVO_EXCERCISE_RESOLUTION * SERVO_EXCERCISE_MAXIMUM_DELAY ms
#define SERVO_EXCERCISE_DEFAULT_DELAY	10	// 
#define SERVO_EXCERCISE_MINIMUM_DELAY	1	// 


#define SWITCH_UP_ID		0
#define SWITCH_UP_PORT		PORTB
#define SWITCH_UP_PIN		PINB
#define SWITCH_UP_DIR		DDRB
#define SWITCH_UP_BIT		4
#define SWITCH_UP_PULLUP	1

// PB0: RX input (ICP1)

#define SWITCH_DOWN_ID		1
#define SWITCH_DOWN_PORT	PORTB
#define SWITCH_DOWN_PIN		PINB
#define SWITCH_DOWN_DIR		DDRB
#define SWITCH_DOWN_BIT		3
#define SWITCH_DOWN_PULLUP	1

// PB2: PWM output (OC1B)

#define SWITCH_LEFT_ID		2
#define SWITCH_LEFT_PORT	PORTB
#define SWITCH_LEFT_PIN		PINB
#define SWITCH_LEFT_DIR		DDRB
#define SWITCH_LEFT_BIT		5
#define SWITCH_LEFT_PULLUP	1

#define SWITCH_CENTER_ID	3
#define SWITCH_CENTER_PORT	PORTC
#define SWITCH_CENTER_PIN	PINC
#define SWITCH_CENTER_DIR	DDRC
#define SWITCH_CENTER_BIT	4
#define SWITCH_CENTER_PULLUP	1

#define SWITCH_RIGHT_ID		4
#define SWITCH_RIGHT_PORT	PORTC
#define SWITCH_RIGHT_PIN	PINC
#define SWITCH_RIGHT_DIR	DDRC
#define SWITCH_RIGHT_BIT	5
#define SWITCH_RIGHT_PULLUP	1

#define SWITCH_SOURCE_ID	5
#define SWITCH_SOURCE_PORT	PORTB
#define SWITCH_SOURCE_PIN	PINB
#define SWITCH_SOURCE_DIR	DDRB
#define SWITCH_SOURCE_BIT	1
#define SWITCH_SOURCE_PULLUP	1

#define ERROR_LED_DIR		DDRD
#define ERROR_LED_PORT		PORTD
#define ERROR_LED_BIT		7


#endif


