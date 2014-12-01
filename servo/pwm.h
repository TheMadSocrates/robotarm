/*
 * pwm.h
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

#ifndef PWM_H_
#define PWM_H_

/* right/left servo positions */
#define OC1A_MIN	0x0540
#define OC1A_MAX	0x1400
#define OC1A_START	0x0cb0

/* up/down servo positions */
#define OC1B_MIN	0x0b50
#define OC1B_MAX	0x14e0
#define OC1B_START	0x0e00

/* jaw servo positions */
#define OC3A_MIN	0x0900
#define OC3A_MAX	0x1120
#define	OC3A_START	0x0e00


/**
 * \brief Initialize timers in PWM mode.
 *
 * Initialize high resolution timers in fast PWM mode with a period of 20ms.
 * This function assumes a 20MHz clock.
 */
void pwm_init(void);

/**
 * \brief Update an output compare register variable.
 *
 * Update an output compare register variable if the result would be between
 * the allowed range. This macro expands to a conditional statement and an
 * assignment.
 * \param[inout] current_val Output compare variable.
 * \param[in] modifier Modifier value.
 * \param[in] min Minimum value for output compare variable.
 * \param[in] max Maximum value for output compare variable.
 */
#define UPDATE_OC(current_val, modifier, min, max) \
	if((( (current_val) + (modifier)) <= (max) ) \
			&& (( (current_val) + (modifier) ) >= (min) )) \
		(current_val) += (modifier)

#endif /* PWM_H_ */
