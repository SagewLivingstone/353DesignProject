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

player_t player1;
player_t player2;

bool game_over;
bool kill = false;

void init_game()
{
	lcd_clear_screen(LCD_COLOR_BLACK);
	
	// Setup player 1
	player1.x = 30;
	player1.y = 30;
	player1.fColor = LCD_COLOR_CYAN;
	player1.bColor = LCD_COLOR_BLACK;
	set_player_direction(&player1, PS2_DIR_RIGHT);
	
	// Setup player 2
	//...
	printf("Game beginning\n\r");
	game_over = false;
}

void game_loop()
{
	uint16_t trail_decimator = 0;
	
	init_game();
	
	while(!kill && !game_over)
	{
		if(!trail_decimator)
		{
			draw_trail(&player1);
			// draw p2 too
		}
		trail_decimator = (trail_decimator + 1) % 8; // Only redraw trail every 8 frames
		
		calc_player_bounds(&player1);
		// Draw at max speed
		lcd_draw_image(player1.x, 
								player1.width, 
								player1.y, 
								player1.height, 
								player1.bitmap, 
								player1.fColor, 
								player1.bColor);
	}
}

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  
  DisableInterrupts();
  init_serial_debug(true, true);
	init_hardware();
  EnableInterrupts();
}
//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	initializeBoard();
	
	printf("****************************\n\r");
	printf("**** Light Bike Game  ******\n\r");
	printf("****      by:         ******\n\r");
	printf("**** Sage Livingstone ******\n\r");
	printf("****   and Max Klug   ******\n\r");
	printf("****************************\n\r");
	
	game_loop();
	
	while(1){};
}

void move_player(player_t* player)
{
	switch(player->direction)
	{
		case PS2_DIR_UP:
			player->y -= 1;
			break;
		case PS2_DIR_DOWN:
			player->y += 1;
			break;
		case PS2_DIR_LEFT:
			player->x -= 1;
			break;
		case PS2_DIR_RIGHT:
			player->x += 1;
			break;
		default: break;
	}
}

// Takes in a player pointer, calculates its image boundaries and such from direction
// Always call this before drawing the player
// The "inputs" are the direction and x,y of the player, it then updates the remainig
//   player variables
void calc_player_bounds(player_t* player)
{
	switch(player->direction)
	{
		case PS2_DIR_UP:
			player->front_x = player->x;
			player->front_y = player->y - (player->height / 2);
			break;
		case PS2_DIR_DOWN:
			player->front_x = player->x;
			player->front_y = player->y + (player->height / 2);
			break;
		case PS2_DIR_LEFT:
			player->front_x = player->x - (player->width / 2);
			player->front_y = player->y;
			break;
		case PS2_DIR_RIGHT:
			player->front_x = player->x + (player->width / 2);
			player->front_y = player->y;
			break;
		default: break;
	}
}

void set_player_direction(player_t* player, PS2_DIR_t dir)
{
	player->direction = dir;
	// Set bitmap from direction
	//   and set width and height
	switch(dir)
	{
		case PS2_DIR_UP:
		{
			player->bitmap = lightbike_up;
			player->width  = lightbike_up_width;
			player->height = lightbike_up_height;
			break;
		}
		case PS2_DIR_DOWN:
		{
			player->bitmap = lightbike_down;
			player->width  = lightbike_down_width;
			player->height = lightbike_down_height;
			break;
		}
		case PS2_DIR_LEFT:
		{
			player->bitmap = lightbike_left;
			player->width  = lightbike_left_width;
			player->height = lightbike_left_height;
			break;
		}
		case PS2_DIR_RIGHT:
		{
			player->bitmap = lightbike_right;
			player->width  = lightbike_right_width;
			player->height = lightbike_right_height;
			break;
		}
		default: break;
	}
	
	calc_player_bounds(player);
}

void player1_input(PS2_DIR_t input)
{
	if(input == PS2_DIR_CENTER) return;
	if(player1.direction == input) return;
	set_player_direction(&player1, input);
}

void add_trail(player_t* player)
{
	player->trail[player->trail_index].x = player->x;
	player->trail[player->trail_index].y = player->y;
	player->trail_index = (player->trail_index + 1) % TRAIL_MAX_SIZE; // Wrap back to beginning of loop
}

void draw_trail(player_t* player)
{
	int i;
	for(i = 0; i < TRAIL_MAX_SIZE; i++)
	{
		if(player->trail[i].x == 0 && player->trail[i].y == 0) continue;
		lcd_draw_image(player->trail[i].x,
										trail_width,
										player->trail[i].y,
										trail_height,
										trail_bitmap,
										player->fColor,
										player->bColor);
	}
}

bool check_collision(player_t* player, player_t* ref)
{
	calc_player_bounds(player);
	if(check_world_collision(player))
	{
		return true;
	}
	if(check_trail_collision(player, ref))
	{
		//printf("Collision\n\r");
		return true;
	}
	return false;
}

bool check_world_collision(player_t* player)
{
	// Check if player hit edges of screen
	if(player->front_x <= 0 ||
		player->front_x >= COLS ||
		player->front_y <= 0 ||
		player->front_y >= ROWS)
	{
		return true;
	}
	return false;
}

bool check_trail_collision(player_t* player, player_t* ref)
{
	int i;
	for(i = 0; i < TRAIL_MAX_SIZE; i++)
	{
		if(ref->trail[i].x == 0 && ref->trail[i].y == 0) continue;
		if(player->front_x > ref->trail[i].x - 2 &&
			player->front_x < ref->trail[i].x + 2 &&
			player->front_y > ref->trail[i].y - 2 &&
			player->front_y < ref->trail[i].y + 2)
		{
			printf("Collided with %d %d\n\r", ref->trail[i].y, ref->trail[i].y);
			return true;
		}
	}
	return false;
}


void update_p1()
{
	static uint32_t trail_decimator = 0;
	
	// Add trail piece
	if(!trail_decimator)
		add_trail(&player1);
	trail_decimator = (trail_decimator + 1) % 4;
	
	move_player(&player1);
	if(check_collision(&player1, &player2))
	{
		printf("Collision!\n\r");
		game_over = true;
	}
	// Check_Collision...
}
