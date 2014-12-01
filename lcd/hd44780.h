/*
 * hd44780.h
 *
 *  Created on: Nov 6, 2014
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

#ifndef HD44780_H_
#define HD44780_H_

#include <stdio.h>

/**
 * \brief Initialize a struct of type FILE for use with the vprintf family of
 * functions.
 *
 * Initialize a struct of type FILE for use with the vprintf family of
 * functions in the stdio.h header file. Note that this macro just provides
 * initialization of the struct.
 */
#define hd_setup_stream() FDEV_SETUP_STREAM(hd_fputc, 0, _FDEV_SETUP_WRITE)

/**
 * \brief Initialize the LCD controller.
 *
 * Start an software initialization sequence of the LCD controller and set some
 * default parameters. This function must be called before any data can be
 * written to the LCD.
 */
void hd_init(void);

/**
 * \brief The fputc() function for the LCD screen.
 *
 * Standard fputc() function handling data output on the LCD. This low level
 * function provides the interface for the vprintf family of functions to write
 * to the LCD.
 * \param[in] c Character to be displayed.
 * \param[in] unused A pointer to a file stream. Can be (FILE*)NULL.
 * \return Upon completion, this function returns 0. No error checking is done.
 */
int hd_fputc(char c, FILE *unused);

#endif /* HD44780_H_ */
