/*
 * SmartButton.h
 *
 *  Created on: Oct 3, 2016
 *      Author: compi
 */

#ifndef SRC_SMARTBUTTON_H_
#define SRC_SMARTBUTTON_H_

#include <ButtonLogic.h>
#include <DebouncedInput.h>

class SmartButton
{
public:
	SmartButton() = default;
	void Init(uint16_t debounceDelay, IOPort* port, uint8_t bit, bool activeLow, uint16_t clickDelay, uint16_t dimDelay);
	inline uint8_t Tick(uint16_t now);

private:
	SmartButton(const SmartButton&) = delete;

	DebouncedInput m_input;
	ButtonLogic		m_logic;
};

inline uint8_t SmartButton::Tick(uint16_t now)
{
	return m_logic.Tick(m_input.Tick(now), now);
}

#endif /* SRC_SMARTBUTTON_H_ */
