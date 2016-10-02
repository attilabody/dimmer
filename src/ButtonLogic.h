/*
* SmartButton.h
*
* Created: 9/3/2016 1:44:59 PM
* Author: compi
*/
#ifndef __BUTTONLOGIC_H__
#define __BUTTONLOGIC_H__

#include <inttypes.h>

class ButtonLogic
{
public:
	ButtonLogic() = default;
	void Init(
	    uint16_t clickDelay,
	    uint16_t dimDelay
	);
	void Reset();

	uint8_t Tick(bool buttonActive, uint16_t now);
	uint8_t GetState();
	inline uint8_t GetRawState() { return m_state; }

protected:
private:
	ButtonLogic(const ButtonLogic &c) = delete;
	ButtonLogic &operator=(const ButtonLogic &c) = delete;

public:
protected:
private:
	bool		m_lastInputActive = false;

	uint16_t	m_clickDelay;
	uint16_t	m_dimDelay;

	uint16_t	m_startTick = 0;
	uint8_t		m_releaseCount = 0;

	uint8_t		m_state;
};

#endif //__BUTTONLOGIC_H__
