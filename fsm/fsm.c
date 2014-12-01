/*
 * fsm.c
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

#include <inttypes.h>
#include "fsm.h"

state fsm;

void fsm_init(void) {
	fsm = IDLE;
}

state fsm_state(void) {
	return fsm;
}

void fsm_advance(uint8_t input) {
	switch(fsm) {
		case IDLE:
			if(input & MODE_SELECT)
				fsm = MANUAL_SWITCH;
			else
				fsm = AUTO_SWITCH;
			break;
		case MANUAL_SWITCH:
			fsm = MANUAL_MODE;
			break;
		case MANUAL_MODE:
			if(!(input & MODE_SELECT))
				fsm = AUTO_SWITCH;
			break;
		case AUTO_SWITCH:
			fsm = PROGRAMMING_MODE;
			break;
		case PROGRAMMING_MODE:
			if(input & UPLOAD_DONE)
				fsm = AUTO_MODE;
			else if(input & MODE_SELECT)
				fsm = MANUAL_SWITCH;
			break;
		case AUTO_MODE:
			if(input & MODE_SELECT)
				fsm = MANUAL_SWITCH;
			if(input & PROGRAM_DONE)
				fsm = AUTO_SWITCH;
			break;
		default:
			fsm = IDLE;
	}
}
