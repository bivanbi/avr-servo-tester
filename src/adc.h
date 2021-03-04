/*


ADC clock must be between 50 - 200khz.

When initiating a single ended conversion by setting the ADSC bit in ADCSRA, the conversion starts at the
following rising edge of the ADC clock cycle.
A normal conversion takes 13 ADC clock cycles. The first conversion after the ADC is switched on (ADEN in
ADCSRA is set) takes 25 ADC clock cycles in order to initialize the analog circuitry.

When a conversion is complete, the result is written to the
ADC Data Registers, and ADIF is set. In Single Conversion mode, ADSC is cleared simultaneously. The
software may then set ADSC again, and a new conversion will be initiated on the first rising ADC clock edge.

ADCL and ADCH - data register

**********************************
ADCSRA: ADC control and status register A
**********************************

BITS:
ADPSx    divide
 2 1 0
 0 0 0 = 2
 0 0 1 = 2
 0 1 0 = 4
 0 1 1 = 8   - 1 Mhz cpu clock =  125.000 khz ADC clock
 1 0 0 = 16
 1 0 1 = 32
 1 1 0 = 64  - 10 Mhz CPU clock = 156.250 khz ADC clock
 1 1 1 = 128 - 10 Mhz cpu clock =  78.125 khz ADC clock
#

ADIE = ADC interrupt enable

ADEN = ADC enable ADSC will read as one as long as a conversion is in progress.
	When the conversion is complete, it returns to zero. Writing zero to this bit has no effect.

ADSC = ADC Start Conversion

ADATE = ADC Auto Trigger Enable


**********************************
ADCSRB: ADC control and status register B
**********************************
ADTS 2 .. 0: Auto Trigger Source

**********************************
DIDR0 - Digital Input Disable Register 0
**********************************
ADC5D ... ADC0D: ADCx pin disable


**********************************
ADMUX regixter:
**********************************

10 bit result mode:
ADLAR 0: ACDH bit 0-1 + ADCL
ADLAR 1: ACDH         + ACDL bit 6-7


REFS1 REFS0 Voltage Reference Selection
0     0	    AREF, Internal V ref turned off
0     1     AVCC with external capacitor at AREF pin
1     0     Reserved
1     1     Internal 1.1V Voltage Reference with external capacitor at AREF pin


*/

#ifndef ADC_HEADER_LOADED
#define ADC_HEADER_LOADED

/* should be setup in config.h
	#define ADC_CLOCK_DIV_BIT_2_HIGH
	#define ADC_CLOCK_DIV_BIT_1_HIGH
	#define ADC_CLOCK_DIV_BIT_0_HIGH

	#define ADC_10BIT_RESULT_ARRANGE_LEFT

	#define ADC_PORT	PORTC
	#define ADC_PORT_PIN	PC0

	#define ADC_VOLTAGE_REFERENCE_EXTERNAL_ON_AREF
	#define ADC_VOLTAGE_REFERENCE_AVCC_WITH_CAPACITOR_ON_AREF

*/

#ifndef ADC_PORT
#define ADC_PORT PORTC
#endif

#ifndef ADC_PORT_PIN
#define ADC_PORT_PIN PC0
#endif


void adc_init(void);

inline void adc_enable(void);
inline void adc_disable(void);
inline void adc_start_conversion(void);
//void adc_trigger(void);//
/*  adc_fetch(void); */

inline void adc_set_input_adc0(void);
inline void adc_set_input_1_1v(void);

#endif
