/*
 * pwm.c
 *
 *  Created on: Nov 10, 2014
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

#include <avr/io.h>
#include "pwm.h"

void pwm_init(void) {
	/* Set up timers 1 and 3 in fast PWM mode, with a period of 20ms. Duty
	 * cycle is controlled by changing the OCRnA and OCRnB registers. This
	 * function assumes a 20MHz clock.
	 */
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
	ICR1 = 0xc350;
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
	TCCR3A = _BV(COM3A1) | _BV(COM3B1) | _BV(WGM31);
	ICR3 = 0xc350;
	TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31);
}
