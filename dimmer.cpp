/*
 * dimmer.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: compi
 */
#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "PushButton.h"

//////////////////////////////////////////////////////////////////////////////
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

//////////////////////////////////////////////////////////////////////////////

#define TRIAC_FIRE  PB0;

bool 		g_down = true;
uint8_t		g_dim = 0;
uint8_t		g_prev = 128;
uint16_t	g_now = 0;
uint8_t		230_dim = 0;

PushButton				g_buttons[2];
static const uint8_t	g_buttonbits[2] = {PINB0, PINB3};
static const uint8_t	g_dimValues[] = {0, 1, 13, 26, 51, 84, 128, 192, 255};

//////////////////////////////////////////////////////////////////////////////
void Init()
{
	DDRB = _BV(DDB2); // LED PWM set to Output

	//timer clock = F_CPU / 64 =  250000Hz
	TCCR1 = _BV(CS12) | _BV(CS11) | _BV(CS10); // ?
	GTCCR = _BV(PWM1B) |_BV(COM1B1);  // ?
	OCR1C = 0xff;
	OCR1B = g_dim;
	TIMSK = _BV(TOV1);	//256000 / 256 = 1KHz
	
	//Init Zero Cross Timer
	
	//Timer Clock = F_CPU / 1024 = 15625 Hz
	TCCR2 =  _BV(CS12) | _BV(CS10);

	for(uint8_t button = 0; button < COUNTOF(g_buttons); ++button)
		g_buttons[button].Init(PINB, g_buttonbits[button], 10, 100, 150);

}

void InitInterrupt(){
	
	EIMSK |= _BV(INT0); // Enable INT0
	EICRA |= (1 << ISC01); // Set INT0 to falling edge
	
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
	InitInterrupt();
	while(true);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_OVF_vect)
{
	static uint8_t divider = 0;
	++g_now;

	for(uint8_t button = 0; button < COUNTOF(g_buttons); ++button) {
		g_buttons[button].Tick(g_now);
	}

	switch(g_buttons[0].GetState())		//	up
	{
		case PushButton::active:
			if(!(divider & 15) && g_dim < 0xff)
				++g_dim;
			break;
		
		case PushButton::clicked:
			SetNext(true);
			break;
			
		case PushButton::doubleclicked:
			if(!g_dim && g_prev) {
				g_dim = g_prev;
			} else {
				g_prev = g_dim;
				g_dim = 0xff;
			}
			break;
			
		case PushButton::inactive:
			break;
	}
	
	switch(g_buttons[1].GetState())		//	down
	{
		case PushButton::active:
			if(!(divider & 7) && g_dim > 0)
				--g_dim;
			break;
		
		case PushButton::clicked:
			SetNext(false);
			break;
		
		case PushButton::doubleclicked:
			if(g_dim == 255 && g_prev != 255) {
				g_dim = g_prev;
			} else {
				g_prev = g_dim;
				g_dim = 0;
			}
			break;
		
		case PushButton::inactive:
			break;
	}
	
	
	++divider;

	if(OCR1B < g_dim)
		++OCR1B;
	else if(OCR1B > g_dim)
		--OCR1B;

}


ISR (INT0_vect) {
	
	
	
}
