// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "main.h"

void init_hardware()
{
	printf("Initializing Hardware...\n\r");
	lcd_config_screen();
	lcd_clear_screen(LCD_COLOR_BLACK);
	ps2_initialize();
	ft6x06_init();
	io_expander_init();
	lp_io_init();
	
	gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_1_SHOT, 0, false, false);
	gp_timer_config_32(TIMER1_BASE,TIMER_TAMR_TAMR_PERIOD, 5000000, false, true); // Status LED Timer
	gp_timer_config_32(TIMER3_BASE,TIMER_TAMR_TAMR_1_SHOT, 500000, false, true); // Status LED Timer
	gp_timer_config_32(TIMER2_BASE,TIMER_TAMR_TAMR_PERIOD, 833334, false, true); // Game Tick Timer 60Hz
	gp_timer_config_32(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD, 500000, false, true); // ADC Check Timer
	printf("Initializing Done\n\r");
}

