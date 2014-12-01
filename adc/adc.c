/*
 * adc.c
 *
 *  Created on: Oct 30, 2014
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

#include "adc.h"

void adc_init(void) {
	/* initialize the ADC */
	ADMUX |= _BV(ADLAR) | _BV(REFS0);	/* result is divided by 8, AVcc ref */
	DIDR0 |= _BV(ADC1D) | _BV(ADC0D);	/* disable digital inputs */
	/* enable ADC, set ADC clock */
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint8_t adc_getval(uint8_t channel) {
	/* ADC channel selection */
	ADMUX &= ~(_BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0));
	ADMUX |= channel;
	/* start conversion */
	ADCSRA |= _BV(ADSC);
	/* wait until conversion is finished */
	loop_until_bit_is_clear(ADCSRA, ADSC);
	/* and return conversion value divided by 8 */
	return ADCH;
}
