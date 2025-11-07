#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include <EzGraph.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

extern int stage[F_HEIGHT][F_WIDTH];
extern int field[F_HEIGHT][F_WIDTH];
extern int turn_block[B_HEIGHT][B_WIDTH];
extern int block[B_HEIGHT][B_WIDTH];
extern int clear_line_point[F_HEIGHT-WALL_BOTTOM-1];
extern int ran[7];

extern int hold_id;
extern int up_key_flag;
extern int IPS;
extern int block_x;
extern int block_y;
extern int block_id;
extern int next_block_id;
extern int gameover_flag;
extern int make_block_flag;
extern int hold_null_flag;
extern int bullet_flag;
extern int self_x, self_y, enemy_x, enemy_y; //
extern int id;
extern int ren, ren_check;
extern int perfect_check;
extern int HP;

extern int help1, help2, help3;
extern int IPS_temp;

extern type_result result;
extern clock_t t1, t2;
extern bullet_t my_bullet[4]; //

extern EzImage self, enemy;
extern EzImage Wall_image,IMI,OMI,TMI,SMI,ZMI,LMI,JMI; // graphic chuyen sang
extern clock_t t1;
extern clock_t t_enemy1;
extern char na[15];
clock_t t_ready1, t_ready2;

int k = 0;
char na[15] = "\0";

extern int menu_key;

void ready(void){
  int time;
  t_ready2 = clock();

  time = 1000.0 * (t_ready2 - t_ready1) / CLOCKS_PER_SEC;

  if (time < 100)
	draw_ready(3);
  else if (time < 200)
	draw_ready(2);
  else if (time < 300)
	draw_ready(1);
  else if (time < 400)
	draw_ready(4);
  else if (time < 500)
	draw_ready(5);
  else if (time >= 500){
	  EzSetKeyHandler(key_handler);
	  EzSetTimerHandler(show_game, IPS);
  }
}

void key_menu(int key){
  switch(key){
	
  case EZ_Up:
	if (menu_key < 2){
	  menu_key = 4;
	  draw_menu();
	  EzShowBuffer();
	}
	else{
	  menu_key--;
	  draw_menu();
	  EzShowBuffer();
	}
	break;

  case EZ_Down:
	if (menu_key < 4){
	  menu_key++;
	  draw_menu();
	  EzShowBuffer();
	}
	else{
	  menu_key = 1;
	  draw_menu();
	  EzShowBuffer();
	}
	break;

  case 13:
	switch(menu_key){
	case 1: // New game
	  na[0] = '\0';
	  k = 0;
	  EzSetKeyHandler(key_name);
	  draw_name();
	  EzShowBuffer();	  
	  break;

	case 2: // Continue
	  t_ready1 = clock();
	  if (stage[0][0] != 0 && gameover_flag == 0){
		EzSetKeyHandler(NULL);
		EzSetTimerHandler(ready, IPS);
	  }
	  else{
		init_var();
		t_enemy1 = 0;
		t1 = 0;
		stage_read();
		var_read();
		
		if (gameover_flag == 1){
		  EzSetKeyHandler(key_gameover);
		  draw_field();
		  draw_gameover();
		  EzShowBuffer();
		} else if (gameover_flag == 2){
		  EzSetKeyHandler(key_gameover);
		  draw_field();
		  draw_game_victory();
		  EzShowBuffer();
		} else {
		EzSetKeyHandler(NULL);
		EzSetTimerHandler(ready, IPS);
		}
		
		}
	  break;
	  
	case 3: // Result
	  // doc file
	  EzSetKeyHandler(key_result);
	  draw_result();
	  EzShowBuffer();
	  break;
	  
	case 4: // End
	  EzExitEventLoop();
	  break;
	}
	break;
  }
}

// 7 - 22
void key_name(int n){
  
  switch(n){
  case 13:
	if (na[0] == '\0'){
	  result.name = "NO_NAME";
	  k = 7;
	  strcpy(na, result.name);
	}
	else{
	  result.name = na;
	  init_var();
	  EzSetKeyHandler(key_handler);
	  EzSetTimerHandler(show_game, IPS);
	}
	break;

  case 8:
	if (k > 0){
	  na[k-1] = '\0';
	  k--;
	}
	draw_name();
	EzShowBuffer();
	break;
	
  default:
	if ((isalpha(n) || isdigit(n)) && k < 14){
	  na[k] = n;
	  na[k+1] = '\0';
	  k++;
	}
	
	draw_name();
	EzShowBuffer();
	break;
  }
}

void key_result(int key){
  
  switch(key){

  case 13:
	EzSetKeyHandler(key_menu);
	draw_menu();
	EzShowBuffer();
	break;
	
  default:
	break;
  }
}

void key_handler(int key){
  switch(key){
  case EZ_Left:
	if(collision_left() != 1){
	  block_x--;
	}
	break;
	
  case EZ_Right:
	if(collision_right() != 1){
	  block_x++;
	}
	break;
	
  case EZ_Up:
	if (up_key_flag == 0)
	while(collision_bottom() == 0)
	  block_y++;
	up_key_flag = 1;
	break;

  case EZ_Down:
	if (collision_bottom() == 0)
	  block_y++;
	break;
	
  case 'x':
	roll(block, "clockwise");
	break;
	
  case 'z':
	roll(block, "notclockwise");
	break;
	
  case 'c':
	hold();
	break;
	
  case 'd': // x2
	if (help3)
	  help(3);
	break;

  case 's': // スピード落とす
	if (help2)
	  help(2);
	break;

  case 'a': // プロック交換
	if (help1)
	  help(1);
	break;
	
  case 'p':
	IPS = IPS_temp;
	if (help2 == 2)
	  help2 = 0; // ヘルプが不正に使われることを避ける
	EzSetKeyHandler(key_pause);
	EzSetTimerHandler(NULL, 0);
	draw_game_pause();
	EzShowBuffer();
	break;
	
  case 'q':
	EzExitEventLoop();
	break;
	
  default:
	break;
  }
}
void key_pause(int key){
  if (key == 'y'){
	EzSetKeyHandler(key_handler);
	EzSetTimerHandler(show_game, IPS);	
  }
  if (key == 'n'){
	stage_write();
	var_write();
	EzSetKeyHandler(key_menu);
	draw_menu();
	EzShowBuffer();
  }
}

void key_gameover(int key){
  if (key == 13){
	
	EzSetKeyHandler(key_menu);
	draw_menu();
	EzShowBuffer();
  }	
}



