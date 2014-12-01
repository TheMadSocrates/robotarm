/*
 * interpreter.h
 *
 *  Created on: Nov 15, 2014
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

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#define PROGRAM_BUFFER 1024
uint8_t program[PROGRAM_BUFFER];

/**
 * \brief Updates a variable by a certain value.
 *
 * Computes the step of needed to reach a certain target value. As this is a
 * macro, variables should not be passed by reference.
 * \param[in] target target value to reach
 * \param[in] current current value
 * \param[out] variation trimmed delta needed to reach target
 */
#define UPDATE_VAR(target, current, variation) \
	if(( (current) < (target) ) && ( (target) - (current) < 0x08) ) \
		(variation) = (target) - (current); \
	else if(( (current) > (target) ) && ( (current) - (target) < 0x08) ) \
		(variation) = -((current) - (target)); \
	else if( (current) < (target) ) (variation) = 0x08; \
	else if( (current) > (target) ) (variation) = -0x08; \
	else variation = 0

/**
 * \brief Update program counter.
 *
 * Update the program counter of the execution module if targets have been
 * reached. Since this expands to a macro, variables should not be passed by
 * reference.
 * \param[in] targetA target value for first pair
 * \param[in] currentA current value for first pair
 * \param[in] targetB target value for second pair
 * \param[in] currentB current value for second pair
 * \param[in] targetC target value for third pair
 * \param[in] currentC current value for third pair
 * \param[inout] pc Program counter variable
 */
#define UPDATE_PC(targetA, currentA, targetB, currentB, targetC, currentC, pc) \
		if(targetA == currentA && targetB == currentB && targetC == currentC) \
			pc += 5;

#endif /* INTERPRETER_H_ */
