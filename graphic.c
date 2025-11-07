#include <stdio.h>
#include "library.h"
#include <EzGraph.h>
#include <stdlib.h>
extern int block_x;
extern int block_y;
extern int block_id;
extern int gameover_flag;
extern int make_block_flag;
extern int bullet_flag;
extern bullet_t my_bullet[4];
extern EzImage self, enemy;
extern EzImage Wall_image,IMI,OMI,TMI,SMI,ZMI,LMI,JMI, MENU_BGR;
extern int self_x, self_y, enemy_x, enemy_y; 
extern type_result result; 
extern type_block block_array[7];
extern int block[B_HEIGHT][B_WIDTH];
extern int field[F_HEIGHT][F_WIDTH];
extern int menu_key;
extern int ren, ren_check;
extern int perfect_check;
extern char na[15];
extern int k;
extern int HP;
extern int help1, help2, help3;

void draw_menu(void){
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);

  // Fall Block Battle
  EzSetColorByName("Black");
  //EzDrawBoxB((WALL_LEFT+2)*UNIT, 0.5*(GAME_HEIGHT/5)*UNIT,
  //		 (WALL_LEFT + GAME_WIDTH - 2)*UNIT, 1.5*(GAME_HEIGHT/5)*UNIT);

  EzSetFontSize(60);
  EzDrawStringB(3*UNIT,
				6*UNIT, "Fall Block Battle");

  EzSetColorByName("Yellow");
  EzFillBoxB(12*UNIT, (0.75*menu_key+2.25)*(GAME_HEIGHT/5)*UNIT+UNIT,
			 18*UNIT, (0.75*menu_key+2.75)*(GAME_HEIGHT/5)*UNIT+UNIT);
  
  EzSetColorByName("Black");
  EzSetFontSize(16);
  // Newgame
  EzDrawBoxB(12*UNIT, 3*(GAME_HEIGHT/5)*UNIT + UNIT,
			 18*UNIT, 3.5*(GAME_HEIGHT/5)*UNIT+ UNIT);
  EzDrawStringB(13*UNIT,
				3.25*(GAME_HEIGHT/5)*UNIT+5+UNIT, "New Game");

  // Continue
  EzDrawBoxB(12*UNIT, 3.75*(GAME_HEIGHT/5)*UNIT + UNIT,
			 18*UNIT, 4.25*(GAME_HEIGHT/5)*UNIT+UNIT);
  EzDrawStringB(13*UNIT+5,
				4*(GAME_HEIGHT/5)*UNIT+5+UNIT, "Continue");

  // Result
  EzDrawBoxB(12*UNIT, 4.5*(GAME_HEIGHT/5)*UNIT + UNIT,
			 18*UNIT, 5*(GAME_HEIGHT/5)*UNIT + UNIT);
  EzDrawStringB(14*UNIT - 5,
				4.75*(GAME_HEIGHT/5)*UNIT+5+UNIT, "Record");

  // Exit
  EzDrawBoxB(12*UNIT, 5.25*(GAME_HEIGHT/5)*UNIT + UNIT,
			 18*UNIT, 5.75*(GAME_HEIGHT/5)*UNIT + UNIT);
  EzDrawStringB(14*UNIT,
				5.5*(GAME_HEIGHT/5)*UNIT+5+UNIT, " Exit");
}

void draw_result(void){  
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);

  EzSetFontSize(60);
  EzSetColorByName("Black");
  EzDrawStringB(9*UNIT,	6*UNIT, "RECORD");

  EzSetFontSize(16);

  EzSetColorByName("Yellow");
  EzFillBoxB((WALL_LEFT+1)*UNIT, 6.25*(GAME_HEIGHT/5)*UNIT + UNIT,
			 (WALL_LEFT + GAME_WIDTH - 3)*UNIT, 6.75*(GAME_HEIGHT/5)*UNIT + UNIT);
  
  EzSetColorByName("Black");
  EzDrawBoxB((WALL_LEFT+1)*UNIT, 6.25*(GAME_HEIGHT/5)*UNIT + UNIT,
			 (WALL_LEFT + GAME_WIDTH - 3)*UNIT, 6.75*(GAME_HEIGHT/5)*UNIT + UNIT);
  EzDrawStringB(13*UNIT - 10,
				6.5*(GAME_HEIGHT/5)*UNIT+5+UNIT, "Enter : Menu");
  
  FILE *fp;
  int i, check;
  char ten[10][15];
  int diem[10] = {0};
  
  fp = fopen("result.txt", "r");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
	exit(1);
  }
  
  fscanf(fp, "%d %d %d", &check, &i, &i);
  
  if (check == 1){
	for (i = 0; i < 10; i++){
	  fscanf(fp, "%s %d", ten[i], &diem[i]);
	  if (diem[i] == 0){
		EzDrawStringB(WALL_LEFT*UNIT, (WALL_TOP + i + 3)*UNIT, "N/A");
		EzDrawStringB((WALL_LEFT+7)*UNIT, (WALL_TOP + i + 3)*UNIT, "---");
	  }
	  else{
		char buf[4];
		EzDrawStringB(WALL_LEFT*UNIT, (WALL_TOP + i + 3)*UNIT, ten[i]);
		sprintf(buf, "%3d", diem[i]);
		EzDrawStringB((WALL_LEFT+7)*UNIT, (WALL_TOP + i + 3)*UNIT, buf);
	  }
	}
  }

  fclose(fp);

}


// 7/22

void draw_name(void){
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);
  
  EzSetColorByName("Black");
  EzDrawBoxB(11*UNIT, 13*UNIT + 10,
			 22*UNIT, 16*UNIT);
  
  EzDrawStringB(8*UNIT, 15*UNIT, "Name");
  EzDrawStringB(12*UNIT, 15*UNIT, na);
}


void draw_field(void){

  int i, j;
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);
  // ve tuong, game
  for(i = 0; i < F_HEIGHT; i++){
	for (j = 0; j < F_WIDTH; j++){
	  if (field[i][j] == WALL){
		EzPutB(j*UNIT, i*UNIT,Wall_image);
	  }
	  else if (field[i][j] != 0 && field[i][j] > 0){
		if(block_array[field[i][j] - 1].id==1){
		EzPutB(j*UNIT, i*UNIT, OMI);
		}
		if(block_array[field[i][j] - 1].id==2){
		  EzPutB(j*UNIT, i*UNIT,LMI);
			  }
		if(block_array[field[i][j] - 1].id==3){
		  EzPutB(j*UNIT, i*UNIT,JMI);
			  }
	if(block_array[field[i][j] - 1].id==4){
	  EzPutB(j*UNIT, i*UNIT,IMI);
			  }
		if(block_array[field[i][j] - 1].id==5){
		  EzPutB(j*UNIT, i*UNIT,TMI);
			  }
	if(block_array[field[i][j] - 1].id==6){
	  EzPutB(j*UNIT, i*UNIT,SMI);
			  }
		if(block_array[field[i][j] - 1].id==7){
		  EzPutB(j*UNIT, i*UNIT,ZMI);
			  }
	  }
	}
}
  // help
   char buf[30];
  EzSetColorByName("Black");

  EzDrawStringB((WALL_LEFT + GAME_WIDTH + 3)*UNIT+10, (6+WALL_TOP)*UNIT, "HELP");
   
  if (help1){
	
  sprintf(buf, "A : Change x %d", help1);
  EzDrawStringB((WALL_LEFT + GAME_WIDTH + 2)*UNIT-15, (8+WALL_TOP)*UNIT, buf);
  }

  if(help2)
	EzDrawStringB((WALL_LEFT + GAME_WIDTH + 2)*UNIT-15, (10+WALL_TOP)*UNIT, "S : Slow");

  if(help3)
	EzDrawStringB((WALL_LEFT + GAME_WIDTH + 2)*UNIT-15, (12+WALL_TOP)*UNIT, "D : Double attack");

  // Next Block
  EzDrawStringB((WALL_LEFT + GAME_WIDTH + 2)*UNIT+10, 5*UNIT+15, "Next Block");
  
  // Hold
  EzDrawStringB(5*UNIT, UNIT+15, "C : Hold");

  // Pause
  EzSetColorByName("Yellow");
  EzFillBoxB(25*UNIT, UNIT,
			 29*UNIT, 3*UNIT);
  EzSetColorByName("Black");
  EzDrawStringB(25*UNIT+5, 2*UNIT+5, "P : Pause");
  
  // Info
  EzSetColorByName("Black");
  EzDrawStringB((WALL_LEFT + GAME_WIDTH + 3)*UNIT+5, 22*UNIT+10, "NAME");
  sprintf(buf, "%s", na);
  EzDrawStringB((WALL_LEFT + GAME_WIDTH + 1)*UNIT + (135-9*k)/2, 23*UNIT+10, buf);
  sprintf(buf, "ATTACK : %d", result.attack);
  EzDrawStringB(24*UNIT-14, (20+WALL_TOP)*UNIT, buf);
  
  // ve hinh
  // 敵のHP
  double y = result.enemy_hp / (double)HP;
  if (y <= 0 )
	y = 0;
  
  if (y >= 0.8)
	EzSetColorByName("Green");
  else if (y > 0.3)
	EzSetColorByName("Yellow");
  else
	EzSetColorByName("Red");
  EzFillBoxB(2*UNIT, (WALL_TOP+1)*UNIT + 10, (2+(WALL_LEFT-4)*y)*UNIT, (WALL_TOP+1)*UNIT + 15);

  EzSetColorByName("Black");
  EzDrawBoxB(2*UNIT, (WALL_TOP+1)*UNIT + 10, (WALL_LEFT-2)*UNIT, (WALL_TOP+1)*UNIT + 15);

  // Enemy, Player
  EzPutB((WALL_LEFT/2+1)*UNIT - enemy_x/2 - 5, (5+WALL_TOP)*UNIT - enemy_y/2, enemy);
  EzPutB((WALL_LEFT/2+1)*UNIT - self_x/2, (18+WALL_TOP)*UNIT - self_y/2, self);
  
  
  for (i = 0; i < 4; i++){
	if (my_bullet[i].valid != 0 && my_bullet[i].y <= (18+WALL_TOP)*UNIT - self_y/2){
	  EzSetColorByName("Red");
	  EzFillCircleB(my_bullet[i].x, my_bullet[i].y, BULLET_SIZE);
	  
	  EzSetColorByName("Black");
	  if (ren_check == 1 && ren - 1 > 0){
		sprintf(buf, "%d ren!", ren - 1);
		EzDrawStringB((WALL_LEFT + GAME_WIDTH + 3)*UNIT+5, 24*UNIT+10, buf);
	  }
	  
	  if (perfect_check == 1){
		EzDrawStringB((WALL_LEFT + GAME_WIDTH + 2)*UNIT, 25*UNIT+10, "Perfect clear!");
		perfect_check = 0;
	  }
	}
  }
}
void draw_gameover(void){
  if (gameover_flag == 1){
	EzSetColorByName("Red");
	EzFillBoxB((WALL_LEFT)*UNIT, (WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH)*UNIT, (WALL_TOP+GAME_HEIGHT)*UNIT);
	EzSetColorByName("White");
	EzFillBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5) + WALL_TOP)*UNIT);
	EzSetColorByName("Black");
	EzDrawBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
	EzDrawStringB((WALL_LEFT+2)*UNIT + 7, 16*UNIT + 12, "GAME OVER!");
	EzDrawStringB((WALL_LEFT+2)*UNIT + 9, 18*UNIT + 8, "Enter : Menu");
  }
}

void draw_game_victory(void){
  if (gameover_flag == 2){
	EzSetColorByName("Green");
	EzFillBoxB((WALL_LEFT)*UNIT, (WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH)*UNIT, (WALL_TOP+GAME_HEIGHT)*UNIT);
	EzSetColorByName("White");
	EzFillBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5) + WALL_TOP)*UNIT);
	EzSetColorByName("Black");
	EzDrawBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			   (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
	EzDrawStringB((WALL_LEFT+3)*UNIT, 16*UNIT + 12, "VICTORY!");
	EzDrawStringB((WALL_LEFT+2)*UNIT + 9, 18*UNIT + 8, "Enter : Menu");
  }
}

void draw_game_pause(void){
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);
  draw_field();
  EzSetColorByName("Yellow");
  EzFillBoxB((WALL_LEFT)*UNIT, (WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH)*UNIT, (WALL_TOP+GAME_HEIGHT)*UNIT);

  EzSetColorByName("White");
  EzFillBoxB((WALL_LEFT+2)*UNIT - 10, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH - 2)*UNIT + 10, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
  
  EzSetColorByName("Black");
  EzDrawBoxB((WALL_LEFT+2)*UNIT - 10, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH - 2)*UNIT + 10, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
  EzDrawStringB((WALL_LEFT+2)*UNIT+UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 1.5*UNIT, "Continue?");
  EzDrawStringB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 3.5*UNIT, "Y : Yes");
  EzDrawStringB((WALL_LEFT+2)*UNIT + 3.5*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 3.5*UNIT, "N : No");

  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 6.5*UNIT, "Up : Hard Drop");
  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 7.5*UNIT, "Down: Soft Drop");
  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 8.5*UNIT, "Left : Move Left");
  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 9.5*UNIT, "Right : Move Right");
  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 10.5*UNIT, "Z : Rotate Clockwise");
  EzDrawStringB(12*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT + 11.5*UNIT, "X : Rotate Counter-CW");
}

void draw_ready(int n){
  EzSetColorByName("White");
  EzFillBoxB(0, 0, F_WIDTH*UNIT, F_HEIGHT*UNIT);
  draw_field();
  EzSetColorByName("Yellow");
  EzFillBoxB((WALL_LEFT)*UNIT, (WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH)*UNIT, (WALL_TOP+GAME_HEIGHT)*UNIT);
  
  EzSetColorByName("White");
  EzFillBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
  
  EzSetColorByName("Black");
  EzDrawBoxB((WALL_LEFT+2)*UNIT, (2*(GAME_HEIGHT/5)+WALL_TOP)*UNIT,
			 (WALL_LEFT + GAME_WIDTH - 2)*UNIT, (3*(GAME_HEIGHT/5)+WALL_TOP)*UNIT);
  char *buf;
  switch(n){
  case 1:
	buf = "    1";
	break;
  case 2:
	buf = "    2";
	break;
  case 3:	
	buf = "    3";
	break;
  case 4:	
	buf = "Ready?";
	break;
  case 5:	
	buf = "  Go!";
	break;
  }
  EzDrawStringB((WALL_LEFT+3)*UNIT+15, (2*(GAME_HEIGHT/5)+WALL_TOP+1)*UNIT + 28, buf);
    EzShowBuffer();  
}
