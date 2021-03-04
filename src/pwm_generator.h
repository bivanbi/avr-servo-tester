#ifndef SERVO_PWM_HEADER_LOADED
#define SERVO_PWM_HEADER_LOADED


// The Timer/Counter1 Control Register B- TCCR1B Register is as follows.


/*

PWM_fequency = clock_speed / [Prescaller_value * (1 + TOP_Value) ]



TCNT1 Register
The Timer/Counter1 - TCNT1 Register is as follows. It is 16 bits wide since the TIMER1 is a 16-bit register. TCNT1H represents the HIGH byte whereas TCNT1L represents the LOW byte. The timer/counter value is stored in these bytes.

TCNT1, OCR1A/B, and ICR1


Writing the OCR1x Registers must be done via the TEMP
Register since the compare of all 16 bits is done continuously. The high byte (OCR1xH) has to be written first.
When the high byte I/O location is written by the CPU, the TEMP Register will be updated by the value written.
Then when the low byte (OCR1xL) is written to the lower eight bits, the high byte will be copied into the upper 8-
bits of either the OCR1x buffer or OCR1x Compare Register in the same system clock cycle.

The fast Pulse Width Modulation or fast PWM mode (WGM13:0 = 5, 6, 7, 14, or 15) provides a high frequency


In fast PWM mode the counter is incremented until the counter value matches either one of the fixed values
0x00FF, 0x01FF, or 0x03FF (WGM13:0 = 5, 6, or 7), the value in ICR1 (WGM13:0 = 14), or the value in OCR1A
(WGM13:0 = 15). The counter is then cleared at the following timer clock cycle. The timing diagram for the fast
PWM mode is shown in Figure 16-7. The figure shows fast PWM mode when OCR1A or ICR1 is used to define
TOP. The TCNT1 value is in the timing diagram shown as a histogram for illustrating the single-slope operation.
The diagram includes non-inverted and inverted PWM outputs. The small horizontal line marks on the TCNT1
slopes represent compare matches between OCR1x and TCNT1. The OC1x Interrupt Flag will be set when a
compare match occurs.



*/


/*
 initialize TCCR0 as per requirement, say as follows
    TCCR1 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00);
*/

//#ifndef SWITCH_UP_PULLUP
//#error "SWITCH_UP_PULLUP not defined"
//#endif


inline void pwm_generator_enable(void);
inline void pwm_generator_disable(void);

#endif
