#include <stdio.h>
#include <stdlib.h>
#include <EzGraph.h>
#include <time.h>
#include <string.h>
#include "library.h"

#include "my_tank.xpm" 
#include "imbada.xpm"
#include "Wall_Block.xpm" //
#include "i_mino.xpm" //
#include "j_mino.xpm" //
#include "l_mino.xpm" //
#include "s_mino.xpm" //
#include "z_mino.xpm" //
#include "o_mino.xpm" //
#include "t_mino.xpm" //
#include "tetris.xpm" //

int block[B_HEIGHT][B_WIDTH] = {0};
int stage[F_HEIGHT][F_WIDTH] = {0};
int field[F_HEIGHT][F_WIDTH] = {0};
int turn_block[B_HEIGHT][B_WIDTH];
int clear_line_point[F_HEIGHT-WALL_BOTTOM-1]; 
EzImage self, enemy;
int hold_id;

type_result result = {"", 0, 0, 0};
int block_x;
int block_y;
int block_id;
int next_block_id;
int IPS, IPS_temp;
clock_t t1, t2;
clock_t t_help1, t_help2;
clock_t t_enemy1, t_enemy2;

int gameover_flag;
int make_block_flag;
int hold_null_flag;
int bullet_flag;
int self_x, self_y, enemy_x, enemy_y; //
int id;
int ran[7] = {0};
int up_key_flag;

int ren, ren_check;
int perfect_check;

int help1, help2, help3; // help2(1:まだ使わない 2:使う途中 0:使った)
int HP;

EzImage Wall_image,IMI,OMI,TMI,SMI,ZMI,LMI,JMI, MENU_BGR; // graphic chuyen sang
extern char na[15];
extern int k;


bullet_t my_bullet[4] = {{0}}; //
type_block block_array[7] = {
							 {
							  1,
							  {{0, 0, 0, 0},
							   {0, 1, 1, 0},
							   {0, 1, 1, 0},
							   {0, 0, 0 ,0}}
							 },
							 							 
							 {
							  2,
							  {{0, 0, 0, 0},
							   {0, 0, 2, 0},
							   {2, 2, 2, 0},
							   {0, 0, 0, 0}}
							 },
							 
							 {
							  3,
							  {{0, 0, 0, 0},
							   {3, 0, 0, 0},
							   {3, 3, 3, 0},
							   {0, 0, 0, 0}}
							 },
							 
							 {
							  4,
							  {{0, 0, 0, 0},
							   {4, 4, 4, 4},
							   {0, 0, 0, 0},
							   {0, 0 ,0, 0}}
							 },
							 
							 {
							  5,
							  {{0, 0, 0, 0},
							   {0, 5, 0, 0},
							   {5, 5, 5, 0},
							   {0, 0, 0, 0}}
							 },
							 
							 {
							   6,
							  {{0, 0, 0, 0},
							   {0, 6, 6, 0},
							   {6, 6, 0, 0},
							   {0, 0, 0, 0}}
							 },
							 
							 {
							  7,
							  {{0, 0, 0, 0},
							   {7, 7, 0, 0},
							   {0, 7, 7, 0},
							   {0, 0, 0, 0}}
							 },
							 
};



void show_game(void){
  if (gameover_flag == 0){
	
	make_new_block();
	
	make_field();
	
	fix_block();
	
	if (bullet_flag != 0)
	  move_bullet();
	
	draw_field(); 
	
	if (search_line() == 1){
	  clear_line();
	  t_enemy1 = clock();
	}
	else{
	  t_enemy2 = clock();
	  // 初めて１列クレアのときから、１５秒以内にクレアしないと、敵のHPが５％に回復する。
	  if (1000.0 * (t_enemy2 - t_enemy1) / CLOCKS_PER_SEC > 2000){ 
		result.enemy_hp = (result.enemy_hp * 105) / 100;
		if (result.enemy_hp > HP)
		  result.enemy_hp = HP;
		t_enemy1 = clock();
	  }
	}

	if (collision_bottom() == 0)
	  block_y++;
	
	game_over();
	
	EzShowBuffer();
  }
  else if (gameover_flag == 1){
	stage_write();
	var_write();
	EzSetKeyHandler(key_gameover);
	EzSetTimerHandler(NULL, 100);
	draw_field();
	draw_gameover();
	EzShowBuffer();
	}
  else{
	save();
	stage_write();
	var_write();
	EzSetKeyHandler(key_gameover);
	EzSetTimerHandler(NULL, 100);
	draw_field();
	draw_game_victory();
	EzShowBuffer();
  }
}

void init_var(void){
   int i, j;
   
   // data
   for (i = 0; i < F_HEIGHT; i++)
	 for (j = 0; j < F_WIDTH; j++)
	   stage[i][j] = 0;

   // random
   srand(time(NULL));
   for (i = 0; i < 7; i++)
	 do{
	   ran[i] = rand() % 7;
	   for (j = 0; j < i; j++){
		 if (ran[i] == ran[j])
		   break;
	   }
	 } while(i != j);
   
   // Left Wall
	for (i = 0; i < WALL_LEFT; i++)
	  for(j = 0; j < F_HEIGHT; j++)
		stage[j][i] = WALL;
	// Right Wall
	for (i = WALL_LEFT + GAME_WIDTH; i < F_WIDTH; i++)
	  for(j = 0; j < F_HEIGHT; j++)
		stage[j][i] = WALL;
	// Bottom Wall
	for (i = F_HEIGHT - WALL_BOTTOM; i < F_HEIGHT; i++)
	  for(j = 0; j < F_WIDTH; j++)
		stage[i][j] = WALL;
	// Top Wall
	for (i = 0; i < WALL_TOP; i++)
	  for(j = 0; j < F_WIDTH; j++)
		stage[i][j] = WALL;
	
	// khung trai
	for(i = WALL_TOP+1; i < F_HEIGHT - WALL_BOTTOM; i++)
	  for(j = 1; j < WALL_LEFT - 1; j++)
		stage[i][j] = -1;
	
	// next block	
	for(i = 5; i < 10; i++)
	  for(j = WALL_LEFT + GAME_WIDTH + 1; j < F_WIDTH - 1; j++)
		stage[i][j] = -1;

	// hold
	for(i = 1; i < 6; i++)
	  for(j = WALL_LEFT - 8; j < WALL_LEFT-1; j++)
		stage[i][j] = -1;

	// help
	for(i = WALL_TOP + 5; i < 20; i++)
	  for(j = WALL_LEFT + GAME_WIDTH + 1; j < F_WIDTH - 1; j++)
		stage[i][j] = -1;
	
	// result
	for(i = 21; i < 28; i++)
	  for(j = WALL_LEFT + GAME_WIDTH + 1; j < F_WIDTH - 1; j++)
		stage[i][j] = -1;
	
	// Pause
	for(i = 1; i < 3; i++)
	  for(j = 25; j < 29; j++)
		stage[i][j] = -1;

	/*XPMファイルを読み込む */
	self = EzReadXPM(my_tank); 
	enemy = EzReadXPM(imbada);
	IMI=EzReadXPM(i_mino);
	OMI=EzReadXPM(o_mino);
	TMI=EzReadXPM(t_mino);
	SMI=EzReadXPM(s_mino);
	ZMI=EzReadXPM(z_mino);
	LMI=EzReadXPM(l_mino);
	JMI=EzReadXPM(j_mino);
	MENU_BGR = EzReadXPM(tetris);
  
	/* 画像の幅と高さを読み出し */
	sscanf(my_tank[0], "%d %d", &self_x, &self_y); 
	sscanf(imbada[0], "%d %d", &enemy_x, &enemy_y);
	Wall_image = EzReadXPM(Wall_Block); 

	block_x = WALL_LEFT + 4;
	block_y = WALL_TOP - 3;
	make_block_flag = 1;
	gameover_flag = 0;
	bullet_flag = 0;
	next_block_id = ran[0];
	hold_null_flag = 1;
	id = 1;
	up_key_flag = 0;
	result.attack = 0;
	result.enemy_hp = 100;
	HP = result.enemy_hp;
	IPS = 200;
	IPS_temp = IPS;
	help1= 3;
	help2 = 1;
	help3 = 1;
   
	
	for (i = 0; i < 4; i++){
	  my_bullet[i].dy = (11*UNIT - self_y/2 + enemy_y/2) / 10; /* 自機を狙う */
	  my_bullet[i].x = (WALL_LEFT/2+1)*UNIT;
	  my_bullet[i].y = (18+WALL_TOP)*UNIT - self_y/2 + my_bullet[i].dy*(1+i);
	}

}

void make_new_block(void){
  
  if (make_block_flag == 1){
	int i, j;
	up_key_flag = 0;
	block_id = next_block_id;
	next_block_id = ran[id++];
	if (id == 8){
	  for (i = 0; i < 7; i++)
		do{
		  ran[i] = rand() % 7;
		  for (j = 0; j < i; j++){
			if (ran[i] == ran[j])
			  break;
	   }
		} while(i != j);

 	  next_block_id = ran[0];
	  id = 1;
	}
	
	for(j = 0; j < B_HEIGHT; j++)
	  for(i = 0; i < B_WIDTH; i++)
		block[i][j] =  block_array[block_id].block_element[i][j];
	
	make_block_flag = 0;
	
	//ren
	if (ren_check == 1)
	  ren_check = 0;
	else
	  ren = 0;
  }
}

void make_field(void){
  int i, j, y;

  for(i = 0; i < F_HEIGHT; i++)
	for(j = 0; j < F_WIDTH; j++)
	  field[i][j] = stage[i][j];
  
  for(i = 0; i < B_HEIGHT; i++)
	for(j = 0; j < B_WIDTH; j++){
	  y = block_y + i;
	  if (y >= WALL_TOP) // Vi tri xuat hien cua khoi
		field[y][block_x + j] += block[i][j];
	}

  y = WALL_LEFT + GAME_WIDTH + 3;
  
  for(i = 0; i < B_HEIGHT; i++)
	for(j = 0; j < B_WIDTH; j++)
		field[i+6][y+j] += block_array[next_block_id].block_element[i][j] + 1;

  y = WALL_LEFT - 6;
  if (hold_null_flag == 0)
	for(i = 0; i < B_HEIGHT; i++)
	  for(j = 0; j < B_WIDTH; j++)
		field[i+2][y+j] += block_array[hold_id].block_element[i][j] + 1;
}

int collision_left(void){
  int i, j;
  for (i = 0; i < B_HEIGHT; i++)
	for (j = 0; j < B_WIDTH; j++)
	  if ((block_y + i >= WALL_TOP && block[i][j] != 0 && stage[block_y + i][block_x + (j-1)] != 0)
		  || (block_y < WALL_TOP && block_x == WALL_LEFT)) 
	    return 1;
  return 0;
  // o day thi stage[][] van chua duoc luu gia tr cua block[][]
}
int collision_right(void){
  int i, j;
  for (i = 0; i < B_HEIGHT; i++)
	for (j = 0; j < B_WIDTH; j++)
	  if ((block_y + i >= WALL_TOP && block[i][j] != 0 && stage[block_y + i][block_x + (j+1)] != 0)
		  || (block_y < WALL_TOP && block_x == WALL_LEFT + GAME_WIDTH - 4))
		return 1;
  return 0;
}
int collision_bottom(void){
  int i, j;
  for (i = 0; i < B_HEIGHT; i++)
	for (j = 0; j < B_WIDTH; j++)
	  if (block_y+i+1 >= WALL_TOP && block[i][j] != 0 && stage[block_y + i + 1][block_x + j] != 0)
		return 1;
  return 0;
}
int collision_turn(void){
  int i, j;
  for (i = 0; i < B_HEIGHT; i++)
	for (j = 0; j < B_WIDTH; j++)
	  if (turn_block[i][j] != 0 && ((block_y + i >= WALL_TOP && stage[block_y + i][block_x + j] != 0) || block_x+j < WALL_LEFT))		
		return 1;
  return 0;
}

void roll(int (*p_block)[B_WIDTH], char *dir){
  int i, j, a;
  if (strcmp(dir, "clockwise"))
	a = 1;
  
   if (block_id == 0 || block_id == 3)
	for (i = 0; i < B_HEIGHT; i++)
	  for (j = 0; j < B_WIDTH; j++)
		turn_block[i][j] = a == 1 ? block[j][3-i] : block[3-j][i];
  else
	for (i = 1; i < B_HEIGHT; i++)
	  for (j = 0; j < B_WIDTH-1; j++)
		turn_block[i][j] = a == 1 ?block[j+1][3-i] : block[3-j][i-1] ;
	
  if (collision_turn() != 1){
	if (block_id == 0 || block_id == 3)
	  for (i = 0; i < B_HEIGHT; i++)
		for (j = 0; j < B_WIDTH; j++)
		  block[i][j] = turn_block[i][j];
	else
	  for (i = 1; i < B_HEIGHT; i++)
		for (j = 0; j < B_WIDTH - 1; j++)
		  block[i][j] = turn_block[i][j];
  }
}
void game_over(void){
  int i;
  for (i = WALL_LEFT; i < WALL_LEFT + GAME_WIDTH; i++)
	if (stage[WALL_TOP][i] != 0){
	  gameover_flag = 1;
	  break;
	}
}
void clear_line(void){
  int i, j, k = F_HEIGHT - WALL_BOTTOM  - 1;
  int stage_temp[F_HEIGHT-WALL_BOTTOM][F_WIDTH-6] = {0};
  
  result.clear_line = 0; // so hang duoc clear trong mot luot
  
  for (i = F_HEIGHT - WALL_BOTTOM - 1; i >= WALL_TOP; i--)
	if (!clear_line_point[i]){
	  for (j = WALL_LEFT; j < WALL_LEFT + GAME_WIDTH; j++)
		stage_temp[k][j-WALL_LEFT] = stage[i][j];
	  k--;
	}
	else
	  result.clear_line++;

  // x2 check
  k = help3 == 2 ? 2 : 1;
  if (help3 == 2)
	help3 = 0;
  
  for (i = 0; i <result.clear_line; i++){
	k *= 2;
	my_bullet[i].valid = 1;
	bullet_flag++;
  }

  
  result.attack += k;
  result.enemy_hp -= k;
  if (result.enemy_hp <= 0){
	result.enemy_hp = 0;
	gameover_flag = 2;
  }
  
  for (i = F_HEIGHT - WALL_BOTTOM - 1; i >= WALL_TOP; i--)
	for (j = WALL_LEFT; j < WALL_LEFT + GAME_WIDTH; j++)
	  stage[i][j] = stage_temp[i][j-WALL_LEFT];

    // ren check
  ren_check = 1;
  ren++;
  
  // perfect check
  perfect_check = 1;
  for (i = F_HEIGHT - WALL_BOTTOM - 1; i >= WALL_TOP; i--){
	for (j = WALL_LEFT; j < WALL_LEFT + GAME_WIDTH; j++)
	  if (stage[i][j] != 0){
		perfect_check = 0;
		break;
	  }
	if (perfect_check == 0)
	  break;
  }
  if (perfect_check == 1){
	result.enemy_hp -= k;
	result.attack += k;
  }

  // speedの調節
  if (help2 == 2)
	EzSetTimerHandler(slow, IPS);
  else{
	k = (result.enemy_hp*100) / HP;
	if (k < 30){
	  IPS = 100;
	  EzSetTimerHandler(show_game, IPS);
	} else if (k < 80){
	  IPS = 150;
	  EzSetTimerHandler(show_game, IPS);
	}
  }
	
}

int search_line(void){
  int i, j, l;

  for (i = WALL_TOP; i < F_HEIGHT - WALL_BOTTOM; i++){
	clear_line_point[i] = 1;	
	for (j = WALL_LEFT; j < WALL_LEFT + GAME_WIDTH; j++)
	  if(stage[i][j] == 0){
		clear_line_point[i] = 0; 
		break;
	  }
  }

  for (i = WALL_TOP; i < F_HEIGHT - WALL_BOTTOM; i++)
	if (clear_line_point[i] == 1){
	  for(l=1;l<=254;l++){
     EzSetColorByRGB(l, 0, 255-l);
	 EzFillCircleB((WALL_LEFT+GAME_WIDTH/2)*UNIT,((GAME_HEIGHT/2+WALL_TOP)*UNIT)*i/15, 64-l/4);
	  }
	  return 1;
	  break;
	}
  
  return 0;
}
void save_block(void){
  int i, j;

  for (i = 0; i < B_HEIGHT; i++)
	for (j = 0; j < B_WIDTH; j++)
	  stage[block_y + i][block_x + j] += block[i][j];
}
void init_var2(void){
  int i;
  block_x = WALL_LEFT + 4;
  block_y = WALL_TOP - 3;
  make_block_flag = 1;

  for (i = 0; i < 4; i++)
	my_bullet[i].y = (18+WALL_TOP)*UNIT - self_y/2 + my_bullet[i].dy*(1+i);
	
}

void move_bullet(void){
   int i;

   for (i = 0; i < 4; i++){
	 if (my_bullet[i].valid == 1)
	   my_bullet[i].y -= my_bullet[i].dy;
	   
	 if (my_bullet[i].y + BULLET_SIZE < (5+WALL_TOP)*UNIT + enemy_y/2)
	   my_bullet[i].valid = 0;
   }
}

void grace(void){
  draw_field();
  EzShowBuffer();
  if (collision_bottom() == 0)
	block_y++;
  make_field();
  t2 = clock();
  if(1000.0 * (t2 - t1) / CLOCKS_PER_SEC > 50 || up_key_flag == 1){
	while(collision_bottom() == 0)
	  block_y++;
  	save_block();
  	init_var2();
	t1 = t2;
	game_over();
	if (help2 == 2)
	  EzSetTimerHandler(slow, IPS);
	else
	   EzSetTimerHandler(show_game, IPS);
  } 
}

void fix_block(void){
  if (collision_bottom() == 1){
	t1 = clock();
	EzSetTimerHandler(grace, IPS);
  }
}

void slow(void){
  if (gameover_flag == 0){
  make_new_block();
  make_field();
  fix_block();
  if (bullet_flag != 0)
	move_bullet();
  draw_field();
  EzShowBuffer();
  if (search_line() == 1)
	clear_line();
  if (collision_bottom() == 0)
	block_y++;
  game_over();
  t_help2 = clock();
  
  if(1000.0 * (t_help2 - t_help1) / CLOCKS_PER_SEC > 1000){
	help2 = 0;
	IPS = IPS_temp;
  	EzSetTimerHandler(show_game, IPS);
  }
  }
}

void help(int n){
  // ap dung thang grace de lam cham
  switch(n){
	// プロック交換
  case 1:
	;
	int i, j, check = 1;
	for (i = 0; i < B_HEIGHT; i++)
	  for (j = 0; j < B_WIDTH; j++)
		if (block_array[3].block_element[i][j] != 0
			&& stage[block_y + i][block_x + j] != 0)
		  check = 0;
	
	if (check && block_id != 3){
	  for (i = 0; i < B_HEIGHT; i++)
		for (j = 0; j < B_WIDTH; j++)
		  block[i][j] = block_array[3].block_element[i][j];
	  help1--;
	  block_id = 3;
	}
	break;
	
  case 2: // スピード落とす
	t_help1 = clock();
	help2 = 2;
	IPS_temp = IPS;
	IPS = 500;
	EzSetTimerHandler(slow, IPS);
	break;
	
  case 3: // x2
	help3 = 2;
	break;
  }
}

void hold(void){
 if (hold_null_flag == 1){
	hold_id = block_id;
	hold_null_flag = 0;
	make_block_flag = 1;
	block_y = WALL_TOP - 3;
  }
 else {
   int i, j, check = 0;
   
	for (i = 0; i < B_HEIGHT; i++)
	  for (j = 0; j < B_WIDTH; j++)
		if (block_array[hold_id].block_element[i][j] != 0
			&& stage[block_y + i][block_x + j] != 0)
		  check = 1;
		  
	if (check == 0){
	  int temp;
	  temp = hold_id; 
	  hold_id = block_id;
	  block_id = temp;
	  for (i = 0; i < B_HEIGHT; i++)
		for (j = 0; j < B_WIDTH; j++)
		  block[i][j] = block_array[block_id].block_element[i][j];
	}
 }
}

void save(){
  FILE *fp;
  int i, j, check, max, min;
  char ten[10][15];
  int diem[10] = {0};
  
  fp = fopen("result.txt", "r");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
	exit(1);
  }
  
  fscanf(fp, "%d %d %d", &check, &min, &max);
  
  j = 9;
  if (check == 1 && result.score <= max){
	for (i = 0; i < 10; i++){
	  fscanf(fp, "%s %d", ten[i], &diem[i]);
	  if (diem[i] == 0){
		j = i;
		break;
	  }
	}
  }
  fclose(fp);

  fp = fopen("result.txt", "w");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
	exit(1);
  }
  if (check == 0){
	fprintf(fp, "1 %d %d\n", result.attack, result.attack);
	fprintf(fp, "%s %d\n", result.name, result.attack);
	for (i = 1; i < 10; i++)
	  fprintf(fp, "A 0\n");
  }
  else{
	
	if (j == 9){
	  strcpy(ten[9], na);
	  diem[9] = result.score;
	} else{
	  strcpy(ten[j], na);
	  diem[j] = result.attack;
	}
	
	for (i = 0; i <= j; i++)
	  if (diem[j] <= diem[i]){
		int temp1;
		char temp2[15];
		temp1 = diem[i];
		diem[i] = diem[j];
		diem[j] = temp1;
		strcpy(temp2, ten[i]);
		strcpy(ten[i], ten[j]);
		strcpy(ten[j], temp2);
	  }

	fprintf(fp, "1 %d %d\n", diem[0], diem[j]);
	for (i = 0; i <= j; i++)
	  fprintf(fp, "%s %d\n", ten[i], diem[i]);
	
	for (i = j + 1; i < 10; i++)
	fprintf(fp, "A 0\n");
  } 
  
  fclose(fp);

}

void stage_read(void){
  FILE *fp;
  int i, j;

  fp = fopen("stage.txt", "r");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
        exit(1);
  }

  // stage
  for (i = 0; i < F_HEIGHT; i++){
	for (j = 0; j < F_WIDTH; j++)
	  fscanf(fp, "%d ", &stage[i][j]);
  }

  // field
    for (i = 0; i < F_HEIGHT; i++){
	for (j = 0; j < F_WIDTH; j++)
	  fscanf(fp, "%d ", &field[i][j]);
  }

	 // block
   for (i = 0; i < B_HEIGHT; i++){
	for (j = 0; j < B_WIDTH; j++)
	  fscanf(fp, "%2d ", &block[i][j]);
  }
   // turn block
   for (i = 0; i < B_HEIGHT; i++){
	for (j = 0; j < B_WIDTH; j++)
	  fscanf(fp, "%2d ", &turn_block[i][j]);
  }
   // clear_line
   for (i = 0; i < F_HEIGHT-WALL_BOTTOM-1; i++)
	 fscanf(fp, "%2d ", &clear_line_point[i]);
   
   // ran
   for (i = 0; i < 7; i++)
	 fscanf(fp, "%2d ", &ran[i]);
   
  fclose(fp);   
}
void stage_write(void){
 FILE *fp;
  int i, j;

  fp = fopen("stage.txt", "w");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
        exit(1);
  }

  // stage
  for (i = 0; i < F_HEIGHT; i++){
	for (j = 0; j < F_WIDTH; j++)
	  fprintf(fp, "%2d ", stage[i][j]);
	
	fprintf(fp, "\n");
  }

  // field
   for (i = 0; i < F_HEIGHT; i++){
	for (j = 0; j < F_WIDTH; j++)
	  fprintf(fp, "%2d ", field[i][j]);
	
	fprintf(fp, "\n");
  }

   // block
   for (i = 0; i < B_HEIGHT; i++){
	for (j = 0; j < B_WIDTH; j++)
	  fprintf(fp, "%2d ", block[i][j]);
	
	fprintf(fp, "\n");
  }
   // turn block
   for (i = 0; i < B_HEIGHT; i++){
	for (j = 0; j < B_WIDTH; j++)
	  fprintf(fp, "%2d ", turn_block[i][j]);
	
	fprintf(fp, "\n");
  }
   // clear_line
   for (i = 0; i < F_HEIGHT-WALL_BOTTOM-1; i++)
	 fprintf(fp, "%2d ", clear_line_point[i]);
   fprintf(fp, "\n");
   
   // ran
   for (i = 0; i < 7; i++)
	 fprintf(fp, "%2d ", ran[i]);
   fprintf(fp, "\n");
   
  fclose(fp); 
}

void var_write(void){
 FILE *fp;
  int i;
  fp = fopen("var.txt", "w");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
        exit(1);
  }

  fprintf(fp, "%d\n", hold_id);
  fprintf(fp, "%d\n", up_key_flag);
  fprintf(fp, "%d %d\n", IPS, IPS_temp);
  fprintf(fp, "%d\n", block_x);
  fprintf(fp, "%d\n", block_y);
  fprintf(fp, "%d\n", block_id);
  fprintf(fp, "%d\n", next_block_id);
  fprintf(fp, "%d\n", gameover_flag);
  fprintf(fp, "%d\n", make_block_flag);
  fprintf(fp, "%d\n", hold_null_flag);
  fprintf(fp, "%d\n", bullet_flag);
  fprintf(fp, "%d %d %d %d\n", self_x, self_y, enemy_x, enemy_y);
  fprintf(fp, "%d\n", id);
  fprintf(fp, "%d %d\n", ren, ren_check);
  fprintf(fp, "%d\n", perfect_check);
  fprintf(fp, "%d %d %d\n", help1, help2, help3);
  fprintf(fp, "%d %d\n", HP, k);
  
  for (i = 0; i < 4; i++)
	fprintf(fp, "%d %d %d %d\n", my_bullet[i].x, my_bullet[i].y, my_bullet[i].dy, my_bullet[i].valid);
  fprintf(fp, "%d %d %d %d %d %s\n", result.clear_line, result.clear_line_sum, result.enemy_hp, result.score, result.attack, na);
  
  fclose(fp); 
}
void var_read(void){
  FILE *fp;
  int i;
  
  fp = fopen("var.txt", "r");
  if (fp == NULL){
	printf("ファイルオープン失敗\n");
        exit(1);
  }

  fscanf(fp, "%d\n", &hold_id);
  fscanf(fp, "%d\n", &up_key_flag);
  fscanf(fp, "%d %d\n", &IPS, &IPS_temp);
  fscanf(fp, "%d\n", &block_x);
  fscanf(fp, "%d\n", &block_y);
  fscanf(fp, "%d\n", &block_id);
  fscanf(fp, "%d\n", &next_block_id);
  fscanf(fp, "%d\n", &gameover_flag);
  fscanf(fp, "%d\n", &make_block_flag);
  fscanf(fp, "%d\n", &hold_null_flag);
  fscanf(fp, "%d\n", &bullet_flag);
  fscanf(fp, "%d %d %d %d\n", &self_x, &self_y, &enemy_x, &enemy_y);
  fscanf(fp, "%d\n", &id);
  fscanf(fp, "%d %d\n", &ren, &ren_check);
  fscanf(fp, "%d\n", &perfect_check);
  fscanf(fp, "%d %d %d\n", &help1, &help2, &help3);
  fscanf(fp, "%d %d\n", &HP, &k);
  
  for (i = 0; i < 4; i++)
	fscanf(fp, "%d %d %d %d\n", &my_bullet[i].x, &my_bullet[i].y, &my_bullet[i].dy, &my_bullet[i].valid);
  
  char name[15];
  fscanf(fp, "%d %d %d %d %d %s\n", &result.clear_line, &result.clear_line_sum, &result.enemy_hp, &result.score, &result.attack, name);
  result.name = name;
  strcpy(na, name);
  fclose(fp); 
}
