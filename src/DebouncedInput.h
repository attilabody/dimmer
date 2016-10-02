/*
 * DebouncedInput.h
 *
 *  Created on: Oct 2, 2016
 *      Author: compi
 */

#ifndef SRC_DEBOUNCEDINPUT_H_
#define SRC_DEBOUNCEDINPUT_H_
#include "IOPort.h"

class DebouncedInput {
public:
	DebouncedInput() = default;
	void Init(uint16_t debounceDelay, IOPort* port, uint8_t bit, bool activeLow);
	bool Tick(uint16_t now);

private:
	DebouncedInput(const DebouncedInput&) = delete;

	IOPort		*m_port = nullptr;
	uint8_t		m_bit = 0;
	bool		m_activeLow = false;
	bool		m_debouncedState = false;
	uint16_t	m_inputStateEqualTick = 0;
	uint16_t	m_debounceDelay = 0;
};

#endif /* SRC_DEBOUNCEDINPUT_H_ */
