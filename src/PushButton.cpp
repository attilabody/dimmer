/*
 * PushButton.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: compi
 */

#include "PushButton.h"
#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////////
PushButton::PushButton()
: m_port(nullptr)
, m_bit(0)
, m_debouncedState(false)
, m_inputStateEqualTick(0)
, m_debounceDelay(0)
, m_clickDelay(0)
, m_lastPushedTick(0)
, m_state(inactive)
{
}

//////////////////////////////////////////////////////////////////////////////
void PushButton::Init(volatile uint8_t &port, uint8_t bit, uint8_t debounceDelay, uint8_t clickDelay, uint8_t doubleclickDelay)
{
	m_port = &port;
	m_bit = bit;
	m_debounceDelay = debounceDelay;
	m_clickDelay = clickDelay;
	m_doubleclickDelay = doubleclickDelay;
	*m_port = _BV(m_bit);
}

//////////////////////////////////////////////////////////////////////////////
void PushButton::Tick(uint16_t now)
{
	bool inputState = !(bool)(*m_port & _BV(m_bit));
	bool changed = false;
	if(inputState == m_debouncedState) {
		m_inputStateEqualTick = now;
	} else if(now - m_inputStateEqualTick >= m_debounceDelay)
	{	//debouncing delay ellapsed
		m_debouncedState = inputState;
		changed = true;
		if(inputState) {
			m_lastPushedTick = now;
			m_clickAwaited = true;
		}
	}
	
	if(m_state != clicked && m_state != doubleclicked) {	//don't lose clicks
		if(m_debouncedState) {	//button active
			if(!m_clickAwaited)
				m_state = active;
		} else
			m_state = inactive;
	}
	if(changed && !inputState)		//button released
	{
		if(m_clickAwaited) {		//clicked
			if(m_doubleclickAwaited) {	//doubleclicked
				m_state = doubleclicked;
				m_clickAwaited = m_doubleclickAwaited = false;
			} else { //click state will be set when doubleclickAwaited expires
				m_lastClickedTick = now;
				m_clickAwaited = false;
				m_doubleclickAwaited = true;
			}
		}
	}
	
	if(m_clickAwaited && now - m_lastPushedTick >= m_clickDelay)
		m_clickAwaited = false;
		
	if(m_doubleclickAwaited && now - m_lastClickedTick >= m_doubleclickDelay) {
		m_doubleclickAwaited = false;
		m_state = clicked;
	}
}

//////////////////////////////////////////////////////////////////////////////
PushButton::STATE PushButton::GetState()
{
	STATE	ret(m_state);
	if(m_state == clicked || m_state == doubleclicked) {
		m_state = inactive;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////////
bool PushButton::IsClicked()
{
	if(m_state == clicked) {
		m_state = inactive;
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
bool PushButton::IsDoubleClicked()
{
	if(m_state == doubleclicked) {
		m_state = inactive;
		return true;
	}
	return false;
}
