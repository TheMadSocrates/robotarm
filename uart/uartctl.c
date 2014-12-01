/*
 * uartctl.c
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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uartctl.h"
#include "ringbuffer.h"

#define BUFFER_FULL PD2
#define BUFFER_EMPTY PD3

/* uart data ring buffer */
ringbuffer rb;

/* UART0 receive interrupt */
ISR(USART0_RX_vect) {
	char c = UDR0;
	/* check if ring buffer is full */
	if(rb_full(&rb)) {
		/* signal the user the buffer is full */
		PORTD |= _BV(BUFFER_FULL);
		/* and bail out */
		return;
	}
	/* otherwise, add the character to the buffer */
	PORTD &= ~_BV(BUFFER_FULL);
	rb_put(&rb, c);
}

void uart0_init(void) {
	/* initialize the ring buffer */
	rb_init(&rb);
	/* set TX and RX pins */
	DDRD |= _BV(PD1);
	DDRD &= ~_BV(PD0);
	/* compute the baud rate */
#	define BAUD 9600
#	include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#	if USE_2x
		UCSR0A |= _BV(U2X);
#	else
		UCSR0A &= ~_BV(U2X0);
#	endif
	/* and enable receiver, transmitter and RX interrupt */
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
}

void uart0_flush(void) {
	/* initializing the ring buffer effectively flushes it */
	rb_init(&rb);
}

int uart0_fputc(char c, FILE* unused) {
	/* emulate UNIX style newlines */
	if(c == '\n')
		uart0_fputc('\r', unused);
	/* wait for previous transmit operation */
	loop_until_bit_is_set(UCSR0A, UDRE0);
	/* and save the character into the data register */
	UDR0 = c;

	/* ring buffer status update */
	if(rb_empty(&rb)) PORTD |= _BV(BUFFER_EMPTY);
	else PORTD &= ~_BV(BUFFER_EMPTY);
	return 0;
}

int uart0_getc(void) {
	/* return a character from the ring buffer or EOF if empty */
	return rb_get(&rb);
}
