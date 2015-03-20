/**
 * \file
 *
 * \brief Audio codec task.
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

#include <avr32/io.h>
#include "compiler.h"
#include "board.h"
#include "com_task.h"
#include "audio_interface.h"
#include "audio_mixer.h"
#include "wav_player.h"

extern Ai_player_flag_t g_player_flag;
extern bool g_player_flag_run_wav;
extern bool g_player_flag_error;

void codec_wav_task(void);

/*! \brief codec task.
 */
void codec_wav_task(void)
{
	if (g_player_flag_run_wav) {
		g_player_flag_error = !play_wav(NULL, false);   // Start playing MP3.
		// Here, the player is stopped.
		g_player_flag_run_wav = false;
		if (g_player_flag.status != PLAYER_FLAG_STOP) { // No STOP command.
			// Makes sure the pending commands are done.
			while (!is_ai_async_cmd_finished()) {
				AI_SYNC_TASK_CALL_BACK();
				ai_async_cmd_task();
			}
			// It is a end of file
#if (PAUSE_ON_TRACK_CHANGED == ENABLE)
			ai_audio_nav_eof_occur();
#else
			if (ai_audio_nav_eof_occur()) {
				ai_audio_ctrl_resume();
			}
#endif
		}
	}
}
