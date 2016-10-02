/*
 * PushButton.h
 *
 *  Created on: Aug 7, 2016
 *      Author: compi
 */

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include <stdint.h>

extern "C" void TIMER1_OVF_vect() __attribute__((signal));

class PushButton
{
public:
	enum STATE
	{
		inactive,
		active,
		clicked,
		doubleclicked,
	};

	PushButton();
	void Init(volatile uint8_t &port, uint8_t bit, uint8_t debounceDelay, uint8_t clickDelay, uint8_t doubleclickDelay);
	inline uint16_t GetLastPushedTick() { return m_lastPushedTick; }
	STATE GetState();
	inline bool IsActive() { return m_state == active; }
	bool IsClicked();
	bool IsDoubleClicked();

private:
	void Tick(uint16_t now);

	friend void TIMER1_OVF_vect(void);

	volatile uint8_t	*m_port;
	uint8_t				m_bit;

	bool		m_debouncedState;
	bool		m_doubleclickAwaited;
	bool		m_clickAwaited;
	uint16_t	m_inputStateEqualTick;
	uint8_t		m_debounceDelay;
	uint8_t		m_clickDelay;
	uint8_t		m_doubleclickDelay;
	uint16_t	m_lastPushedTick;
	uint16_t	m_lastClickedTick;

	STATE		m_state;
};

#endif /* PUSHBUTTON_H_ */
