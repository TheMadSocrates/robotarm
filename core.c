/*
 * core.c
 *
 *  Created on: Oct 24, 2014
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
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <util/delay.h>

#include "adc/adc.h"
#include "cmd/interpreter.h"
#include "fsm/fsm.h"
#include "lcd/hd44780.h"
#include "servo/pwm.h"
#include "uart/uartctl.h"

#include "core.h"

void main(void) {
	/* I/O Streams */
	FILE lcd0 = hd_setup_stream();
	FILE uart0 = uart0_setup_stream();

	/* program counter */
	uint16_t pc = 0;
	/* character from uart */
	int c;

	/* right/left motion */
	uint8_t rl_center;
	int8_t rl_value;
	uint16_t oc1a_val;
	uint16_t oc1a_target;

	/* up/down motion */
	uint8_t ud_center;
	int8_t ud_value;
	uint16_t oc1b_val;
	uint16_t oc1b_target;

	/* jaw motion */
	uint8_t jw_center;
	int8_t jw_value;
	uint16_t oc3a_val;
	uint16_t oc3a_target;

	/* FSM initialization */
	uint8_t fsm_input;
	fsm_init();

	/* operation mode select */
	DDRD = ~_BV(OP_MODE);
	DDRB = 0xff;

	/* initialize ADC and joysticks */
	adc_init();
	_delay_ms(100);
	rl_center = adc_getval(RL_CHANNEL) >> 3;
	_delay_ms(100);
	ud_center = adc_getval(UD_CHANNEL) >> 3;
	_delay_ms(100);
	jw_center = adc_getval(JW_CHANNEL) >> 3;

	/* initialize I/O peripherals */
	uart0_init();
	hd_init();
	sei();

	fprintf_P(&uart0, PSTR("\fSystem initialized\n\n"));
	fprintf_P(&lcd0, PSTR("System initialized..."));

	/* start timers */
	pwm_init();
	oc1a_val = OC1A_START;
	oc1b_val = OC1B_START;
	oc3a_val = OC3A_START;

	while(1) {
		/* assign value to output compare registers */
		OCR1A = oc1a_val;
		OCR1B = oc1b_val;
		OCR3A = oc3a_val;

		/* fsm input updates every loop */
		fsm_input = 0;

		/* operate according to FSM state */
		switch(fsm_state()) {
			case MANUAL_SWITCH:
				/* transition to manual mode */
				fprintf_P(&lcd0, PSTR("\fManual operation\n"));
				fprintf_P(&uart0, PSTR("Manual operation\n"));
				break;
			case MANUAL_MODE:
				/* compute joystick position */
				rl_value = rl_center - (adc_getval(RL_CHANNEL) >> 3);
				ud_value = ud_center - (adc_getval(UD_CHANNEL) >> 3);
				jw_value = jw_center - (adc_getval(JW_CHANNEL) >> 3);
				/* update PWM variables */
				UPDATE_OC(oc1a_val, rl_value, OC1A_MIN, OC1A_MAX);
				UPDATE_OC(oc1b_val, ud_value, OC1B_MIN, OC1B_MAX);
				UPDATE_OC(oc3a_val, jw_value, OC3A_MIN, OC3A_MAX);
				/* and place them on screen */
				fprintf_P(&lcd0, PSTR("\r%04x %04x %04x %04x"),
						oc1a_val, oc1b_val, oc3a_val, 0);
				break;
			case AUTO_SWITCH:
				/* transition to automatic mode */
				fprintf_P(&lcd0, PSTR("\fAutomatic operation\n"));
				fprintf_P(&uart0, PSTR("Automatic operation\n"));
				/* discard all commands given to the arm */
				uart0_flush();
				/* and reset program counter */
				pc = 0;
				break;
			case PROGRAMMING_MODE:
				fprintf_P(&lcd0, PSTR("\rNo program"));
				/* obtain a character from the uart module */
				c = uart0_getc();
				/* and place it on the program buffer */
				if(c != _FDEV_EOF) {
					program[pc++] = (uint8_t)c;
					fputc(c, &uart0);
				}
				/* if we have return or the buffer is full, set the done flag */
				if(c == '\r' || pc == PROGRAM_BUFFER) {
					pc = 0;
					fputc('\n', &uart0);
					fsm_input = UPLOAD_DONE;
				}
				break;
			case AUTO_MODE:
				fprintf_P(&lcd0, PSTR("\rExecuting: "));
				/* default position values */
				oc1a_target = oc1a_val;
				oc1b_target = oc1b_val;
				oc3a_target = oc3a_val;

				/* command interpreter */
				switch(program[pc]) {
					case 'R':	/* reset */
						oc1a_target = OC1A_START;
						oc1b_target = OC1B_START;
						oc3a_target = OC3A_START;
						fprintf_P(&lcd0, PSTR("RESET"));
						break;
					case 'H':	/* horizontal movement */
						oc1a_target =
								strtol((char*)&program[pc+1], (char**)NULL, 16);
						fprintf_P(&lcd0, PSTR("H %04x"), oc1a_target);
						break;
					case 'V':	/* vertical movement */
						oc1b_target =
								strtol((char*)&program[pc+1], (char**)NULL, 16);
						fprintf_P(&lcd0, PSTR("V %04x"), oc1b_target);
						break;
					case 'J':	/* jaw movement */
						oc3a_target =
								strtol((char*)&program[pc+1], (char**)NULL, 16);
						fprintf_P(&lcd0, PSTR("J %04x"), oc3a_target);
						break;
					case 'C':	/* claw movement */
						fprintf_P(&lcd0, PSTR("NOCLAW"));
						break;
					default:	/* bad command */
						fprintf_P(&lcd0, PSTR("BADCMD"));
				}

				/* update values */
				UPDATE_VAR(oc1a_target, oc1a_val, rl_value);
				UPDATE_VAR(oc1b_target, oc1b_val, ud_value);
				UPDATE_VAR(oc3a_target, oc3a_val, jw_value);

				/* update pc */
				UPDATE_PC(oc1a_target, oc1a_val, oc1b_target, oc1b_val,
						oc3a_target, oc3a_val, pc);

				if(program[pc] == '\r' || pc >= PROGRAM_BUFFER) {
					fprintf_P(&uart0, PSTR("\nok\n"));
					fsm_input = PROGRAM_DONE;
				}
				/* update PWM variables */
				UPDATE_OC(oc1a_val, rl_value, OC1A_MIN, OC1A_MAX);
				UPDATE_OC(oc1b_val, ud_value, OC1B_MIN, OC1B_MAX);
				UPDATE_OC(oc3a_val, jw_value, OC3A_MIN, OC3A_MAX);

				break;
			case IDLE:
				/* idle state */
				fprintf_P(&uart0, PSTR("Initializing FSM\n"));
				fprintf_P(&lcd0, PSTR("Initializing FSM\r"));
				break;
			default:
				/* if we got here, we broke it */
				fprintf_P(&uart0, PSTR("Invalid mode: %d\n"), fsm_state());
				fprintf_P(&lcd0, PSTR("Invalid mode: %d\r"), fsm_state());
				break;
		}

		/* and advance FSM state according to inputs */
		if(PIND & _BV(OP_MODE))
			fsm_input |= MODE_SELECT;
		fsm_advance(fsm_input);

		/* the above code can't run continuously, must delay execution */
		/* ISSUE: USART0 RX ISR may trigger faster than this delay. This causes
		 * the ring buffer to fill faster than it can be emptied. Disabling
		 * interrupts throughout the loop will result in characters being lost
		 * on the USART0 module. Loop delay must be tuned along with USART0
		 * baud rate.
		 */
		_delay_ms(10);
	}
}
