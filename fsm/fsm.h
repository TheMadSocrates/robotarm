/*
 * fsm.h
 *
 *  Created on: Nov 12, 2014
 *      Author: Orlando Arias
 *
 *	Copyright (C) 2014  Orlando Arias
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FSM_H_
#define FSM_H_

#include <inttypes.h>

typedef enum states_ {
	IDLE,
	MANUAL_SWITCH,
	MANUAL_MODE,
	AUTO_SWITCH,
	PROGRAMMING_MODE,
	AUTO_MODE
} state;

#define MODE_SELECT (1 << 0)
#define UPLOAD_DONE (1 << 2)
#define PROGRAM_DONE (1 << 3)

/**
 * \brief Initialize the state machine.
 *
 * Initialize the internal deterministic finite automaton.
 */
void fsm_init(void);

/**
 * \brief Obtain the current state of the state machine.
 *
 * Obtain the current state of the deterministic finite automaton is in.
 * \return The state the automaton is in.
 */
state fsm_state(void);

/**
 * \brief Update the state of the state machine.
 *
 * Update the current state of the deterministic finite automaton based on a
 * given input.
 * \param[in] input input flags for the automaton
 */
void fsm_advance(uint8_t input);

#endif /* FSM_H_ */
