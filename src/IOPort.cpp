/*
* IOPort.cpp
*
* Created: 9/3/2016 6:56:08 PM
* Author: compi
*/


#include "IOPort.h"

//////////////////////////////////////////////////////////////////////////////
IOPort::IOPort(
    volatile uint8_t *inReg,
    volatile uint8_t *outReg,
    volatile uint8_t *ddReg,
	uint8_t ddMask,
	uint8_t	outValues
)
	: m_inReg(inReg)
	, m_outReg(outReg)
	, m_ddReg(ddReg)
{
	*m_ddReg = ddMask;
	*m_outReg = outValues;
}

//////////////////////////////////////////////////////////////////////////////
void IOPort::ConfigurePin(uint8_t bit, bool direction, bool value)
{
	if(direction) *m_ddReg |= (1 << bit);
	else *m_ddReg &= ~(1 << bit);
	if(value) *m_outReg |= (1 << bit);
	else *m_outReg &= ~(1 << bit);
}
