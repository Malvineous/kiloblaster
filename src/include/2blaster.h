// Header file for "Blaster.C"

#define end_x 16			// number of blocks 16 x 16
#define end_y 12
#define scrn_x 256			// number of pixels in game viewport
#define scrn_y 192
extern vptype gamevp, cmdvp, statvp, textvp;
extern int board[end_x][end_y];
extern int modflg[end_x][end_y];
extern int gamecount, cnt_down;
extern int fire_cnt, weapon_cnt;
extern int wing1_x, wing1_y, wing2_x, wing2_y;
extern int wing3_x, wing3_y, wing4_x, wing4_y;
extern int star_flag, x_pnt, y_pnt, x_pnt2, y_pnt2, x_pnt3, y_pnt3;
extern int bonus_flg1, bonus_flg2, bonus_flg3;
extern int enemy_cnt, enemy_max, enemy_flag;
extern unsigned long points, extra_ship;
#define modboard(x, y) modflg[x][y] |= mod_screen;

#define mirrortabd {\
   0,0,0,0,0,0,0,0,0,0,\
   0,0,0,0,0,0,0,0,0,0,\
   0,0,0,0,0,0,0,0,0,0,\
   0,0,0,0,0,0,0,0,0,0,\
   0,0,0,0,0,0,0,0,0,0}

#define hilen       10
#define savelen     10
#define numhighs    10
#define numsaves    7
extern char hiname [hilen][numhighs];
extern char savename [numsaves][savelen];
extern unsigned long hiscore [numhighs];

#define mod_screen  3		// For non-pageflipping
#define mod_page0   1		// for page-flipping
#define mod_page1   2

#define b_blank      0		// Define background objects
#define b_wall1      1
#define b_wall2      2
#define b_wall3      3
#define b_wall4      4
#define b_wall5      5
#define b_wall6      6
#define b_wall7      7
#define b_wall8      8
#define b_wall9      9		// stonewall
#define b_walla     10
#define b_wallb     11
#define b_wallc     12
#define b_walld     13
#define b_walle     14
#define b_wallf     15
#define b_wallg     16		// center piece
#define b_wallh     17		// top left
#define b_walli     18		// bottom left
#define b_wallj     19		// top right
#define b_wallk     20		// bottom right
#define b_walll     21		// top middle
#define b_wallm     22		// bottom middle
#define b_walln     23		// left side
#define b_wallo     24		// right side
#define b_wallp     25		// top left
#define b_wallq     26		// bottom left
#define b_wallr     27		// top right
#define b_walls     28		// bottom right
#define b_wallt     29		// top middle (blue)
#define b_wallu     30		// bottom middle (blue)

#define b_maxbkgnd  31

typedef struct {
	int sh;
	char *na;
	int flags;
	} info_type;

#define obj_player    0		// Define objects
#define obj_killme    1
#define obj_bullet    2
#define obj_platinum  3
#define obj_bomb      4
#define obj_missile   5
#define obj_bullet2   6		// enemy weapon
#define obj_spinner   7		// enemy weapon
#define obj_enemy1    8
#define obj_enemy2    9
#define obj_enemy3   10
#define obj_enemy4   11
#define obj_enemy5   12
#define obj_enemy6   13
#define obj_enemy7   14
#define obj_enemy8   15
#define obj_enemy9   16
#define obj_enemya   17
#define obj_enemyb   18
#define obj_enemyc   19
#define obj_enemyd   20
#define obj_enemye   21
#define obj_enemyf   22
#define obj_enemyg   23
#define obj_enemyh   24
#define obj_enemyi   25
#define obj_enemyj   26
#define obj_enemyk   27
#define obj_ejected  28
#define obj_explode1 29
#define obj_explode2 30
#define obj_explode3 31
#define obj_explode4 32
#define obj_explode5 33
#define obj_explode6 34
#define obj_triple   35
#define obj_bonus1   36
#define obj_bonus2   37
#define obj_bonus3   38
#define obj_bonus4   39
#define obj_bonus5   40
#define obj_wing1    41
#define obj_wing2    42
#define obj_wing3    43
#define obj_wing4    44
#define obj_stars    45
#define obj_falling  46
#define obj_score    47
#define obj_level1   48
#define obj_shield   49
#define obj_laser    50
#define obj_max      51		// max shields
#define obj_mines    52
#define obj_demo     53
#define obj_jump1    54		// player jumping in
#define obj_jump2    55		// enemy jumping in

#define maxobjkinds  56		// total objects

int msg_player	     (int n, int msg, int z);
int msg_killme      (int n, int msg, int z);
int msg_bullet		(int n, int msg, int z);
int msg_platinum    (int n, int msg, int z);
int msg_bomb        (int n, int msg, int z);
int msg_missile     (int n, int msg, int z);
int msg_bullet2     (int n, int msg, int z);
int msg_spinner     (int n, int msg, int z);
int msg_enemy1		(int n, int msg, int z);
int msg_enemy2      (int n, int msg, int z);
int msg_enemy3      (int n, int msg, int z);
int msg_enemy4      (int n, int msg, int z);
int msg_enemy5      (int n, int msg, int z);
int msg_enemy6      (int n, int msg, int z);
int msg_enemy7      (int n, int msg, int z);
int msg_enemy8      (int n, int msg, int z);
int msg_enemy9      (int n, int msg, int z);
int msg_enemya      (int n, int msg, int z);
int msg_enemyb      (int n, int msg, int z);
int msg_enemyc      (int n, int msg, int z);
int msg_enemyd      (int n, int msg, int z);
int msg_enemye      (int n, int msg, int z);
int msg_enemyf      (int n, int msg, int z);
int msg_enemyg      (int n, int msg, int z);
int msg_enemyh      (int n, int msg, int z);
int msg_enemyi      (int n, int msg, int z);
int msg_enemyj      (int n, int msg, int z);
int msg_enemyk      (int n, int msg, int z);
int msg_ejected     (int n, int msg, int z);
int msg_explode1    (int n, int msg, int z);
int msg_explode2    (int n, int msg, int z);
int msg_explode3    (int n, int msg, int z);
int msg_explode4    (int n, int msg, int z);
int msg_explode5    (int n, int msg, int z);
int msg_explode6    (int n, int msg, int z);
int msg_triple      (int n, int msg, int z);
int msg_bonus1      (int n, int msg, int z);
int msg_bonus2      (int n, int msg, int z);
int msg_bonus3      (int n, int msg, int z);
int msg_bonus4      (int n, int msg, int z);
int msg_bonus5      (int n, int msg, int z);
int msg_wing1       (int n, int msg, int z);
int msg_wing2       (int n, int msg, int z);
int msg_wing3       (int n, int msg, int z);
int msg_wing4       (int n, int msg, int z);
int msg_stars       (int n, int msg, int z);
int msg_falling     (int n, int msg, int z);
int msg_score       (int n, int msg, int z);
int msg_level1      (int n, int msg, int z);
int msg_shield      (int n, int msg, int z);
int msg_laser       (int n, int msg, int z);
int msg_max         (int n, int msg, int z);
int msg_mines       (int n, int msg, int z);
int msg_demo        (int n, int msg, int z);
int msg_jump1       (int n, int msg, int z);
int msg_jump2       (int n, int msg, int z);

typedef struct {
	char *na;					// objects name
	int xl, yl;				// size in pixels
	int (*msg)(int n, int msg, int z);
	int flags;
	int class;				// is it player, enemy or what?
	} objinfo_type;

#define class_enemy   1
#define class_player  2
#define class_wing1   3
#define class_wing2   4
#define class_wing3   5
#define class_wing4   6
#define class_weapon1 7
#define class_weapon2 8
#define class_shield  9

#define max_objs 64				// total # of objects allowed on screen

typedef struct {
	int kind;
	int x, y;
	int xl, yl;
	int xd, yd;				// count is for animation sequences
	int state, flags, class;		// count2 keeps track of updating
	int count, count2, count3;	// count3 is to track multiple enemy hits
	} obj_type;

#define  msg_draw   0
#define  msg_update 1
#define  msg_touch  2

typedef struct {
	unsigned long score;
	int level;
	int health;
	int num_inv;
	int inv[40];
	} pltype;

#define inv_player   0			// number of ships
#define inv_triple   1			// 3-way fire power
#define inv_split    2			// Multiple Warhead Missile
#define inv_bomb     3			// Smart Bomb
#define num_invkinds 4			// total inventory kinds

#define st_begin 0
#define st_die   1

extern info_type info[b_maxbkgnd];
extern objinfo_type objinfo[maxobjkinds];
extern obj_type objs[max_objs];

extern int num_objs, gameover, statmodflg, pcx_sh, winflg;
extern pltype pl;
extern char botmsg[50];
extern int text_flg;

void init_colors (void);
void init_objinfo (void);
void init_info (void);
void init_objs (void);
void init_inv (void);
void init_brd (void);			// initialize (set up) the board

void drawcell (int x, int y);		// draw the board element at x, y
void drawboard (void);			// draw the entire board
void edit_brd (void);
void loadbrd (char *fname);		// loads a board with .BAK extension
void loadboard (char *fname);		// loads a board with .BRD extension
void saveboard (char *fname);		// saves a board with .BRD extension

void upd_objs (void);
void upd_objs2 (void);
void upd_end (void);
void refresh (int pagemode);

void addobj (int kind, int x, int y, int xd, int yd);
void killobj (int n);
void purgeobjs (void);
void p_new (void);
void p_hit (int take);

void rest (int num);
void wait (void);
void wait2 (void);
void wait4 (void);
void story (int page);
void menu (void);
void intro (void);
void game_end (void);
void scrn_1 (void);