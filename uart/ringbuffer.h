/*
 * ringbuffer.h
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

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <inttypes.h>

/**
 * \brief Ring buffer size.
 *
 * Size of the ring buffer in bytes. The size must be an integer power of 2.
 */
#define BUFFER_SIZE 128

typedef struct ringbuffer_ {
	char buffer[BUFFER_SIZE];
	size_t head;
	size_t tail;
} ringbuffer;

/**
 * \brief Initialize a ring buffer.
 *
 * Initialize a ring buffer. Reinitializing an already initialized ring buffer
 * causes its contents to be directly lost. This ring buffer implements a FIFO
 * structure.
 * \param[in] rb pointer to a ring buffer object
 */
void rb_init(ringbuffer* rb);

/**
 * \brief Add an element to a ring buffer.
 *
 * Add an element to a ring buffer if the buffer is not full. If an attempt
 * is made to add an element to a ring that is full, the function will silently
 * fail.
 * \param[in] rb pointer to a ring buffer object
 * \param[in] c character to add to the ring buffer
 */
void rb_put(ringbuffer* rb, char c);

/**
 * \brief Obtain element from ring buffer.
 *
 * Obtain an element from a ring buffer. If the ring buffer is empty, _F_EOF is
 * returned instead.
 * \param[in] rb pointer to a ring buffer object
 * \return Returns an element stored in the ring buffer using a FIFO scheme. If
 * the buffer is empty, _F_EOF is returned instead.
 */
int rb_get(ringbuffer* rb);

/**
 * \brief Returns whether ring buffer is full.
 *
 * Returns whether a ring buffer object is full.
 * \param[in] rb pointer to a ring buffer object
 * \return Returns zero if the ring buffer is not full, non-zero otherwise.
 */
uint8_t rb_full(ringbuffer* rb);

/**
 * \brief Returns whether ring buffer is empty.
 *
 * Returns whether a ring buffer object is empty.
 * \param[in] rb pointer to a ring buffer object
 * \return Returns zero if the ring buffer is not empty, non-zero otherwise.
 */
uint8_t rb_empty(ringbuffer* rb);

#endif /* RINGBUFFER_H_ */
