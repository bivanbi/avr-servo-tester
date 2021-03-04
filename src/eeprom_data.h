// this contains default values for eeprom.

#ifndef EEPROM_HEADER_LOADED
#define EEPROM_HEADER_LOADED

#include <avr/eeprom.h>

testerConfigStruct EEMEM nv_testerConfig = {
    .configVersion		= CONFIG_VERSION,       // 
    .voltageCorrection		= 512          		// 0 - 1023
};

servoAttribStruct EEMEM nv_servoAttrib[] = {
    {
	.id                 = 1,            // id
	.name               = "Servo01         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 2,            // id
	.name               = "Servo02         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 3,            // id
	.name               = "Servo03         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 4,            // id
	.name               = "Servo04         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 5,            // id
	.name               = "Servo05         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 6,            // id
	.name               = "Servo06         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 7,            // id
	.name               = "Servo07         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 8,            // id
	.name               = "Servo08         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 9,            // id
	.name               = "Servo09         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 10,            // id
	.name               = "Servo10         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 11,            // id
	.name               = "Servo11         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 12,            // id
	.name               = "Servo12         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 13,            // id
	.name               = "Servo13         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 14,            // id
	.name               = "Servo14         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 15,            // id
	.name               = "Servo15         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 16,            // id
	.name               = "Servo16         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 17,            // id
	.name               = "Servo17         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 18,            // id
	.name               = "Servo18         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 19,            // id
	.name               = "Servo19         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

    {
	.id                 = 20,            // id
	.name               = "Servo20         ",     // name
	.signal_type        = 0,            // signal_type;         // reserved for future use, now its 0: legacy_pwm
	.pwm_center         = 1520,         // pwm_center;
	.pwm_range          = 400,          // pwm_range;
	.pwm_freq           = 50            // pwm_freq;
    },

};


#endif

