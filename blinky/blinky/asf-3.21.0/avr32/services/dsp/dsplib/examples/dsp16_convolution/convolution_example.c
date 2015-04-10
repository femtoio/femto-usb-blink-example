/*****************************************************************************
 *
 * \file
 *
 * \brief 16 bits Convolution
 *
 * This file show a way to use the 16 bits Convolution function and print the output signal
 * through the USART.
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
 ******************************************************************************/

/*! \mainpage
 * \section intro Introduction
 * This example demonstrates how to use the 16-bit convolution function.
 * To visualize the result, you may use a terminal configured as described in the section <a href="#configinfo">Configuration Information</a>.\n
 * Through this page, you can access the \ref group_dsplib documentation.
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC for 32-bit AVRs and for IAR Embedded Workbench
 * for Atmel 32-bit AVRs. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All 32-bit AVR devices with a USART module can be used. This example has been tested
 * with the following setup:<BR>
 * - EVK1100 evaluation kit,
 * - EVK1101 evaluation kit.
 *
 * \section configinfo Configuration Information
 * This example has been tested with the following configuration:
 * - CPU clock: 12 MHz;
 * - USART1 connected to a PC serial port via a standard RS232 DB9 cable;
 * - PC terminal settings:
 *   - 9600 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <avr32/io.h>

#include "compiler.h"
#include "board.h"

#include "dsp.h"
#include "dsp_debug.h"

#include "pm.h"
#include "cycle_counter.h"

//! The size of the input signal
#define VECT2_SIZE  64
//! The number of tap
#define VECT3_SIZE  25

//! The output buffer
A_ALIGNED dsp16_t vect1[VECT2_SIZE + 2*(VECT3_SIZE - 1) + 4];  // <- "+4" for avr32-uc3 optimized FIR version

//! First input signal
A_ALIGNED dsp16_t vect2[VECT2_SIZE] = {
  DSP16_Q(0.000000),
  DSP16_Q(0.059717),
  DSP16_Q(0.101397),
  DSP16_Q(0.110140),
  DSP16_Q(0.076849),
  DSP16_Q(-0.000000),
  DSP16_Q(-0.113757),
  DSP16_Q(-0.250267),
  DSP16_Q(-0.389747),
  DSP16_Q(-0.509602),
  DSP16_Q(-0.587785),
  DSP16_Q(-0.606226),
  DSP16_Q(-0.553810),
  DSP16_Q(-0.428477),
  DSP16_Q(-0.238102),
  DSP16_Q(0.000000),
  DSP16_Q(0.260912),
  DSP16_Q(0.515080),
  DSP16_Q(0.732020),
  DSP16_Q(0.884270),
  DSP16_Q(0.951057),
  DSP16_Q(0.921177),
  DSP16_Q(0.794687),
  DSP16_Q(0.583150),
  DSP16_Q(0.308407),
  DSP16_Q(-0.000000),
  DSP16_Q(-0.308407),
  DSP16_Q(-0.583150),
  DSP16_Q(-0.794687),
  DSP16_Q(-0.921177),
  DSP16_Q(-0.951057),
  DSP16_Q(-0.884270),
  DSP16_Q(-0.732020),
  DSP16_Q(-0.515080),
  DSP16_Q(-0.260912),
  DSP16_Q(0.000000),
  DSP16_Q(0.238102),
  DSP16_Q(0.428477),
  DSP16_Q(0.553810),
  DSP16_Q(0.606226),
  DSP16_Q(0.587785),
  DSP16_Q(0.509602),
  DSP16_Q(0.389747),
  DSP16_Q(0.250267),
  DSP16_Q(0.113757),
  DSP16_Q(-0.000000),
  DSP16_Q(-0.076849),
  DSP16_Q(-0.110140),
  DSP16_Q(-0.101397),
  DSP16_Q(-0.059717),
  DSP16_Q(0.000000),
  DSP16_Q(0.059717),
  DSP16_Q(0.101397),
  DSP16_Q(0.110140),
  DSP16_Q(0.076849),
  DSP16_Q(-0.000000),
  DSP16_Q(-0.113757),
  DSP16_Q(-0.250267),
  DSP16_Q(-0.389747),
  DSP16_Q(-0.509602),
  DSP16_Q(-0.587785),
  DSP16_Q(-0.606226),
  DSP16_Q(-0.553810),
  DSP16_Q(-0.428477)
};

//! Second input signal
A_ALIGNED dsp16_t vect3[VECT3_SIZE] = {
  DSP16_Q(0.015591),
  DSP16_Q(0.008942),
  DSP16_Q(-0.),
  DSP16_Q(-0.010929),
  DSP16_Q(-0.023387),
  DSP16_Q(-0.036788),
  DSP16_Q(-0.050455),
  DSP16_Q(-0.063661),
  DSP16_Q(-0.075682),
  DSP16_Q(-0.085839),
  DSP16_Q(-0.093548),
  DSP16_Q(-0.098363),
  DSP16_Q(0.1),
  DSP16_Q(0.098363),
  DSP16_Q(0.093548),
  DSP16_Q(0.085839),
  DSP16_Q(0.075682),
  DSP16_Q(0.063661),
  DSP16_Q(0.050455),
  DSP16_Q(0.036788),
  DSP16_Q(0.023387),
  DSP16_Q(0.010929),
  DSP16_Q(0.),
  DSP16_Q(-0.008942),
  DSP16_Q(-0.015591)
};


//! The main function
int main(int argc, char *argv[])
{
  unsigned int cycle_count;

  // Switch to external Oscillator 0.
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

  // Initialize the DSP debug module
  dsp_debug_initialization(FOSC0);

  // Get the actual cycle count
  cycle_count = Get_sys_count();

  // Perform a convolution
  dsp16_vect_conv(vect1, vect2, VECT2_SIZE, vect3, VECT3_SIZE);

  // Calculate the number of cycles
  cycle_count = Get_sys_count() - cycle_count;

  // Print the number of cycles
  dsp16_debug_printf("Cycle count: %d\n", cycle_count);
  // Print the  output signal
  dsp16_debug_print_vect(vect1, VECT2_SIZE + VECT3_SIZE - 1);

  while(1);
}


