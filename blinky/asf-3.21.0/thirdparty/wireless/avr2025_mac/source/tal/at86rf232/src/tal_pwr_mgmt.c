/**
 * @file
 *
 * @brief
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
 */

/* === INCLUDES ============================================================ */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "tal_constants.h"
#include "at86rf232.h"
#include "tal_internal.h"
#ifdef STB_ON_SAL
#include "sal_types.h"
#if (SAL_TYPE == AT86RF2xx)
#include "stb.h"
#endif
#endif

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/*
 * \brief Sets the transceiver to sleep
 *
 * This function sets the transceiver to sleep state.
 *
 * \param mode Defines sleep mode of transceiver SLEEP or PHY_TRX_OFF)
 *
 * \return   TAL_BUSY - The transceiver is busy in TX or RX
 *           MAC_SUCCESS - The transceiver is put to sleep
 *           TAL_TRX_ASLEEP - The transceiver is already asleep
 *           MAC_INVALID_PARAMETER - The specified sleep mode is not supported
 */
retval_t tal_trx_sleep(sleep_mode_t mode)
{
	tal_trx_status_t trx_status;

	/* Current transceiver only supports SLEEP_MODE_1 mode. */
#if _DEBUG_ > 0
	if (SLEEP_MODE_1 != mode) {
		return MAC_INVALID_PARAMETER;
	}

#else
	/* Keep compiler happy */
	mode = mode;
#endif

	if (tal_trx_status == TRX_SLEEP) {
		return TAL_TRX_ASLEEP;
	}

	/* Device can be put to sleep only when the TAL is in IDLE state. */
	if (TAL_IDLE != tal_state) {
		return TAL_BUSY;
	}

	tal_rx_on_required = false;

	/*
	 * First set trx to TRX_OFF.
	 * If trx is busy, like ACK transmission, do not interrupt it.
	 */
	do {
		trx_status = set_trx_state(CMD_TRX_OFF);
	} while (trx_status != TRX_OFF);

	pal_timer_source_select(TMR_CLK_SRC_DURING_TRX_SLEEP);

	trx_status = set_trx_state(CMD_SLEEP);

#ifdef ENABLE_FTN_PLL_CALIBRATION

	/*
	 * Stop the calibration timer now.
	 * The timer will be restarted during wake-up.
	 */
	pal_timer_stop(TAL_CALIBRATION);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

	if (trx_status == TRX_SLEEP) {
#ifdef STB_ON_SAL
#if (SAL_TYPE == AT86RF2xx)
		stb_restart();
#endif
#endif
		return MAC_SUCCESS;
	} else {
		/* State could not be set due to TAL_BUSY state. */
		return TAL_BUSY;
	}
}

/*
 * \brief Wakes up the transceiver from sleep
 *
 * This function awakes the transceiver from sleep state.
 *
 * \return   TAL_TRX_AWAKE - The transceiver is already awake
 *           MAC_SUCCESS - The transceiver is woken up from sleep
 *           FAILURE - The transceiver did not wake-up from sleep
 */
retval_t tal_trx_wakeup(void)
{
	tal_trx_status_t trx_status;

	if (tal_trx_status != TRX_SLEEP) {
		return TAL_TRX_AWAKE;
	}

#ifdef ENABLE_FTN_PLL_CALIBRATION
	{
		retval_t timer_status;

		/*
		 * Calibration timer has been stopped when going to sleep,
		 * so it needs to be restarted.
		 * All other state changes except via sleep that are ensuring
		 * implicit filter tuning and pll calibration are ignored.
		 * Therefore the calibration timer needs to be restarted for
		 * to those cases.
		 * This is handled in file tal.c.
		 */

		/* Start periodic calibration timer. */
		timer_status = pal_timer_start(TAL_CALIBRATION,
				TAL_CALIBRATION_TIMEOUT_US,
				TIMEOUT_RELATIVE,
				(FUNC_PTR)calibration_timer_handler_cb,
				NULL);

		if (timer_status != MAC_SUCCESS) {
			Assert("PLL calibration timer start problem" == 0);
		}
	}
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

	trx_status = set_trx_state(CMD_TRX_OFF);

	if (trx_status == TRX_OFF) {
		pal_timer_source_select(TMR_CLK_SRC_DURING_TRX_AWAKE);
		return MAC_SUCCESS;
	} else {
		return FAILURE;
	}
}

/* EOF */
