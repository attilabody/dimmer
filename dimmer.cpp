/*
 * dimmer.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: compi
 */
#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

bool 	g_down = true;
uint8_t	g_pwm = 255;

void Init()
{
	DDRB = _BV(DDB4);
	PORTB = _BV(PORTB3) | _BV(PORTB2) | _BV(PORTB1) | _BV(PORTB0);

	//timer clock = F_CPU / 64 =  125000Hz
	TCCR1 = _BV(CS12) | _BV(CS11) | _BV(CS10);
	GTCCR = _BV(PWM1B) |_BV(COM1B1);
	OCR1C = 0xff;
	OCR1B = g_pwm;
	TIMSK = _BV(TOV1);	//125000 / 256 = 488.28125Hz
	sei();
}

int main()
{
	Init();
	while(true)
	{
		if((PORTB & _BV(PORTB3)) == 0) g_pwm = 0;
		else if((PORTB & _BV(PORTB2)) == 0) g_pwm = 85;
		else if((PORTB & _BV(PORTB1)) == 0) g_pwm = 170;
		else if((PORTB & _BV(PORTB0)) == 0) g_pwm = 255;
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	if(OCR1B < g_pwm) ++OCR1B;
	else if(OCR1B > g_pwm) --OCR1B;
}
