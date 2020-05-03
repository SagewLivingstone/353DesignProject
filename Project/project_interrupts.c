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
#include "project_interrupts.h"
#include "io_expander.c"

static volatile uint16_t PS2_X_DATA = 0;
static volatile uint16_t PS2_Y_DATA = 0;
static volatile PS2_DIR_t PS2_DIR = PS2_DIR_CENTER;

static const uint16_t PS2_UPPER_VAL = 2978; // 2.40V value
static const uint16_t PS2_LOWER_VAL = 1055; // 0.85V value


//*****************************************************************************
// Returns the most current direction that was pressed.
//*****************************************************************************
PS2_DIR_t ps2_get_direction(void)
{
	// Check PS2 data against thresholds
	if(PS2_X_DATA > PS2_UPPER_VAL)
	{
		return PS2_DIR_LEFT;
	}
	if(PS2_X_DATA < PS2_LOWER_VAL)
	{
		return PS2_DIR_RIGHT;
	}
	if(PS2_Y_DATA > PS2_UPPER_VAL)
	{
		return PS2_DIR_UP;
	}
	if(PS2_Y_DATA < PS2_LOWER_VAL)
	{
		return PS2_DIR_DOWN;
	}
	return PS2_DIR_CENTER;
}


void TIMER1A_Handler(void)
{
	// Clear the interrupt
	TIMER1->ICR |= TIMER_ICR_TATOCINT;
}

void TIMER2A_Handler(void)
{
	// UPDATE GAME STATE
	
	// Clear the interrupt
	TIMER1->ICR |= TIMER_ICR_TATOCINT;
}

void TIMER4A_Handler(void)
{
	ADC0->PSSI |= ADC_PSSI_SS2;
	
	// Clear the interrupt
	TIMER4->ICR |= TIMER_ICR_TATOCINT; 
}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{
	PS2_X_DATA = ADC0->SSFIFO2;
	PS2_Y_DATA = ADC0->SSFIFO2;
	PS2_DIR = ps2_get_direction();
	
  // Clear the interrupt
  ADC0->ISC |= ADC_ISC_IN2;
}

