/**
 * \file
 *
 * \brief Metering Application Emulator (APPEMU) for ATMEL PRIME v1.3 Base Node
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
 *  \mainpage ATMEL PRIME Metering Application Emulator (APPEMU) for Base Node
 *
 *  \section Purpose
 *
 *  This application provides the base node with metering emulation
 * capabilities.
 *
 *  \section Requirements
 *
 *  This package should be used with SAM4CP16BMB board, on which there is
 * dedicated PLC hardware.
 *
 *  \section Description
 *
 *  This application will configure the PRIME stack and its serial interface to
 * use PHY, MAC and IEC_432 layers as Base Node. It will also implement a 
 * metering application emulator that will interact with the respective 
 * APPEMU-equipped base node.
 *
 *  \section Usage
 *
 *  -# Build the program and download it into the evaluation board. Please
 *     refer to the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *     SAM-BA User Guide</a> or the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *     GNU-Based Software Development</a>
 *     application note depending on the solutions that users choose.
 *     \endcode
 *  -# The application will start PRIME standard as Base Node mode.
 *	-# The application is configured to serialize several protocols with
 * these settings:
 *    - Uart0 Serial port
 *	  - 115200 bauds
 *    - TX buffer: 1024 bytes, Rx buffer: 1024 bytes
 *    - Serialized protocols in use:
 *              - Prime Management
 *              - Prime MLME
 *              - Prime PLME
 *              - Prime IEC-432
 *
 */

/* Atmel library includes. */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "asf.h"

/* Atmel boards includes. */
#include "board.h"

/* Configuration includes. */
#include "conf_example.h"

/* AppEmu includes */
#include "app_emu.h"
#include "conf_app_emu.h"

#define STRING_EOL    "\r"
#define STRING_HEADER \
	"-- ATMEL Prime Base Node Metering Application Emulator --\r\n"	\
	"-- "BOARD_NAME " --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/* FreeRTOS utils */
void vApplicationIdleHook( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( xTaskHandle pxTask,
		                            signed char *pcTaskName );
void vApplicationTickHook( void );

/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is
	 * a hook function that will get called if a call to pvPortMalloc() fails.
     * pvPortMalloc() is called internally by the kernel whenever a task,
	 * queue,timer or semaphore is created.  It is also called by various parts of
	 * the demo application.  If heap_1.c or heap_2.c are used, then the size of
	 * the heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE
	 * in FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be
	 * used to query the size of free heap space that remains (although it does
	 * not provide information on how the remaining heap might be fragmented).
	 * */
	taskDISABLE_INTERRUPTS();
	for (;;) {
		while (1) {
		}
	}
}

/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is
	 * set to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the
	 * idle task.  It is essential that code added to this hook function never
	 * attempts to block in any way (for example, call xQueueReceive() with a block
	 * time specified, or call vTaskDelay()).  If the application makes use of
	 * the vTaskDelete() API function (as this demo application does) then it is
	 * also important that vApplicationIdleHook() is permitted to return to its
	 * calling function, because it is the responsibility of the idle task to clean
	 * up memory allocated by the kernel to any task that has since been
	 * deleted. */
}

/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook( xTaskHandle pxTask,
		signed char *pcTaskName )
{
	(void)pcTaskName;
	(void)pxTask;

	/* Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 * function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for (;;) {
		while (1) {
		}
	}
}

/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	* configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can
	* be added here, but the tick hook is called from an interrupt context, so
	* code must not attempt to block, and only the interrupt safe FreeRTOS
	* API functions can be used (those that end in FromISR()). */
}

/**
 * \brief Configure the hardware.
 */
static void prvSetupHardware(void)
{
#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
	status_code_t status;
#endif

	/* ASF function to setup clocking. */
	sysclk_init();

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_SetPriorityGrouping(__NVIC_PRIO_BITS);

	/* Atmel library function to setup for the evaluation kit being used. */
	board_init();

	/* PLC HAL service initialization */
	hal_init();
	hal_start();

#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
	/* Initialize the C42364A LCD glass component. */
	status = c42364a_init();
	if (status != STATUS_OK) {
		puts("-- LCD Initialization fails! --\r\n");
		while (1) {
		}
	}

	c42364a_set_contrast(15);
	c42364a_clear_all();
	c42364a_show_icon(C42364A_ICON_ATMEL);
	c42364a_show_icon(C42364A_ICON_WLESS);
	c42364a_show_text((const uint8_t *)"BASE  ");
#endif
	/* Init pin of APPEMU enable */
	ioport_set_pin_dir(PIN_APPEMU_GPIO, IOPORT_DIR_INPUT);
}

#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE

/**
 * \internal
 * \brief Function to blink a symbol or led.
 * \note Please see conf_oss file in order to configure the signalling.
 *
 */
static uint8_t _blink_symbol(uint8_t icon_com, uint8_t icon_seg, uint8_t status)
{
	if (!status) {
		c42364a_show_icon(icon_com, icon_seg);
		return true;
	} else {
		c42364a_clear_icon(icon_com, icon_seg);
		return false;
	}
}

#endif

/**
 * \internal
 * \brief Periodic task to provide visual feedback that the system status.
 * \note Please see conf_oss file in order to configure the signalling.
 *
 */
#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
extern uint8_t macPLCState;
extern uint8_t connection432State;
extern uint8_t certificationState;
static uint8_t uc_blink_status;
#endif
static void _prime_signalling(xTimerHandle pxTimer)
{
	UNUSED(pxTimer);
#if BOARD == SAM4CMP_DB
	LED_Toggle(LED4);
#elif BOARD == SAM4CMS_DB
	LED_Toggle(LED4);
#elif BOARD == SAM4C_EK
	LED_Toggle(LED0);
#else
	LED_Toggle(LED0);
#endif

#ifdef EXAMPLE_LCD_SIGNALLING_ENABLE
	if (certificationState == 1) {
		c42364a_show_text((const uint8_t *)"PHYCER");
	} else {
		switch (macPLCState) {
		case 0:                 /* DISCONNECTED */
			uc_blink_status = _blink_symbol(C42364A_ICON_WLESS,
											uc_blink_status);
			c42364a_show_text((const uint8_t *)"BN DIS");
			break;

		case 1:                 /* DETECTION */
			uc_blink_status = _blink_symbol(C42364A_ICON_WLESS,
											uc_blink_status);
			c42364a_show_text((const uint8_t *)"BN BCN");
			break;

		case 2:                 /* REGISTERING */
			uc_blink_status = _blink_symbol(C42364A_ICON_WLESS,
											uc_blink_status);
			c42364a_show_text((const uint8_t *)"BN REQ");
			break;

		case 3:                 /* OPERATIVE */
			c42364a_show_icon(C42364A_ICON_WLESS);
			if (connection432State) {
				c42364a_show_text((const uint8_t *)"BN CON");
			} else {
				c42364a_show_text((const uint8_t *)"BN REG");
			}

			break;

		default:
			break;
		}
	}
#endif
}

/**
 *  Configure UART console.
 */
/* [main_console_configure] */
static void configure_dbg_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONF_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Main code entry point.
 */
int main( void )
{
	xTimerHandle xMonitorTimer;

	/* Prepare the hardware */
	prvSetupHardware();

	/* Init Prime Stack */
	vPrimeStackInitTask();

	/* Configure console */
	configure_dbg_console();
	puts(STRING_HEADER);

	/* Debug port for AppEmu */
	if (!pio_get(PIN_APPEMU_PIO, PIN_APPEMU_TYPE, PIN_APPEMU_MASK)) {
		/* Init AppEmu Application */
		vAppEmuInitTask();
	}

	/* Create timer to monitor tasks execution */
	xMonitorTimer = xTimerCreate(
			(const signed char *const)"Monitor timer",
			SIGNALLING_TIMER_RATE,
			pdTRUE,
			NULL,
			_prime_signalling
			);
	configASSERT(xMonitorTimer);
	xTimerStart(xMonitorTimer, SIGNALLING_TIMER_RATE);

	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	 * line will never be reached. If the following line does execute, then
	 * there was insufficient FreeRTOS heap memory available for the idle
	 * and/or
	 * timer tasks to be created. See the memory management section on the
	 * FreeRTOS web site for more details. */
	for (;;) {
	}
}