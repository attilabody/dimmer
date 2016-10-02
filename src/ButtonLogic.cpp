/*
* DimmerButton.cpp
*
* Created: 9/3/2016 1:44:59 PM
* Author: compi
*/

#include "ButtonLogic.h"

//////////////////////////////////////////////////////////////////////////////
uint8_t ButtonLogic::Tick(bool inputActive, uint16_t now)
{
	if(inputActive != m_lastInputActive) {
		m_lastInputActive = inputActive;
		if(!inputActive) {					//release
			if(m_state == 255) {			//pushed
				m_state = 0;
			} else {
				++m_releaseCount;
			}
		}
		m_startTick = now;
	}

	uint16_t	ellapsed = now - m_startTick;

	if(inputActive) {
		if((m_dimDelay != 0 || m_clickDelay == 0) && ellapsed >= m_dimDelay) {
			m_state = 255;
			m_releaseCount = 0;
		}
	} else {
		if(ellapsed >= m_clickDelay) {
			if(m_releaseCount != 0) {
				m_state = m_releaseCount;
				m_startTick = now;
				m_releaseCount = 0;
			}
		}
	}

	return m_state;
}

//////////////////////////////////////////////////////////////////////////////
void ButtonLogic::Reset()
{
	m_state = 0;
	m_lastInputActive = 0;
	m_releaseCount = 0;
}

//////////////////////////////////////////////////////////////////////////////
void ButtonLogic::Init(
    uint16_t clickDelay,
    uint16_t dimDelay
)
{
	m_clickDelay = clickDelay;
	m_dimDelay = dimDelay;
	Reset();
}

//////////////////////////////////////////////////////////////////////////////
uint8_t ButtonLogic::GetState()
{
	uint8_t	ret(m_state);
	if(m_state > 0 && m_state < 255) {
		m_state = 0;
	}
	return ret;
}

