/**
 * \file
 *
 * \brief Unit test configuration
 *
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
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
#ifndef CONF_TEST_H
#define CONF_TEST_H

#include <ebi.h>
#include <ebi_port.h>
#include <string.h>
#include <sysclk.h>

//! \note USART0 on PORTD (RX on PD2, TX on PD3)
#define CONF_TEST_USART      &USARTD0
//! \note 38.4 kbaud
#define CONF_TEST_BAUDRATE   38400
//! \note 8-bit character length
#define CONF_TEST_CHARLENGTH USART_CHSIZE_8BIT_gc
//! \note No parity check
#define CONF_TEST_PARITY     USART_PMODE_DISABLED_gc
//! \note No extra stopbit, i.e., use 1
#define CONF_TEST_STOPBITS   false

//! \note The EBI is set up so that external RAM starts at 0x800000.
#define CONF_TEST_RAM_BASE   0x800000UL
//! \note The external SDRAM device on Xplain is of size 8 MB.
#define CONF_TEST_RAM_SIZE   0x800000UL

//! \name Convenience macros for EBI initialization
//@{
//! SDRAM initialization delay in number of CLKper2 cycles (100 us)
#define EBI_SDRAM_INITDLY    (100 * 2 * sysclk_get_per2_hz() / 1000000)
//! SDRAM refresh interval in number of CLKper2 cycles (16 us)
#define EBI_SDRAM_REFRESH    (16 * 2 * sysclk_get_per2_hz() / 1000000)
//@}

static void ebi_sdram_init(void)
{
	static struct ebi_sdram_config sdram_config;
	static struct ebi_cs_config    cs_config;

	memset(&sdram_config, 0, sizeof(sdram_config));
	memset(&cs_config, 0, sizeof(cs_config));

	/*
	 * Configure the EBI port with 12 address lines, no address latches or
	 * low pin count, and set it in SDRAM mode with 3-port EBI port.
	 */
	ebi_setup_port(12, 0, 0, EBI_PORT_3PORT | EBI_PORT_SDRAM);

	/*
	 * Configure the EBI chip select for an 8 MB SDRAM located at
	 * \ref CONF_TEST_RAM_BASE.
	 */
	ebi_cs_set_mode(&cs_config, EBI_CS_MODE_SDRAM_gc);
	ebi_cs_set_address_size(&cs_config, EBI_CS_ASPACE_8MB_gc);
	ebi_cs_set_base_address(&cs_config, CONF_TEST_RAM_BASE);

	// Configure the EBI chip select to be in SDRAM mode.
	ebi_sdram_set_mode(&cs_config, EBI_CS_SDMODE_NORMAL_gc);

	// Setup the number of SDRAM rows and columns.
	ebi_sdram_set_row_bits(&sdram_config, 12);
	ebi_sdram_set_col_bits(&sdram_config, 10);

	// Further, setup the SDRAM timing.
	ebi_sdram_set_cas_latency(&sdram_config, 3);
	ebi_sdram_set_mode_delay(&sdram_config, EBI_MRDLY_2CLK_gc);
	ebi_sdram_set_row_cycle_delay(&sdram_config, EBI_ROWCYCDLY_7CLK_gc);
	ebi_sdram_set_row_to_precharge_delay(&sdram_config, EBI_RPDLY_7CLK_gc);
	ebi_sdram_set_write_recovery_delay(&sdram_config, EBI_WRDLY_1CLK_gc);
	ebi_sdram_set_self_refresh_to_active_delay(&sdram_config,
			EBI_ESRDLY_7CLK_gc);
	ebi_sdram_set_row_to_col_delay(&sdram_config, EBI_ROWCOLDLY_7CLK_gc);
	ebi_sdram_set_refresh_period(&sdram_config, EBI_SDRAM_REFRESH);
	ebi_sdram_set_initialization_delay(&sdram_config, EBI_SDRAM_INITDLY);

	// Write SDRAM configuration into the EBI registers.
	ebi_sdram_write_config(&sdram_config);
	// Write the chip select configuration into the EBI registers.
	ebi_cs_write_config(EBI_SDRAM_CS, &cs_config);

	ebi_enable_cs(EBI_SDRAM_CS, &cs_config);

	do {
		// Wait for SDRAM to initialize.
	} while (!ebi_sdram_is_ready());
}

#endif
