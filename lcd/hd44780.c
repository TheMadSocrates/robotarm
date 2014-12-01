/*
 * hd44780.c
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

#include <inttypes.h>
#include <stdio.h>

#include "hd44780.h"
#include "hdconfig.h"

void hd_init(void) {
	/* set DDR */
	HDDDR = (0x0F | bv_(EN) | bv_(RS) | bv_(RW));

	/* start software initialization of controller as per datasheet */
	delay_function_(40);
	HDPORT = 0x03;
	hd_strobe_();
	delay_function_(4.1);
	HDPORT = 0x03;
	hd_strobe_();
	delay_function_(0.1);
	HDPORT = 0x03;
	hd_strobe_();
	delay_function_(0.1);
	HDPORT = 0x02;
	hd_strobe_();
	/* and provide a few defaults */
	hd_send_raw_(0x28, 0);
	hd_send_raw_(BLANK_DISPLAY, 0);
	hd_send_raw_(CURSOR_INVISIBLE, 0);
	hd_send_raw_(EM_INCREMENT_OFF, 0);
	hd_send_raw_(CLEAR, 0);
	hd_send_raw_(MOVE_HOME, 0);
}

int hd_fputc(char c, FILE *unused) {
	if(c == '\f') {				/* form feed - clear screen */
		_row = 0;
		_col = 0;
		hd_send_raw_(CLEAR, 0);

	/* horizontal movement */
	} else if(c == '\r') {		/* carriage return - cursor to line start */
		_col = 0;
		hd_send_raw_(SET_DDRAM | _hd_map[_row], 0);

	} else if(c == '\b') {		/* backspace - cursor left */
		if(_col > 0)
			_col--;
		else
			_col = cols_ - 1;
		hd_send_raw_(SET_DDRAM | (_hd_map[_row] + _col), 0);

	} else if(c == '\t') {		/* tab - cursor right */
		_col++;
		if(_col == cols_)
			_col = 0;
		hd_send_raw_(SET_DDRAM | (_hd_map[_row] + _col), 0);

	/* vertical movement */
	} else if(c == '\v') {		/* vertical tab - cursor to previous line */
		if(_row == 0)
			_row = rows_ - 1;
		else
			_row--;
		hd_send_raw_(SET_DDRAM | (_hd_map[_row] + _col), 0);

	} else if(c == '\n') {		/* line feed - cursor to next line */
		_row++;
		if(_row == rows_)
			_row = 0;
		hd_send_raw_(SET_DDRAM | (_hd_map[_row] + _col), 0);

	/* miscellaneous */
	} else if(c == '\a') {		/* system bell */
		hd_send_raw_('*', 1);
	} else {					/* printable character */
		hd_send_raw_(c, 1);

		_col++;
		if(_col == cols_) {
			_col = 0;
			_row++;
			if(_row == rows_)
				_row = 0;

			hd_send_raw_(SET_DDRAM | _hd_map[_row], 0);
		}
	}
	return 0;
}

void hd_send_raw_(char c, uint8_t _rs) {
	wait_for_busy_flag();			/* wait for LCD to be free */
	HDPORT &= ~(0x0f | bv_(RW));	/* clear low nibble of port previous to
									 * operation and clear R/~W for a write
									 */

	if (_rs)						/* register select */
		HDPORT |= bv_(RS);
	else
		HDPORT &= ~bv_(RS);

	HDPORT |= (c >> 4);				/* send high nibble */
	hd_strobe_();

	HDPORT &= ~(0x0f);
	HDPORT |= (c & 0x0f);			/* send low nibble */
	hd_strobe_();
}

void hd_strobe_(void) {
	delay_function_(0.000060);		/* t_AS = 60ns */
	HDPORT |= bv_(EN);
	delay_function_(0.000450);		/* PW_EH = 450ns */
	HDPORT &= ~bv_(EN);
	delay_function_(0.000020);		/* t_AH = 20ns */
}

uint8_t hd_bf_(void) {
	uint8_t bf = 0;
	HDPORT |= bv_(RW);
	HDPORT &= ~bv_(RS);				/* read operation, command register */
	HDDDR &= 0xf0;					/* set data bus as input */
	for(uint8_t i = 0; i < 2; i++) {
		delay_function_(0.000060);	/* t_AS = 60ns */
		HDPORT |= bv_(EN);
		delay_function_(.000225);	/* t_DDR = 160ns data is available */
		if(i == 0)
			bf = (HDPIN & bv_(BF));
		delay_function_(0.000225);	/* PW_EH = 450 ns */
		HDPORT &= ~bv_(EN);
		delay_function_(0.000020);	/* t_AH = 20 ns */
	}
	HDDDR |= 0x0f;					/* restore data bus as output */
	return bf;
}
