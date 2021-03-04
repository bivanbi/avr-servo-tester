#include "config.h"

#include <avr/io.h>
// #include <util/delay.h>

#include "pwm_generator.h"


/*
 initialize TCCR0 as per requirement, say as follows
    TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00);
*/

inline void pwm_generator_enable(void)
{


    /*
TCCR1A  

        7 COM1A1
        6 COM1A0
        5 COM1B1
        4 COM1B0
        -
        -
        1 WGM11 Waveform Generation Mode bit 1
        0 WGM10 Waveform Generation Mode bit 0

TCCR1B
        7 ICNC1 Input Capture Noice Canceler
        6 ICES1 Input Capture Edge Select: 0 - falling edge, 1: rising edge
        -
        4 WGM13 Waveform Generation Mode bit 3
        3 WGM12 Waveform Generation Mode bit 2
        2 CS12  Clock Select bit 2
        1 CS11  Clock Select bit 1
        0 CS10  Clock Select bit 0

COM1x bits for Fast PWM:
        1 0     clear OC1A/OC1B on Compare Match, set at Bottom (non-inverting)

CS bits 2 1 0
        0 0 0   clock disabled - counter stopped
        0 0 1   clk/1
        0 1 0   clk/8
        0 1 1   clk/64
        1 0 0   clk/256
        1 0 1   clk/1024

WGM bits for fast PWM:
         1 1 1 0        Fast PWM, TOP: ICR1	ICR1 is NOT double buffered, there need to be an
						interrupt handler if it needs to be changed. But it leaves
						IC1A pin available for PWM output

         1 1 1 1        Fast PWM, TOP: OCR1A	OCR1A is double-buffered, it can be changed anytime,
						however, when OCR1A is used as TOP value, IC1A pin
						cannot be used as output

    */


    TCCR1A	 = ((1<<COM1B1)|(1<<WGM11)|(1<<WGM10));
    TCCR1B	 = ((1<<WGM13) |(1<<WGM12)|(1<<CS10));
    DDRB	|= (1<<PB2);
    /*

The PRTIM1 bit in ”PRR – Power Reduction Register” on page 45 must be written
 to zero to enable Timer/Counter1 module.

Bit 3 – PRTIM1: Power Reduction Timer/Counter1
Writing a logic one to this bit shuts down the Timer/Counter1 module. When the Timer/Counter1 is enabled,
operation will continue like before the shutdown.

    since PRR is initially zero, we need not do anything here
    PRR &= ~(1<<PRTIM1);
    */

}


inline void pwm_generator_disable(void)
{
    TCCR1B = 0;
    TCCR1A = 0;
}



