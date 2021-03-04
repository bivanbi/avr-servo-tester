#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"



void adc_init(void){

    // set up frequency divider: ADC clock must be between 50kHz to 200kHz.

#ifdef ADC_CLOCK_DIV_BIT_2_HIGH
    ADCSRA |= (1<<ADPS2);
#endif
#ifdef ADC_CLOCK_DIV_BIT_1_HIGH
    ADCSRA |= (1<<ADPS1);
#endif
#ifdef ADC_CLOCK_DIV_BIT_0_HIGH
    ADCSRA |= (1<<ADPS0);
#endif


    // arrange 10 bit result to the left
#ifdef ADC_10BIT_RESULT_ARRANGE_LEFT
    ADMUX |= (1<<ADLAR);
#endif


    // voltage reference
#ifdef ADC_VOLTAGE_REFERENCE_AVCC_WITH_CAPACITOR_ON_AREF
    ADMUX |= (1<<REFS0);
#endif

    // disable digital input buffer for ADC pin
    DIDR0 |=  (1<<ADC_PORT_PIN);
}


inline void adc_enable(void)
{
    ADCSRA |= (1<<ADEN);
}

inline void adc_disable(void)
{
    ADCSRA &= ~(1<<ADEN);
}

inline void adc_start_conversion(void)
{
    ADCSRA |= (1<<ADSC);
}


inline void adc_set_input_adc0(void)
{
    ADMUX &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));
}

inline void adc_set_input_1_1v(void)
{
// if 10bit arrange left, we have to set ADLAR bit
#ifdef ADC_10BIT_RESULT_ARRANGE_LEFT

#ifdef ADC_VOLTAGE_REFERENCE_AVCC_WITH_CAPACITOR_ON_AREF
    // if AVCC is set to reference, we have to set REFS0 too
    ADMUX = (1<<ADLAR)|(1<<REFS0)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1);
#else
    ADMUX = (1<<ADLAR)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1);
#endif

#else

#ifdef ADC_VOLTAGE_REFERENCE_AVCC_WITH_CAPACITOR_ON_AREF
    // if AVCC is set to reference, we have to set REFS0 too
    ADMUX = (1<<REFS0)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1);
#else
    ADMUX = (1<<MUX3)|(1<<MUX2)|(1<<MUX1);
#endif



#endif
}

