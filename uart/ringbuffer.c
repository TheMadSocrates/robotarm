/*
 * ringbuffer.c
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

#include <string.h>
#include <stdio.h>
#include "ringbuffer.h"

void rb_init(ringbuffer* rb) {
	/* initialize ring buffer pointers */
	rb->head = 0;
	rb->tail = BUFFER_SIZE - 1;
}

void rb_put(ringbuffer* rb, char c) {
	/* if the buffer is full, bail out */
	if(rb_full(rb)) return;
	/* place character on ring buffer */
	rb->buffer[rb->head++] = c;
	rb->head &= (BUFFER_SIZE - 1);
}

int rb_get(ringbuffer* rb) {
	if(rb_empty(rb)) return _FDEV_EOF;
	/* increment the tail */
	rb->tail++;
	rb->tail &= (BUFFER_SIZE - 1);
	/* and return the pointing character */
	return rb->buffer[rb->tail];
}

uint8_t rb_full(ringbuffer* rb) {
	/* if the head of the buffer has reached the tail, the buffer is full */
	return (((rb->head + 1) & (BUFFER_SIZE - 1)) == rb->tail);
}

uint8_t rb_empty(ringbuffer* rb) {
	/* if the tail of the buffer reaches the head, the buffer is empty */
	return (((rb->tail + 1) & (BUFFER_SIZE - 1)) == rb->head);
}
