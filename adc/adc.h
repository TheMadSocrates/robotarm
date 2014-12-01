/*
 * adc.h
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

#ifndef ADC_H_
#define ADC_H_

#include <inttypes.h>

/**
 * \brief Initialize the ADC.
 *
 * Initialize the Analog to Digital Converter with the following parameters:
 * - store 8 bits in ADCH, two bits in ADCL
 * - clock divider as 8
 * - ADC is enabled
 */
void adc_init(void);

/**
 * \brief Obtain a reading from the ADC unit.
 *
 * Obtain a reading from the ADC unit on a given channel. Function will not
 * return until the reading has been finished.
 *
 * \param[in] channel ADC channel to read. It is the user's responsibility to
 * provide a valid ADC channel to read as no sanity check is performed. Passing
 * an invalid channel may result in undefined readings and/or corruption of
 * the ADCSRA register.
 *
 * \return Upon completion, the function returns the value stored in ADCH. With
 * the settings given to the ADC using the adc_init() function, this is
 * equivalent to returning the read value divided by 4.
 */
uint8_t adc_getval(uint8_t channel);

#endif /* ADC_H_ */
