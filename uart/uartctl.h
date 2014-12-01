/*
 * uartctl.h
 *
 *  Created on: Oct 29, 2014
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

#ifndef UARTCTL_H_
#define UARTCTL_H_

#include <stdio.h>

/**
 * \brief Initialize USART0 module.
 *
 * Initializes the UART0 module to 9600 8N1. This function is clock-agnostic,
 * expecting the value to be passed as compiler macro F_CPU. RX port is enabled
 * but requires interrupts to be manually enabled.
 */
void uart0_init(void);

/**
 * \brief Flushes the character queue.
 *
 * Flushes the queue of characters received from the USART0 module. The
 * character queue is implemented as a ring buffer.
 * \see ringbuffer.h
 */
void uart0_flush(void);

/**
 * \brief Low level interface to transmit a character.
 *
 * Low level interface to transmit a character from the USART0 module. This
 * function is provided to interface directly with the vprintf() family of
 * functions. UNIX carriage returns are emulated.
 * \param[in] c character to print
 * \param[in] unused unused pointer to a FILE object
 * \return This function always returns 0.
 */
int uart0_fputc(char c, FILE* unused);

/**
 * \brief Obtain a character from UART.
 *
 * Obtains a character from the software queue, if available. The characters
 * are retrieved using a FIFO structure implemented as a ring buffer. If the
 * ring buffer is empty, _F_EOF is returned instead.
 */
int uart0_getc(void);

/**
 * \brief Provides initialization of a FILE struct.
 *
 * This macro provides the initialization to a FILE type struct. The
 * initialization follows C99 style conventions.
 */
#define uart0_setup_stream() \
	FDEV_SETUP_STREAM(uart0_fputc, NULL, _FDEV_SETUP_WRITE);

#endif /* UARTCTL_H_ */
