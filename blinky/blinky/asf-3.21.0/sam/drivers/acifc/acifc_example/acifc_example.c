/**
 * \file
 *
 * \brief SAM4L Analog Comparator Interfacer (ACIFC) example.
 *
 * Copyright (c) 2013-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <stdio_serial.h>
#include <conf_board.h>
#include <conf_clock.h>

//! [acifc_define_channel]
/** Analog comparator channel number */
#define EXAMPLE_AC_CHANNEL         0
//! [acifc_define_channel]

#define STRING_EOL    "\r"
#define STRING_HEADER "-- ACIFC IRQ Example  --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

//! [acifc_struct_declaration]
struct ac_dev_inst ac_device;
//! [acifc_struct_declaration]

/* State indicate */
volatile bool state = false;

/**
 * Callback for channel 0 comparison done.
 */
static void compare_result_output(void)
{
	uint32_t ul_comp_status = ac_get_status(&ac_device);

	/* Compare Output Interrupt */
	if ((ul_comp_status & ACIFC_SR_ACCS0) == ACIFC_SR_ACCS0) {
		puts("-ISR- Voltage Comparison Result: ACAP0 > ACAN0\r");
	} else {
		puts("-ISR- Voltage Comparison Result: ACAP0 < ACAN0\r");
	}
	ac_clear_interrupt_status(&ac_device, ACIFC_ICR_ACINT0);
        state = true;
}

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 *  \brief ACIFC example application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Initialize debug console */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	//! [acifc_init_and_configure]
	/** AC instance configuration */
	//! [acifc_init_and_configure_1]
	struct ac_config module_cfg;
	ac_get_config_defaults(&module_cfg);
	ac_init(&ac_device, ACIFC, &module_cfg);
	//! [acifc_init_and_configure_1]

	ac_enable(&ac_device);

	/** AC channel configuration */
	//! [acifc_init_and_configure_2]
	struct ac_ch_config ch_cfg;
	ac_ch_get_config_defaults(&ch_cfg);
	ch_cfg.always_on = true;
	ch_cfg.fast_mode = true;
	ac_ch_set_config(&ac_device, EXAMPLE_AC_CHANNEL, &ch_cfg);
	//! [acifc_init_and_configure_2]
	//! [acifc_init_and_configure]
	ac_set_callback(&ac_device, AC_INTERRUPT_CONVERSION_COMPLETED_0,
			compare_result_output, 1);

	/* Start the comparison */
	ac_user_trigger_single_comparison(&ac_device);

	while (1) {
		while(state == false);
		state = true;
		/* Start the comparison */
		ac_user_trigger_single_comparison(&ac_device);
		delay_ms(1000);
	}
}
