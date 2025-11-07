#ifndef _library
#define _library

#define B_HEIGHT 4
#define B_WIDTH 4 

#define WALL 9 // 壁の値
#define BLOCK 1 // ブロックの値
#define UNIT 20 // 画面の単位の値

#define GAME_HEIGHT 20 //
#define GAME_WIDTH 10 //
#define WALL_LEFT 11 //
#define WALL_RIGHT 9 // 
#define WALL_BOTTOM 3 // 下壁のブロック数
#define WALL_TOP 7 
#define F_HEIGHT (GAME_HEIGHT + WALL_BOTTOM + WALL_TOP) // ゲームの高さ
#define F_WIDTH (WALL_LEFT + GAME_WIDTH + WALL_RIGHT) // ゲームの広さ
#define BULLET_SIZE 8 // たまのサイズ

void show_game(void); //ゲームを表示する
void init_var(void); // ゲームを初期化
void init_var2(void);// ブロックをクリアした後、新しいブロックを作る
void make_new_block(void); //　ブロックをランダムに生成する
void make_field(void); //ゲームのバッファを作る
void game_over(void); //　game over を判断
void fix_block(void); //　ゲームバッファを保存した後、新しいブロックを生成
void save_block(void); //　ゲームバッファを保存する
void move_bullet(void); // たまを動かす
void grace(void);
void slow(void);

int collision_left(void); // 左に移動できるかどうかを確認
int collision_right(void); //右に移動できるかどうかを確認
int collision_bottom(void); //ブロックが着地したかどうかを確認
int collision_turn(void); //　回転できるかどうかを確認
void roll(int (*p_block)[B_WIDTH], char *dir); //ブロックを回転
void clear_line(void); //　埋め尽くされた行を消す
int search_line(void); //　埋め尽くされた行を探す

void draw_field(void);// ゲームのバッファを描く
void draw_gameover(void); //　game over 画面を描く
void draw_game_pause(void); //　停止している画面を描く
void draw_menu(void);
void draw_result(void);
void draw_name(void);
void draw_game_victory(void);
void draw_ready(int n);

void key_handler(int key); //　ゲームキー
void key_menu(int key);
void key_pause(int key); // 停止key
void key_result(int key);
void key_gameover(int key);
void help(int);
void hold(void);
void key_name(int n);

void save(void);
void stage_write(void);
void stage_read(void);
void var_write(void);
void var_read(void);

typedef struct{
  char *name;
  int clear_line;
  int clear_line_sum;
  int attack;
  int enemy_hp;
  int score;
} type_result;

typedef struct
{
    int x, y;    /* 弾の中心座標 */
    int dy;  /* 弾の速度 */
    int valid;      /* 有効フラグ */
} bullet_t;

typedef struct {
  int id;
  int block_element[B_HEIGHT][B_WIDTH];
} type_block;


#endif
