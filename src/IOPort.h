/*
* IOPort.h
*
* Created: 9/3/2016 6:56:08 PM
* Author: compi
*/


#ifndef __IOPORT_H_INCLUDED__
#define __IOPORT_H_INCLUDED__
#include <inttypes.h>

class IOPort
{
public:
	IOPort(
	    volatile uint8_t *inReg,
	    volatile uint8_t *outReg,
	    volatile uint8_t *ddReg,
		uint8_t ddMask = 0,			//all port are input
		uint8_t	outValues = 0		//no pullups
	);

	void ConfigurePin(uint8_t bit, bool direction, bool value = 0);
	inline bool Read(uint8_t bit);
	inline uint8_t Read();
	inline void Write(uint8_t bit, bool value);
	inline void Write(uint8_t value);

protected:
private:
	IOPort(const IOPort &c);
	IOPort &operator=(const IOPort &c);

	volatile uint8_t *m_inReg = nullptr;
	volatile uint8_t *m_outReg = nullptr;
	volatile uint8_t *m_ddReg = nullptr;
};

//////////////////////////////////////////////////////////////////////////////
inline bool IOPort::Read(uint8_t bit)
{
	return (*m_inReg & (1 << bit)) != 0;
}

//////////////////////////////////////////////////////////////////////////////
inline uint8_t IOPort::Read()
{
	return *m_inReg;
}

//////////////////////////////////////////////////////////////////////////////
inline void IOPort::Write(uint8_t bit, bool value)
{
	if(value) *m_outReg |= (1 << bit);
	else *m_outReg &= (1 << bit);
}

//////////////////////////////////////////////////////////////////////////////
inline void IOPort::Write(uint8_t value)
{
	*m_outReg = value;
}


#endif //__IOPORT_H__

