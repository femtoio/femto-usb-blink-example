/**
 * \file
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

#ifndef _SAM4CP_UART1_INSTANCE_
#define _SAM4CP_UART1_INSTANCE_

/* ========== Register definition for UART1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_UART1_CR                    (0x48004000U) /**< \brief (UART1) Control Register */
  #define REG_UART1_MR                    (0x48004004U) /**< \brief (UART1) Mode Register */
  #define REG_UART1_IER                   (0x48004008U) /**< \brief (UART1) Interrupt Enable Register */
  #define REG_UART1_IDR                   (0x4800400CU) /**< \brief (UART1) Interrupt Disable Register */
  #define REG_UART1_IMR                   (0x48004010U) /**< \brief (UART1) Interrupt Mask Register */
  #define REG_UART1_SR                    (0x48004014U) /**< \brief (UART1) Status Register */
  #define REG_UART1_RHR                   (0x48004018U) /**< \brief (UART1) Receive Holding Register */
  #define REG_UART1_THR                   (0x4800401CU) /**< \brief (UART1) Transmit Holding Register */
  #define REG_UART1_BRGR                  (0x48004020U) /**< \brief (UART1) Baud Rate Generator Register */
  #define REG_UART1_RPR                   (0x48004100U) /**< \brief (UART1) Receive Pointer Register */
  #define REG_UART1_RCR                   (0x48004104U) /**< \brief (UART1) Receive Counter Register */
  #define REG_UART1_TPR                   (0x48004108U) /**< \brief (UART1) Transmit Pointer Register */
  #define REG_UART1_TCR                   (0x4800410CU) /**< \brief (UART1) Transmit Counter Register */
  #define REG_UART1_RNPR                  (0x48004110U) /**< \brief (UART1) Receive Next Pointer Register */
  #define REG_UART1_RNCR                  (0x48004114U) /**< \brief (UART1) Receive Next Counter Register */
  #define REG_UART1_TNPR                  (0x48004118U) /**< \brief (UART1) Transmit Next Pointer Register */
  #define REG_UART1_TNCR                  (0x4800411CU) /**< \brief (UART1) Transmit Next Counter Register */
  #define REG_UART1_PTCR                  (0x48004120U) /**< \brief (UART1) Transfer Control Register */
  #define REG_UART1_PTSR                  (0x48004124U) /**< \brief (UART1) Transfer Status Register */
#else
  #define REG_UART1_CR   (*(__O  uint32_t*)0x48004000U) /**< \brief (UART1) Control Register */
  #define REG_UART1_MR   (*(__IO uint32_t*)0x48004004U) /**< \brief (UART1) Mode Register */
  #define REG_UART1_IER  (*(__O  uint32_t*)0x48004008U) /**< \brief (UART1) Interrupt Enable Register */
  #define REG_UART1_IDR  (*(__O  uint32_t*)0x4800400CU) /**< \brief (UART1) Interrupt Disable Register */
  #define REG_UART1_IMR  (*(__I  uint32_t*)0x48004010U) /**< \brief (UART1) Interrupt Mask Register */
  #define REG_UART1_SR   (*(__I  uint32_t*)0x48004014U) /**< \brief (UART1) Status Register */
  #define REG_UART1_RHR  (*(__I  uint32_t*)0x48004018U) /**< \brief (UART1) Receive Holding Register */
  #define REG_UART1_THR  (*(__O  uint32_t*)0x4800401CU) /**< \brief (UART1) Transmit Holding Register */
  #define REG_UART1_BRGR (*(__IO uint32_t*)0x48004020U) /**< \brief (UART1) Baud Rate Generator Register */
  #define REG_UART1_RPR  (*(__IO uint32_t*)0x48004100U) /**< \brief (UART1) Receive Pointer Register */
  #define REG_UART1_RCR  (*(__IO uint32_t*)0x48004104U) /**< \brief (UART1) Receive Counter Register */
  #define REG_UART1_TPR  (*(__IO uint32_t*)0x48004108U) /**< \brief (UART1) Transmit Pointer Register */
  #define REG_UART1_TCR  (*(__IO uint32_t*)0x4800410CU) /**< \brief (UART1) Transmit Counter Register */
  #define REG_UART1_RNPR (*(__IO uint32_t*)0x48004110U) /**< \brief (UART1) Receive Next Pointer Register */
  #define REG_UART1_RNCR (*(__IO uint32_t*)0x48004114U) /**< \brief (UART1) Receive Next Counter Register */
  #define REG_UART1_TNPR (*(__IO uint32_t*)0x48004118U) /**< \brief (UART1) Transmit Next Pointer Register */
  #define REG_UART1_TNCR (*(__IO uint32_t*)0x4800411CU) /**< \brief (UART1) Transmit Next Counter Register */
  #define REG_UART1_PTCR (*(__O  uint32_t*)0x48004120U) /**< \brief (UART1) Transfer Control Register */
  #define REG_UART1_PTSR (*(__I  uint32_t*)0x48004124U) /**< \brief (UART1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4CP_UART1_INSTANCE_ */
