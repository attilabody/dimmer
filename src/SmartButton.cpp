/*
 * SmartButton.cpp
 *
 *  Created on: Oct 3, 2016
 *      Author: compi
 */

#include <SmartButton.h>

void SmartButton::Init(uint16_t debounceDelay, IOPort* port, uint8_t bit, bool activeLow, uint16_t clickDelay, uint16_t dimDelay)
{
	m_input.Init(debounceDelay, port, bit, activeLow);
	m_logic.Init(clickDelay, dimDelay);
}
