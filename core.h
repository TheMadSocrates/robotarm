/*
 * core.h
 *
 *  Created on: Nov 14, 2014
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

#ifndef CORE_H_
#define CORE_H_

/* board parameters */
#define ACTIVE PD7
/* mode select pin */
#define OP_MODE PD6

/* up/down joystick channel */
#define UD_CHANNEL 0
/* right/left joystick channel */
#define RL_CHANNEL 1
/* jaw up/down joystick channel */
#define JW_CHANNEL 2

/**
 * \brief Main function.
 *
 * Main program function. Initializes all subsystems and state machine. Main
 * loop acts depending on state machine.
 */
void main(void) __attribute__((noreturn));

#endif /* CORE_H_ */
