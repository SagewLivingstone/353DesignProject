// Copyright (c) 2015-16, Joe Krachey
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
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "gpio_port.h"
#include "lcd.h"
#include "lcd_images.h"
#include "timers.h"
#include "ps2.h"
#include "launchpad_io.h"
#include "serial_debug.h"
#include "ft6x06.h"
#include "lcd_images.h"
#include "io_expander.h"

#include "project_interrupts.h"
#include "project_hardware_init.h"

#define TRAIL_MAX_SIZE 1000

typedef enum{
  PS2_DIR_UP,
  PS2_DIR_DOWN,
  PS2_DIR_LEFT,
  PS2_DIR_RIGHT,
  PS2_DIR_CENTER,
  PS2_DIR_INIT,
} PS2_DIR_t;

typedef struct
{
	uint32_t x;
	uint32_t y;
} trail_t;

typedef struct
{
	// Vector
	PS2_DIR_t direction;
	uint32_t x;
	uint32_t y;
	// Bitmap
	const uint8_t* bitmap;
	uint32_t width;
	uint32_t height;
	uint32_t fColor;
	uint32_t bColor;
	uint32_t front_x;
	uint32_t front_y;
	// Trail
	trail_t trail[TRAIL_MAX_SIZE];
	uint32_t trail_index;
	
} player_t;

void update_p1(void);
void update_p2(void);

void move_player(player_t* player);
void set_player_direction(player_t* player, PS2_DIR_t dir);

// Trail
void add_trail(player_t* player);
void draw_trail(player_t* player);

// Collision
void calc_player_bounds(player_t* player);
bool check_collision(player_t* player, player_t* ref);
bool check_world_collision(player_t* player);
bool check_trail_collision(player_t* player, player_t* ref);

void player1_input(PS2_DIR_t input);

void pause_pressed();
void end_game();


#endif
