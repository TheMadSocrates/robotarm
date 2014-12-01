/*
 * hdconfig.h
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

#ifndef HDCONFIG_H_
#define HDCONFIG_H_

/*
 * Configuration parameters for HD44780 library. Library assumes the
 * availability of an 8-bit I/O port which can do reads and writes in parallel.
 * Consult the datasheet of for your microcontroller before configuring this
 * library.
 */
#include <avr/io.h>
#include <util/delay.h>

/* I/O port where LCD is located at */
#define hd_port_ C

/* LCD geometry */
#define rows_ 2
#define cols_ 20

/* Control signals pins */
#define RS 4
#define RW 5
#define EN 6
#ifdef _has_bell
#       define BELL 7
#       define BELL_LEN 50
#endif

/* Busy flag pin */
#define BF 3

/* internal macros */
#define hd_glue__(_a, _port) _a##_port
#define hd_glue_(_a, _port) hd_glue__(_a, _port)

#define HDDDR hd_glue_(DDR, hd_port_)
#define HDPORT hd_glue_(PORT, hd_port_)
#define HDPIN hd_glue_(PIN, hd_port_)

/* delay macro */
#define delay_function_(x) _delay_ms(x)
/* bit value macro */
#define bv_(n) _BV(n)

/* busy flag function */
#define wait_for_busy_flag() while(hd_bf_())

/* LCD memory map */
static const uint8_t _hd_map[] = {0x00, 0x40
#	if _rows == 4
                , 0x14, 0x54
#	endif
	};

/* LCD control codes */
typedef enum command_ {
	CLEAR = 0x01,
	MOVE_HOME = 0x02,
	EM_DECREMENT_OFF = 0x04,
	EM_DECREMENT_ON = 0x05,
	EM_INCREMENT_OFF = 0x06,
	EM_INCREMENT_ON = 0x07,
	BLANK_DISPLAY = 0x08,
	CURSOR_INVISIBLE = 0x0C,
	CURSOR_VISIBLE_BLOCK = 0x0D,
	CURSOR_VISIBLE_UNDERLINE = 0x0E,
	CURSOR_VISIBLE_ALT = 0x0F,
	MOVE_LEFT = 0x10,
	MOVE_RIGHT = 0x14,
	SCROLL_LEFT = 0x18,
	SCROLL_RIGHT = 0x1E,
	SET_CGRAM = 0x40,
	SET_DDRAM = 0x80
} command;

/* row and column */
static uint8_t _row;
static uint8_t _col;

/**
 * \brief Send a raw byte to the LCD.
 *
 * Send a raw byte to the LCD, selecting the proper register to write into.
 * This function will split the byte into two nibbles and send them
 * individually.
 * \param[in] c Byte to be sent to the LCD.
 * \param[in] _rs Register select. If set, write to data register, if cleared,
 * write to command register.
 */
void hd_send_raw_(char c, uint8_t _rs);

/**
 * \brief Strobe the E line of the LCD.
 *
 * Send a logic one followed by a logic zero on the Enable line of the LCD. The
 * function follows the timing characteristics of the LCD.
 */
void hd_strobe_(void);

/**
 * \brief Check the busy flag of the LCD.
 *
 * Check the busy flag of the LCD by setting the LCD to read mode, command
 * register and reading the data bus. The function follows the timing
 * characteristics of the LCD.
 * \return The status of the LCD. If set, LCD is busy and can not be sent
 * another command. If cleared, LCD is ready to accept new data.
 */
uint8_t hd_bf_(void);


#endif /* HDCONFIG_H_ */
