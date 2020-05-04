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

static volatile uint16_t PS2_X_DATA = 0;
static volatile uint16_t PS2_Y_DATA = 0;
static volatile PS2_DIR_t PS2_DIR = PS2_DIR_CENTER;

static const uint16_t PS2_UPPER_VAL = 2978; // 2.40V value
static const uint16_t PS2_LOWER_VAL = 1055; // 0.85V value
static volatile uint16_t duty_cycle = 70;

extern uint16_t game_time;

volatile bool pressed = true;
volatile uint8_t BTN_DIR;

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
	static bool count_up = true;
	static uint8_t timer_dec = 0;
	if(!timer_dec)
	{
		game_time++;
	}
	timer_dec = (timer_dec + 1) % 10;
	
	if(count_up)
	{
		if(duty_cycle >= DUTY_CYCLE_MAX)
		{
			count_up = false;
		}
		else
		{
			duty_cycle += DUTY_CYCLE_INTERVAL;
		}
	}
	else
	{
		if(duty_cycle <= 0)
		{
			count_up = true;
		}
		else
		{
			duty_cycle -= DUTY_CYCLE_INTERVAL;
		}
	}
	
	// Clear the interrupt
	TIMER1->ICR |= TIMER_ICR_TATOCINT;
}

void TIMER2A_Handler(void)
{
	// Update game state at ~50hz
	update_p1();
	update_p2();
	
	// Clear the interrupt
	TIMER2->ICR |= TIMER_ICR_TATOCINT;
}

void TIMER3A_Handler(void)
{
	static bool led_on = false;
	if(led_on)
	{
		lp_io_clear_pin(BLUE_BIT);
		led_on = false;
		gp_timer_config_32(TIMER3_BASE,TIMER_TAMR_TAMR_1_SHOT, 5000 * (100 - duty_cycle), false, true); // Status LED Timer
	}
	else
	{
		lp_io_set_pin(BLUE_BIT);
		led_on = true;
		gp_timer_config_32(TIMER3_BASE,TIMER_TAMR_TAMR_1_SHOT, 5000 * (duty_cycle), false, true); // Status LED Timer
	}
	
	// Clear the interrupt
	TIMER3->ICR |= TIMER_ICR_TATOCINT;
}

// Timer for setting of PS2 ADC
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
	
	player1_input(PS2_DIR);
	
  // Clear the interrupt
  ADC0->ISC |= ADC_ISC_IN2;
}

PS2_DIR_t btn_dir_to_ps2_dir(PS2_DIR_t dir)
{
	if(dir == DIR_BTN_LEFT_PIN)
	{
		return PS2_DIR_LEFT;
	}
	if(dir == DIR_BTN_RIGHT_PIN)
	{
		return PS2_DIR_RIGHT;
	}
	if(dir == DIR_BTN_UP_PIN)
	{
		return PS2_DIR_UP;
	}
	if(dir == DIR_BTN_DOWN_PIN)
	{
		return PS2_DIR_DOWN;
	}
	return PS2_DIR_CENTER;
}

void GPIOF_Handler(void)
{
		uint8_t data, gpio_data;
		GPIOA_Type *gpioPort;
	
		gpioPort = (GPIOA_Type *)IO_EXPANDER_IRQ_GPIO_BASE;
		data = io_expander_read_reg(MCP23017_GPIOB_R);
		
		if(~data & 0x8){
			BTN_DIR = DIR_BTN_RIGHT_PIN;
		}
		
		else if(~data & 0x4){
			BTN_DIR = DIR_BTN_LEFT_PIN;
		}
		
		else if(~data & 0x2){
			BTN_DIR = DIR_BTN_DOWN_PIN;
		}
		
		else if(~data & 0x1){
			BTN_DIR = DIR_BTN_UP_PIN;
		}
		
		if(data != 0xf) // "Debounce" by disallowing before release
		{
			player2_input(btn_dir_to_ps2_dir(BTN_DIR));
		}
		
		io_expander_read_reg(MCP23017_GPIOB_R);
		gpioPort->ICR |= IO_EXPANDER_IRQ_PIN_NUM;
}	

