#ifndef MAIN_HEADER_LOADED
#define MAIN_HEADER_LOADED

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

#endif

typedef void (*FuncPtr)(void);
//function pointer
//FuncPtr FPtr;


#define LINE_2	LCD_ADDRESS_SECOND_LINE
#define MOVECRS	LCD_COMMAND_MOVE_CURSOR

/*
typedef struct PROGMEM
{
    uint8_t position;
    char    *text;
} addrstr;
*/


/*
 *
 * we have 2048 bytes of EEPROM
 * 
 *
 */

typedef struct			
{
    uint8_t	id;										//1
    char	name[16];									// 17
    uint8_t	signal_type;		// reserved for future use, now its 0: legacy_pwm	// 18
    uint16_t	pwm_center;									//20
    uint16_t	pwm_range;									//22
    uint16_t	pwm_freq;									//24
} servoAttribStruct;

typedef struct			
{
    uint16_t	configVersion;									//2
    uint16_t	voltageCorrection;								//4
} testerConfigStruct;



// reserved for future global configuration

#define	SERVO_MEMORY_SLOTS  	  20

#define STRING_WELCOME			0
#define STRING_FWVER			1
#define STRING_RELASE_BUTTONS		2
#define STRING_LIVE_DATA		3
#define STRING_SERVO_LOAD		4
#define STRING_SETTINGS_PWM_CENTER	5
#define STRING_SETTINGS_PWM_RANGE	6
#define STRING_SETTINGS_PWM_FREQ	7
#define STRING_SETTINGS_SAVE		8
#define STRING_SETTINGS_SERVO_NAME	9
#define STRING_ERROR_EEPROM_LOAD_FAIL	10
#define STRING_SERVO_LOADED		11
#define STRING_SERVO_SAVED		12
#define STRING_SETTINGS_PWM_PERIOD	13
#define STRING_SETTINGS_INVALID		14
#define STRING_RX_INPUT_INVALID		15
#define STRING_EXCERCISE_SERVO		16
#define STRING_EXCERCISE_SERVO_LINE2	17
#define STRING_VOLTMETER_CALIBRATE	18

addrstr mystring[] = {
	{ 0,		"KiGN ServoTester" },
	{ LINE_2,	"Firmware v"FIRMWARE_VERSION },
	{ LINE_2,	"Release buttons!" },
	// default view: live PWM data
	//               F999*PW9999*100%
	//  index        0  3   7   b   f
	{ 0,		"    V     us   %" },
	// this line need not to be predefined      
        //{ LINE_2,		"*9999*9999*9999*" }

	//  index        0  3   7   b   f
	{ 0,		"Load Servo:"		},
	{ 0,		"Center:     us"	},
	{ 0,		"Range: +-     us"	},
	{ 0,		"Frequency:    Hz"	},
	//  index        0  3   7   b   f
	{ 0,		"Save to slot:"		},
	{ 0,		"Servo Name: "		},
	{ LINE_2,	"ERR InvalidEPROM"	},
	{ 0,		"Loaded servo:"		},
	{ 0,		"Saved servo: "		},
	{ LINE_2,	"Period:      us"	},
        { LINE_2,       "Invalid settings"	},
        { LINE_2,       "Invalid RX input"	},

	//  index        0  3   7   b   f
	{ 0,		"PE     V -     V"	},
	{ LINE_2,	"     ms   %-   %"	},
	{ 0,		"Voltmeter Calib."	}
};

#define SERVO_SETTINGS_LOAD_SERVO_ID_POSTION		13
#define SERVO_SETTINGS_SAVE_SERVO_ID_POSTION		14

#define SERVO_SETTINGS_PWM_CENTER_INTEGER_POSITION	8
#define SERVO_SETTINGS_PWM_RANGE_INTEGER_POSITION	10
#define SERVO_SETTINGS_PWM_FREQ_INTEGER_POSITION	11
#define SERVO_SETTINGS_PWM_PERIOD_INTEGER_POSITION	8

#define SERVO_SETTINGS_NAME_SELECTED_CHAR_POSITION	15

#define VOLTMETER_CALIBRATE_INTEGER_POSITION		(LINE_2)

#define SERVO_LIVE_DATA_VOLTAGE_INTEGER_POSITION	0
#define SERVO_LIVE_DATA_PWM_FREQ_INTEGER_POSITION	0
#define SERVO_LIVE_DATA_CURRENT_PWM_INTEGER_POSITION	6
#define SERVO_LIVE_DATA_CURRENT_PWM_PERCENTAGE_POSITION	12
#define SERVO_LIVE_DATA_PWM_MIN_CENTER_MAX_POSITION	LINE_2

#define SERVO_EXCERCISE_LOWEST_VOLTAGE_POSITION		3
#define SERVO_EXCERCISE_HIGHEST_VOLTAGE_POSITION	11
#define SERVO_EXCERCISE_DELAY_POSITION	 		(LINE_2)
#define SERVO_EXCERCISE_PERCENT_MIN		        (LINE_2|7)
#define SERVO_EXCERCISE_PERCENT_MAX		        (LINE_2|12)

typedef struct			
{
    uint16_t	adcResult;									//2
    uint16_t	pwm;										//2
    uint16_t	pwm_min;									//4
    uint16_t	pwm_center;									//4
    uint16_t	pwm_range;									//6
    uint16_t	pwm_max;									//4
    uint16_t	pwm_freq;									//8
    uint16_t	pwm_period;									//8
} PWMStruct;

PWMStruct	PWMAttrib	= { 512, 0, 0, 0, 0, 0, 0, 0 };

uint8_t		loop_count	= 0;
uint8_t		theLowADC	= 0;
uint16_t	adcResult	= 0;
uint16_t	adcPowerSupplyResult	= 0;
uint8_t		adcSampleCount	= 0;

uint16_t	adcPowerSupplyResult_min	= 0;
uint16_t	adcPowerSupplyResult_max	= 0;

//uint16_t	voltageCorrection	= 512;
double		voltageCalculated	= 0;
double		voltageCalculatedAvg	= 0;

double		refVoltageCorrected; // refVoltageCorrected * 1023

uint8_t		menu_servo_id			= 0;
uint8_t		edit_name_cursor_position	= 0;
uint8_t		edit_name_current_char_index	= 0;

uint16_t	servoExcerciseDelay	= SERVO_EXCERCISE_DEFAULT_DELAY;
uint16_t	servoExcercisePosition	= 512;
uint16_t	servoExcerciseTimer	= 0;

servoAttribStruct servoAttrib;
testerConfigStruct testerConfig = {CONFIG_VERSION, VOLTMETER_DEFAULT_CORRECTION };

char servoNameCharSet[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

// this will hold the (generated) string to display
char PrintString[17];
/*
 *
 * this is "where we are", the current menu item if you will.
 * this defines what gets displayed on screen and how
 * program behaves upon key press.
 *
 */
uint8_t current_function = 0;


/*
 *
 *  program states
 *
 */
#define FLAG_ADC_ENABLED		0
#define FLAG_RX_PASSTHROUGH_INITIATED	1
#define FLAG_PWM_GENERATOR_INITIATED	2
//#define VIEW_CHANGED			3
#define MULTIPLE_BUTTONS_PRESSED	4
// if we are in servo name screen, disable adc to pwm so servo would not follow
// our character input
#define FLAG_ADC_TO_PWM_ENABLED		5

#define FLAG_PWM_ATTRIB_INVALID		6
// 0: input is adc0 (potmeter), 1: input is 1.1v band gap to measure power supply voltage
#define FLAG_ADC_INPUT			7

// default flags: VIEW_CHANGED & FLAG_ADC_TO_PWM_ENABLED
uint8_t global_flag	= 0b01101000;

#define FLAG2_SERVO_EXCERCISE_ENABLED		0
#define FLAG2_SERVO_EXCERCISE_POSITION		1
// 0 = drive servo to minimum PWM, 1 = drive servo to maximum PWM
uint8_t global_flag2	= 0b00000000;


// temp flag will be reset every time menu changes
uint8_t menu_local_flag = 0;

uint8_t pwm_frequency = 20;

// uint8_t states = 0;

void startup_check_for_stuck_buttons(void);
void superdelay(void);
void adc_get_result(void);
int main(void);
void handle_menu_button(void);
void menu_change_general_todo(void);
inline void clearscreen(void);
void debug(uint8_t step);
void load_servo_data(uint8_t servo_id);
void save_servo_data(uint8_t servo_id);

void validate_servo_data(void);

void validate_servo_data(void);

void servo_excercise_timer_start(void);
void servo_excercise_timer_stop(void);

void print_settings_servo_id_and_name(uint8_t id_position);

void load_settings(void);
void save_settings(void);

inline void print_servo_attrib_pwm_min_center_max(void);
inline void print_servo_attrib_pwm_center(void);
inline void print_servo_attrib_pwm_range(void);
inline void print_servo_attrib_pwm_frequency(void);
inline void print_servo_attrib_pwm_period(void);




#define MENU_MAX     8
void init_live_data(void);
void init_excercise_servo(void);
void init_settings_servo_name(void);
void init_settings_pwm_center(void);
void init_settings_pwm_range(void);
void init_settings_pwm_freq(void);
void init_servo_save(void);
void init_settings_voltmeter_calibrate(void);
void init_servo_load(void);

void (*menu_init_handlers[])(void) = 
{
    init_live_data,
    init_excercise_servo,
    init_settings_pwm_center,
    init_settings_pwm_range,
    init_settings_pwm_freq,
    init_settings_servo_name,
    init_servo_save,
    init_settings_voltmeter_calibrate,
    init_servo_load,
};




void screen_live_data(void);
void screen_excercise_servo(void);
void screen_settings_pwm_center(void);
void screen_settings_pwm_range(void);
void screen_settings_pwm_freq(void);
void screen_settings_servo_name(void);
void screen_servo_save(void);
void screen_settings_voltmeter_calibrate(void);
void screen_servo_load(void);

void (*menu_handlers[])(void) = 
{
    screen_live_data,
    screen_excercise_servo,
    screen_settings_pwm_center,
    screen_settings_pwm_range,
    screen_settings_pwm_freq,
    screen_settings_servo_name,
    screen_servo_save,
    screen_settings_voltmeter_calibrate,
    screen_servo_load,
};

void exit_live_data(void);
void exit_excercise_servo(void);
void exit_settings_servo_name(void);
void exit_settings_pwm_center(void);
void exit_settings_pwm_range(void);
void exit_settings_pwm_freq(void);
void exit_servo_save(void);
void exit_settings_voltmeter_calibrate(void);
void exit_servo_load(void);

void (*menu_exit_handlers[])(void) = 
{
    exit_live_data,
    exit_excercise_servo,
    exit_settings_pwm_center,
    exit_settings_pwm_range,
    exit_settings_pwm_freq,
    exit_settings_servo_name,
    exit_servo_save,
    exit_settings_voltmeter_calibrate,
    exit_servo_load,
};


void do_generate_pwm(void);
void do_rx_passthrough(void);

void (*pwm_source_handlers[])(void) = 
{
    do_generate_pwm,
    do_rx_passthrough,
};


void exit_generate_pwm(void);
void exit_rx_passthrough(void);

void (*pwm_source_exit_handlers[])(void) = 
{
    exit_rx_passthrough,
    exit_generate_pwm,
};

