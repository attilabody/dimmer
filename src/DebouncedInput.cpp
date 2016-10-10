/*
 * DebouncedInput.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: compi
 */

#include "DebouncedInput.h"

//////////////////////////////////////////////////////////////////////////////
void DebouncedInput::Init(uint16_t debounceDelay, IOPort* port, uint8_t bit, bool activeLow)
{
	m_debounceDelay = debounceDelay;
	m_port = port;
	m_bit = bit;
	m_activeLow = activeLow;
}

//////////////////////////////////////////////////////////////////////////////
bool DebouncedInput::Tick(uint16_t now)
{
	bool inputState = (m_port->Read(m_bit) != m_activeLow);

	if(inputState == m_debouncedState) {
		m_inputStateEqualTick = now;
	} else if(now - m_inputStateEqualTick >= m_debounceDelay) {	//debouncing delay ellapsed
		m_debouncedState = inputState;
	}
	return m_debouncedState;
}
