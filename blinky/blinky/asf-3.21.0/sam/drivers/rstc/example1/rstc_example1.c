/**
 * \file
 *
 * \brief SAM Reset Controller (RSTC) Driver Example
 *
 * Copyright (C) 2012-2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
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
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include <stdio_serial.h>
#include <conf_clock.h>
#include <conf_board.h>
#include <rstc_example1.h>

#define STRING_EOL    "\r"
#define STRING_HEADER "-- RSTC Reset Controller Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** \internal Watchdog period 3000ms. */
#define WDT_PERIOD		3000
		
/* SysTick Counter */
static volatile uint32_t tick_count = 0U; 
static volatile bool watchdog_enabled = true;
//! [reset_irq_variable]
static volatile bool reset_interrupt_triggered = false;
//! [reset_irq_variable]
/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	static const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Configure the watchdog timer.
 */
 static void configure_watchdog(void)
 {
	/* Get timeout value for 3 seconds. */
	uint32_t t = wdt_get_timeout_value(WDT_PERIOD * 1000,
			BOARD_FREQ_SLCK_XTAL);

	/* Initialize the watchdog. */
	wdt_init(WDT, WDT_MR_WDRSTEN | WDT_MR_WDRPROC /* | WDT_MR_WDDBGHLT | WDT_MR_WDIDLEHLT */,
			t, t);
 }
 
/**
 * \brief Wait for UART to finish transmitting.
 */
static void wait_for_message_gone(void)
{
	while (!uart_is_tx_buf_empty(CONSOLE_UART));
}

/**
 * \brief SysTick interrupt handler.
 */
void SysTick_Handler(void)
{
	/* Time to tickle the watchdog? */
	if (++tick_count>500) {
		tick_count = 0;
		
		/* Watchdog enabled? */
		if (watchdog_enabled) {
			wdt_restart(WDT);
		}
	}
}

/**
 * \brief RSTC interrupt handler.
 */
//! [reset_irq_handler] 
void RSTC_Handler(void)
{
	/* Clear the interrupt. */
	rstc_get_status(RSTC);
	
	reset_interrupt_triggered = true;
}
//! [reset_irq_handler] 

/**
 * \brief Display the menu.
 */
static void display_menu(void)
{
	puts("\r\nMenu:\r\n"
			"  0 - Reset Status information.\r\n"
			"  1 - User Reset enable.\r\n"
			"  2 - User Reset disable.\r\n"
			"  3 - User Reset interrupt enable.\r\n"
			"  4 - User Reset interrupt disable.\r\n"
			"  5 - Software Reset.\r\n"
			"  6 - Watchdog Reset.\r\n"
			"  7 - NRST assert.\r");
}

/**
 * \brief Display the reset reason(s).
 */
 static void display_reset_reason(void)
 {
	char msg[80];
	
	strcpy(&msg[0], "\r\nReset info : ");
	
	//! [reset_get_status]
	uint32_t info = rstc_get_status(RSTC);
	//! [reset_get_status]
	
	/* Decode the reset reason. */
	switch (info & RSTC_SR_RSTTYP_Msk) {	
	case RSTC_GENERAL_RESET:
		strcat(&msg[0], "General Reset,");
	break;
	
	case RSTC_BACKUP_RESET:
		strcat(&msg[0], "Backup Reset,");
	break;
	
	case RSTC_WATCHDOG_RESET:
		strcat(&msg[0], "Watchdog Reset,");
	break;
	
	case RSTC_SOFTWARE_RESET:
		strcat(&msg[0], "Software Reset,");
	break;
	
	case RSTC_USER_RESET:
		strcat(&msg[0], "User Reset,");
	break;
	
	default:
		strcat(&msg[0], "Invalid reset reason!,");
	}
	
	/* NRST level. */
	if (info & RSTC_SR_NRSTL) {
		strcat(&msg[0], " NRST=1,");
	}
	else {
		strcat(&msg[0], " NRST=0,");
	}

	/* User reset status. */
	if (info & RSTC_SR_URSTS) {
		strcat(&msg[0], " User Reset=1\r");
	}
	else {
		strcat(&msg[0], " User Reset=0\r");
	}

	puts(&msg[0]);
 }
 
/**
 *  \brief SUPC RTC Wakeup Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_key;
	
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	/* Initialize the console UART. */
	configure_console();

	/* Output example information. */
	puts(STRING_HEADER);

	/* Set up SysTick Timer for 1 msec interrupts. */
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("\r\nSysTick_Config failed!\r");
		/* Critical error. */
		while (1);
	}

	/* Display the reset reason. */
	display_reset_reason();
	
	/* Display the menu. */
	display_menu();
	
	while (1) {
		/* Wait for a key press or reset IRQ to trigger. */
		while (uart_read(CONSOLE_UART, &uc_key) && (!reset_interrupt_triggered));
		
		//! [reset_irq_has_triggered]
		if (reset_interrupt_triggered) {
			/* Critical section to access a variable that is set in an IRQ. */
			cpu_irq_enter_critical();
			reset_interrupt_triggered = false;
			cpu_irq_leave_critical();
			
			puts("User Reset IRQ triggered. Press any console key for the menu\r");
		}
		//! [reset_irq_has_triggered]
		else {
			/* Act on the key press. */
			switch (uc_key - '0') {
			case 0: /* Display reset information. */
				display_reset_reason();
			break;
			
			case 1:	/* Enable User Reset. */
				//! [reset_enable_user_reset]
				rstc_enable_user_reset(RSTC);
				//! [reset_enable_user_reset]
				puts("User Reset enabled - Press evaluation kit RESET button to test.\r");
			break;
		
			case 2:	/* Disable User Reset. */
				//! [reset_disable_user_reset]
				rstc_disable_user_reset(RSTC);
				//! [reset_disable_user_reset]
				puts("User Reset disabled  - Press evaluation kit RESET button to test.\r");
			break;
	
			case 3: /* User Reset interrupt enable. */
				reset_interrupt_triggered = false;
			
				/* Configure and enable WDT interrupt. */
				//! [reset_enable_user_reset_interrupt]
				rstc_disable_user_reset(RSTC);
				NVIC_DisableIRQ(RSTC_IRQn);
				rstc_enable_user_reset_interrupt(RSTC);
				NVIC_ClearPendingIRQ(RSTC_IRQn);
				NVIC_SetPriority(RSTC_IRQn, 0);
				NVIC_EnableIRQ(RSTC_IRQn);
				//! [reset_enable_user_reset_interrupt]
			
				puts("User Reset interrupt enabled.\r");
			break;

			case 4: /* User Reset interrupt disable. */
				//! [reset_disable_user_reset_interrupt]
				NVIC_DisableIRQ(RSTC_IRQn);
				rstc_disable_user_reset_interrupt(RSTC);
				rstc_enable_user_reset(RSTC);
				//! [reset_disable_user_reset_interrupt]
				
				puts("User Reset interrupt disabled.\r");
			break;
		
			case 5:	/* Software Reset. */
				puts("Software Reset activated.\r");				
				wait_for_message_gone();
				
				rstc_start_software_reset(RSTC);
				
				/* Spin. */
				while (1);
			break;
		
			case 6:	/* Watchdog Reset. */
				configure_watchdog();
				
				/* Stop tickling the watchdog. */
				watchdog_enabled = false;
				
				puts("Watchdog Reset will trigger in 3 seconds.\r\n\r");
				
				/* Spin. */
				while (1);
			break;
		
			case 7:	/* NRST assert. */
				puts("NRST asserted.\r");
				
				//! [reset_assert_nrst]
				rstc_set_external_reset(RSTC, 0);	/* 60us */
				rstc_reset_extern(RSTC);
				//! [reset_assert_nrst]
			break;
		
			default:
				puts("Invalid menu option!\r");
			}	
			
		/* Display the menu. */
		display_menu();
		}
	}
}

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond