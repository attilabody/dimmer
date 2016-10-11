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

#include "../SmartButton.h"

//////////////////////////////////////////////////////////////////////////////
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

//////////////////////////////////////////////////////////////////////////////
IOPort	g_port(&PINB, &PORTB, &DDRB, (1 << DDB4), ~(1 << PORTB4));

bool 		g_down = true;
uint8_t		g_dim = 0;
uint8_t		g_prev = 128;
uint16_t	g_now = 0;

SmartButton				g_buttons[2] ;
static const uint8_t	g_buttonbits[2] = {PINB1, PINB3};
static const uint8_t	g_dimValues[] = {0, 1, 13, 26, 51, 84, 128, 192, 255};

//////////////////////////////////////////////////////////////////////////////
void Init()
{
	DDRB = _BV(DDB4);

	//timer clock = F_CPU / 64 =  125000Hz
	TCCR1 = _BV(CS12) | _BV(CS11) | _BV(CS10);
	GTCCR = _BV(PWM1B) |_BV(COM1B1);
	OCR1C = 0xff;
	OCR1B = g_dim;
	TIMSK = _BV(TOV1);	//125000 / 256 = 488.28125Hz

	for(uint8_t button = 0; button < COUNTOF(g_buttons); ++button) {
		g_buttons[button].Init(10, &g_port, g_buttonbits[button], true, 144, 488);
	}
	sei();
}

//////////////////////////////////////////////////////////////////////////////
void SetNext(bool up)
{
	uint8_t	index;

	if(up) {
		index = 0;
		while(index < COUNTOF(g_dimValues) - 1 && g_dimValues[index] <= g_dim)
			++index;
	} else {
		index = COUNTOF(g_dimValues) - 1;
		while(index && g_dimValues[index] >= g_dim)
			--index;
	}
	g_dim = g_dimValues[index];
}

//////////////////////////////////////////////////////////////////////////////
int main()
{
	Init();
	while(true);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_OVF_vect)
{
	static uint8_t divider = 0;
	uint8_t	states[2] {0, 0};//COUNTOF(g_buttons)];

	++g_now;

	for(uint8_t button = 0; button < COUNTOF(g_buttons); ++button) {
		states[button] = g_buttons[button].Tick(g_now);
	}

	switch(states[0])		//	up
	{
		case 0:
			break;
		
		case 1:
			SetNext(true);
			break;
			
		case 2:
			if(!g_dim && g_prev) {
				g_dim = g_prev;
			} else {
				g_prev = g_dim;
				g_dim = 0xff;
			}
			break;

		case 255:
			if(!(divider & 15) && g_dim < 0xff)
				++g_dim;
			break;
	}
	
	switch(states[1])		//	down
	{
	case 0:
		break;

	case 1:
		SetNext(false);
		break;
		
	case 2:
		if(g_dim == 255 && g_prev != 255) {
			g_dim = g_prev;
		} else {
			g_prev = g_dim;
			g_dim = 0;
		}
		break;
		
	case 255:
		if(!(divider & 7) && g_dim > 0)
			--g_dim;
		break;
	}
	
	++divider;

	if(OCR1B < g_dim)
		++OCR1B;
	else if(OCR1B > g_dim)
		--OCR1B;

}
