/*****************************************************************************
 *
 * \file
 *
 * \brief Graphical User Interface.
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
 *****************************************************************************/
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <stdio.h>
#include "board.h"
#include "gui.h"
#include "gpio.h"
#include "delay.h"
#include "et024006dhu.h"


/*! flag set when joystick display starts to signal main function to clear this display */
unsigned short display = 0;

static void gui_print( TString textToDisplay )
{
  int	i = 0;
  boolean strError = false;

  while ( textToDisplay[i] != 0 )
  {
    // Maximum length for display, adjust in engine
    if ( i == 19 )
    {
      strError = true;
      break;
    }
    i++;
  }

  et024006_PrintString("                                          ", (const unsigned char *)&FONT6x8, 80, 80, WHITE, -1);
  if (!strError )
  {
    et024006_PrintString((char*)textToDisplay, (const unsigned char *)&FONT6x8, 80, 80, WHITE, -1);

  }
  else
  {
    et024006_PrintString("str err" , (const unsigned char *)&FONT6x8, 80, 80, WHITE, -1);

  }
}

/*!
 * \brief function to display default message
 */
static void gui_print_default_message(void)
{
  // Display default message.
  et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );
  et024006_PrintString("ATMEL AVR UC3", (const unsigned char *)&FONT6x8, 80, 50, WHITE, -1);
  et024006_PrintString("Cybercom Bluetooth", (const unsigned char *)&FONT6x8, 80, 60, WHITE, -1);
  et024006_PrintString("    SPP Lite Demo  ", (const unsigned char *)&FONT6x8, 80, 70, WHITE, -1);
}

/*!
 * \brief function to configure LCD
 */
void gui_init(U32 fcpu_hz, U32 fhsb_hz, U32 fpbb_hz, U32 fpba_hz)
{

  delay_init(fcpu_hz);
  et024006_Init( fcpu_hz, fhsb_hz );
  gpio_set_gpio_pin(ET024006DHU_BL_PIN);
  et024006_DrawFilledRect(0 , 0, ET024006_WIDTH, ET024006_HEIGHT, BLACK );

  gui_print_default_message();
}

void gui_callback( TString textToDisplay )
{
  display = 1;
  gui_print_default_message();
  gui_print( textToDisplay );
}
