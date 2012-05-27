//	2BLASTER.C
//
// Kiloblaster v.2 Main File
//
// Written by Allen W. Pilgrim

unsigned _stklen=8192;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>			// for savegame and loadgame
#include <ctype.h>
#include "include/gr.h"
#include "include/keyboard.h"
#include "include/windows.h"
#include "include/gamectrl.h"
#include "include/uncrunch.h"
#include "include/config.h"
#include "include/music.h"
#include "include/2blaster.h"

vptype gamevp, cmdvp, statvp, textvp;
int16_t board[end_x][end_y];
int16_t modflg[end_x][end_y];
int16_t gamecount, cnt_down;
int16_t fire_cnt, weapon_cnt;
int16_t wing1_x, wing1_y, wing2_x, wing2_y;
int16_t wing3_x, wing3_y, wing4_x, wing4_y;
int16_t star_flag, x_pnt, y_pnt, x_pnt2, y_pnt2, x_pnt3, y_pnt3;
int16_t bonus_flg1, bonus_flg2, bonus_flg3;
int16_t enemy_cnt, enemy_max, enemy_flag;
uint32_t points, extra_ship;
extern int16_t vocflag;
wintype menu_win;

info_type info[b_maxbkgnd];
objinfo_type objinfo[maxobjkinds];
obj_type objs[max_objs];

int16_t num_objs, gameover, statmodflg, pcx_sh, winflg;
pltype pl;
char botmsg [50];
extern char k_msg2[];
int16_t text_flg;

char hiname [hilen][numhighs];
char savename [numsaves][savelen];
uint32_t hiscore [numhighs];
char mirrortab[num_samps]=mirrortabd;

//int16_t debug=0;
//int16_t swrite=0;
void rexit (int16_t n);
int16_t askquit (void);

uint32_t o_extra;
pltype o_pl;

extern char demolvl[3];
extern char *demoname[3];
char tempname[16];

extern char gamename[],audioname[],shapename[];
extern const char *end_msg[5];
extern char *song1[32];
extern char cfgfname[];
extern char ext[];

void init_colors (void) {
	setcolor (1,0,0,42); setcolor (2,0,36,0); setcolor (3,18,41,52);
	setcolor (4,48,0,0); setcolor (5,23,13,33); setcolor (6,28,16,4);
	setcolor (7,32,36,40); setcolor (8,12,12,12); setcolor (9,10,30,63);
	setcolor (10,0,60,41); setcolor (11,32,52,63); setcolor (12,63,0,0);
	setcolor (13,49,39,57); setcolor (14,63,62,32);
	};

void init_win (void) {
	int16_t x,y;
	gamevp.vpx=0; gamevp.vpy=0;
	gamevp.vpox=0; gamevp.vpoy=0;
	gamevp.vpxl=256; gamevp.vpyl=192;

	cmdvp.vpx=264; cmdvp.vpy=8;
	cmdvp.vpox=0; cmdvp.vpoy=0;
	cmdvp.vpxl=48; cmdvp.vpyl=112;

	statvp.vpx=264; statvp.vpy=140;
	statvp.vpox=0; statvp.vpoy=0;
	statvp.vpxl=48; statvp.vpyl=40;

	textvp.vpx=10; textvp.vpy=193;
	textvp.vpox=0; textvp.vpoy=0;
	textvp.vpxl=300; textvp.vpyl=6;

	drawshape (&mainvp,0x30a,0,192);		// bar below game window
		for (x=16; x<289; x+=16) {
			drawshape (&mainvp,0x30b,x,192);
			};
	drawshape (&mainvp,0x30c,304,192);

	drawshape (&mainvp,0x300,256,0);		// top cmd_win
	drawshape (&mainvp,0x304,264,0);
	drawshape (&mainvp,0x304,280,0);
	drawshape (&mainvp,0x304,296,0);
	drawshape (&mainvp,0x301,312,0);

	for (y=12; y<93; y+=16) {				// sides cmd_win
		drawshape (&mainvp,0x306,256,y);
		drawshape (&mainvp,0x307,312,y);
		};
	drawshape (&mainvp,0x308,256,108);
	drawshape (&mainvp,0x309,312,108);

	drawshape (&mainvp,0x302,256,116);	// bottom cmd_win
	drawshape (&mainvp,0x305,264,120);
	drawshape (&mainvp,0x305,280,120);
	drawshape (&mainvp,0x305,296,120);
	drawshape (&mainvp,0x303,312,116);

	drawshape (&mainvp,0x300,256,132);	// top stat_win
	drawshape (&mainvp,0x304,264,132);
	drawshape (&mainvp,0x304,280,132);
	drawshape (&mainvp,0x304,296,132);
	drawshape (&mainvp,0x301,312,132);

	for (y=144; y<161; y+=16) {			// sides stat_win
		drawshape (&mainvp,0x306,256,y);
		drawshape (&mainvp,0x307,312,y);
		};

	drawshape (&mainvp,0x302,256,176);		// bottom stat_win
	drawshape (&mainvp,0x305,264,180);
	drawshape (&mainvp,0x305,280,180);
	drawshape (&mainvp,0x305,296,180);
	drawshape (&mainvp,0x303,312,176);
	};

void init_brd (void) {
	int16_t x, y;
	clrvp (&gamevp, 0);
	for (x = 0; x < end_x; x++) {
		for (y = 0; y < end_y; y++) {
			board[x][y] = 0;
			modflg[x][y] = 0;
			};
		};
	};

void drawboard (void) {
	int16_t x, y;
	statmodflg = 0;
	refresh (0);
	};

void drawcell (int16_t x, int16_t y) {
	if (board[x][y]==0) {
		drawshape (&gamevp, pcx_sh + x + y * 16, x * 16, y * 16);
		}
	else {
		drawshape (&gamevp, info[board[x][y]].sh, x * 16, y * 16);
		};
	};

void moddrawboard (void) {
	int16_t x, y;
	for (x = 0; x < end_x; x++)
		for (y = 0; y < end_y; y++)
			modboard(x, y);
	statmodflg |= mod_screen;
	};

void loadboard (char *fname) {		// loads a board with .BRD extension
	int16_t boardfile;
	char dest[12];
	char *src1 = ext;
	strcpy(dest, fname);
	strcat(dest, src1);
	boardfile = _open (dest, O_BINARY);
	if (!read (boardfile, &board, sizeof(board))); // rexit(1);
	_close (boardfile);
	};

void saveboard (char *fname) {		// saves a board with .BRD extension
	int16_t boardfile;
	char dest[12];	char dest2[12];
	char *src1 = ext; char *src2 = ".bak";
	strcpy(dest, fname); strcat(dest, src1);
	strcpy(dest2, fname); strcat(dest2, src2);
	boardfile = _creat (dest, 0644);
	if (boardfile== -1) {
		rename (dest, dest2);
		boardfile = _creat (dest, 0644);
		if (!write (boardfile, &board, sizeof(board))) rexit(5);
		}
	else {
		if (!write (boardfile, &board, sizeof(board))) rexit(5);
		};
		_close (boardfile);
	};

void loadcfg (void) {
	int16_t cfgfile, c;
	cfgfile=_open (cfgfname,O_BINARY);
	if ((cfgfile < 0) || (filelength(cfgfile) <= 0)) {
		for (c = 0; c < numhighs; c++) {hiname[c][0]='\0'; hiscore[c]=0;};
		for (c = 0; c < numsaves; c++) {savename[c][0]='\0';};
		cf.firstthru = 1;
		}
	else {
		read (cfgfile, &hiname, sizeof(hiname));
		read (cfgfile, &hiscore, sizeof(hiscore));
		read (cfgfile, &savename, sizeof(savename));
		if (read (cfgfile, &cf, sizeof (cf)) < 0) cf.firstthru = 1;
		};
	close (cfgfile);
	};

void savecfg (void) {
	int16_t cfgfile;
	cfgfile=_creat (cfgfname,0644);
	if (cfgfile >= 0) {
		write (cfgfile, &hiname, sizeof(hiname));
		write (cfgfile, &hiscore, sizeof(hiscore));
		write (cfgfile, &savename, sizeof(savename));
		write (cfgfile, &cf, sizeof (cf));
		};
	close (cfgfile);
	};

void text (const char *msg, int16_t flg) {
	strcpy (botmsg, msg);
	if (!flg) cnt_down=100;
	text_flg=flg;
	statmodflg|=mod_screen;
	};

void stat_win (void) {
	int16_t x;
	char tempstr[12];

	fontcolor (&cmdvp,14,0);
	wprint (&cmdvp,45-(strlen(tempstr)*6),16,2,
		ultoa (pl.score,tempstr,10));					// score

	for (x=0; x<5; x++) {							// shields
		if (x<=pl.health-1) drawshape (&cmdvp,0xd09,x*8+5,42);
		else drawshape (&cmdvp,0x4400+17,x*8+5,42);
		};

	for (x=0; x<5; x++) {							// bombs
		if (x<=inv_cnt(inv_bomb)-1) drawshape (&cmdvp,0x508,x*8+6,71);
		else drawshape (&cmdvp, 0x4400+18, x*8+6, 71);
		};

	for (x=0; x<=inv_cnt(inv_triple); x++) {				// triple
		if (x<=9) {
			drawshape (&cmdvp,0x4400+20,22,88);
			wprint (&cmdvp,30,88,1,itoa(x,tempstr,10));
			}
		else wprint (&cmdvp,22,88,1,itoa(x,tempstr,10));
		};

	for (x=0; x<3; x++) {							// ships
		if (x<=inv_cnt(inv_player)-1) drawshape (&statvp,0x801,x*14+4,16);
		else drawshape (&statvp,0x4400+19,x*14+4,16);
		};

//	drawshape (&cmdvp,0x110a+vocflag,36,101);
	clrvp (&textvp,8);
	fontcolor (&textvp,11,-1);
	wprint (&textvp,150-strlen(botmsg)*3,0,2,botmsg);
	};

void cmd_win (void) {
	fontcolor (&cmdvp,13,0); // foreground, background
	clearvp (&cmdvp);
	wprint (&cmdvp,4,2,1,"SCORE");
		drawshape (&cmdvp,0xd0c,1,12);		// 22x14 all status windows
		drawshape (&cmdvp,0xd0d,25,12);
	wprint (&cmdvp,3,32,2,"SHIELDS");
		drawshape (&cmdvp,0xd0a,2,40);
		drawshape (&cmdvp,0xd0b,24,40);
	wprint (&cmdvp,9,60,2,"BOMBS");
		drawshape (&cmdvp,0xd0c,1,68);
		drawshape (&cmdvp,0xd0d,25,68);
	drawshape (&cmdvp,0xd00,10,88);			// triple fire icon

	drawshape (&cmdvp,0x1109,3,100);
	fontcolor (&cmdvp,12,0);
	wprint (&cmdvp,21,101,2,"HELP");
//	wprint (&cmdvp,2,102,2,"SOUND");
	fontcolor (&statvp,10,0);
	clearvp (&statvp);
	wprint (&statvp,3,2,1,"SHIPS");
	stat_win ();
	};

void printhi (int16_t newhi) {
	int16_t c, posn;
	char s[10];
	defwin (&menu_win, 7, 40, 8, 6, 0, 0, textbox);	// 128x96
	drawwin (&menu_win);
	snd_play (1,9);
	if (newhi) {
		posn = numhighs;
		while ((posn > 0) && (pl.score > hiscore [posn-1])) posn--;
		if (posn >= numhighs) newhi = 0;
		else {
			for (c = numhighs - 2; c >= posn; c--) {
				hiscore [c + 1] = hiscore [c];
				strcpy (hiname [c + 1], hiname [c]);
				};
			hiscore [posn] = pl.score;
			hiname [posn][0] = '\0';
			};
		};
	fontcolor (&menu_win.inside, 15, -1);
	wprint (&menu_win.inside, 18, 4, 2, "Master Blasters");
	fontcolor (&menu_win.inside, 12, -1);
	wprint (&menu_win.inside, 15, 8, 2, "________________");
	fontcolor (&menu_win.inside, 10, -1);
	for (c = 0; c < numhighs; c++)
		wprint (&menu_win.inside, 8, 20+c*7, 2, hiname[c]);
	fontcolor (&menu_win.inside, 14, -1);
	for (c = 0; c < numhighs; c++) {
		wprint (&menu_win.inside, 120-(strlen(s)*6), 20+c*7, 2,
		ultoa (hiscore[c], s, 10));
		};

	if (newhi) {
		ultoa (hiscore [posn], s, 10);
		fontcolor (&menu_win.inside, 15, 1);
		wprint (&menu_win.inside, 8, 20+posn*7, 2, "        ");
		winput (&menu_win.inside, 8, 20+posn*7, 2, hiname[posn], 8);
		savecfg ();
		};
	if (newhi==0) rest (1);
	};

void instructions (int16_t page) {
	int16_t x, y;
	char tempstr[2];
	wprint (&statvp, 20, 22, 1, itoa (page, tempstr, 10));
	switch (page) {
		case 1:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 10, -1);
			wprint (&gamevp, 32, 24, 1, "- - - INSTRUCTIONS - - -");
			wprint (&gamevp, 36, 154, 1, "- - - - - - - - - - - -");
			fontcolor (&gamevp, 14, -1);
			wprint (&gamevp, 22, 40, 1, "To FIRE press ");
			wprint (&gamevp, 172, 40, 1, "OR ");
			wprint (&gamevp, 22, 56, 1, "BOMBS or MISSILES ");
			fontcolor (&gamevp, 11, -1);
			wprint (&gamevp, 22, 72, 1, "Move LEFT ");
			wprint (&gamevp, 120, 72, 1, "Move RIGHT ");
			wprint (&gamevp, 22, 88, 1, "Move UP ");
			wprint (&gamevp, 104, 88, 1, "Move DOWN ");
			fontcolor (&gamevp, 14, -1);
			wprint (&gamevp, 22, 104, 1, "Press 'P' to PAUSE game");
			wprint (&gamevp, 22, 122, 1, "Press 'S' to SAVE a game");
			wprint (&gamevp, 22, 138, 1, "Press 'L' to LOAD a game");
			drawshape (&gamevp, 0x1100, 134, 40);
			drawshape (&gamevp, 0x1103, 198, 40);
			drawshape (&gamevp, 0x1101, 166, 56);
			drawshape (&gamevp, 0x1107, 102, 72);
			drawshape (&gamevp, 0x1106, 208, 72);
			drawshape (&gamevp, 0x1104, 86, 88);
			drawshape (&gamevp, 0x1105, 184, 88);
			pageflip (); setpagemode (0); break;
		case 2:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 10, -1);
			wprint (&gamevp, 32, 24, 1, "- - - INSTRUCTIONS - - -");
			wprint (&gamevp, 36, 154, 1, "- - - - - - - - - - - -");
			fontcolor (&gamevp, 14, -1);
			wprint (&gamevp, 32, 40, 1,  "   Press     for HELP   ");
			wprint (&gamevp, 32, 56, 1,  " F2 enables EXPERT mode ");
			wprint (&gamevp, 32, 88, 1,  " F3 enables GRANNY mode ");
			wprint (&gamevp, 32, 122, 1, "CTRL-S toggles the SOUND");
			wprint (&gamevp, 87, 138, 1, "or 'Q' to QUIT");
			fontcolor (&gamevp, 11, -1);
			wprint (&gamevp, 32, 73, 1,  "   NOT FOR THE TIMID!   ");
			wprint (&gamevp, 36, 104, 1, "    (For Beginners)    ");
			drawshape (&gamevp, 0x1108, 57, 138);
			drawshape (&gamevp, 0x1109, 108, 40);
			pageflip (); setpagemode (0);
		};
	};

void order (int16_t page) {
	int16_t x, y;
	char tempstr[2];
	wprint (&statvp, 20, 22, 1, itoa (page, tempstr, 10));

	switch (page) {
		case 1:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 15, -1);
		wprint (&gamevp, 28, 24, 1, "- - - ORDERING INFO - - -");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 40,  2, "Kiloblaster is a trilogy  of three");
		wprint (&gamevp, 26, 50,  2, "exciting arcade-style games. There");
		wprint (&gamevp, 26, 60,  2, "is plenty of action to satisfy the");
		wprint (&gamevp, 26, 70,  2, "most avid game player.            ");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 80,  2, "There are 30 unique levels in each");
		wprint (&gamevp, 26, 90,  2, "volume for a total of 90 levels of");
		wprint (&gamevp, 26, 100, 2, "intense game play.                ");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 110, 2, "Kiloblaster features: sizzling hot");
		wprint (&gamevp, 26, 120, 2, "graphics, digitized sound effects,");
		wprint (&gamevp, 26, 130, 2, "a musical sound track and joystick");
		wprint (&gamevp, 26, 140, 2, "support. Don't miss the conclusion");
		wprint (&gamevp, 26, 150, 2, "to this outer-space thriller in:  ");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 160, 2, "     <<< the final battle >>>     ");
		pageflip (); setpagemode (0); break;
		case 2:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 15, -1);
		wprint (&gamevp, 28, 24, 1, "- - - ORDERING INFO - - -");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 40,  2, "Order any volume for only  $15 or,");
		wprint (&gamevp, 26, 50,  2, "you can get all 3 volumes for just");
		wprint (&gamevp, 26, 60,  2, "$30. As a bonus you will get:     ");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 70,  2, " > A disk of bonus games          ");
		wprint (&gamevp, 26, 80,  2, " > Hint sheet--Cheat mode password");
		wprint (&gamevp, 26, 90,  2, " > Up-to-date news of new releases");
		wprint (&gamevp, 26, 100, 2, " > 3 gigantic, action-packed games");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 110, 2, "Send Check/MO     | Call toll free");
		wprint (&gamevp, 26, 120, 2, "Epic Megagames    | 1-800-972-7434");
		wprint (&gamevp, 26, 130, 2, "10406 Holbrook Dr.|     Visa/MC   ");
		wprint (&gamevp, 26, 140, 2, "Potomac, MD 20854 |  (orders only)");
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 26, 150, 2, "   Add $2 shipping and handling   ");
		wprint (&gamevp, 23, 160, 2, "Maryland residents add 5% sales tax");
		pageflip (); setpagemode (0);
		};
	};

void about (int16_t page) {
	int16_t x, y;
	char tempstr[2];
	wprint (&statvp, 20, 22, 1, itoa (page, tempstr, 10));
	switch (page) {
		case 1:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 28, 24, 1, "ABOUT EPIC MEGAGAMES. . .");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 40,  2, "   It all began in January of 1991");
		wprint (&gamevp, 26, 50,  2, "when Tim released the original ZZT");
		wprint (&gamevp, 26, 60,  2, "series. Little did he know that it");
		wprint (&gamevp, 26, 70,  2, "would be so successful. It has now");
		wprint (&gamevp, 26, 80,  2, "been released in Canada, Australia");
		wprint (&gamevp, 26, 90,  2, "and even Europe.                  ");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 100, 2, "   Tim Sweeney is  the founder and");
		wprint (&gamevp, 26, 110, 2, "president of Epic MegaGames. He is");
		wprint (&gamevp, 26, 120, 2, "also a  student at the  University");
		wprint (&gamevp, 26, 130, 2, "of Maryland. (Yes, he keeps busy!)");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 140, 2, "   Thanks to the support of people");
		wprint (&gamevp, 26, 150, 2, "like you we have been able to grow");
		wprint (&gamevp, 26, 160, 2, "and bring you more quality games. ");
		pageflip (); setpagemode (0); break;
		case 2:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 28, 24, 1, "ABOUT EPIC MEGAGAMES. . .");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 40,  2, "   Our team now consists of twenty");
		wprint (&gamevp, 26, 50,  2, "programmers,  artists,  musicians,");
		wprint (&gamevp, 26, 60,  2, "and game designers who are willing");
		wprint (&gamevp, 26, 70,  2, "to work hard to bring you the very");
		wprint (&gamevp, 26, 80,  2, "best games possible.              ");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 90,  2, "      - - - Our Vision - - -      ");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 100, 2, "   To create high-quality games to");
		wprint (&gamevp, 26, 110, 2, "satisfy our customers.  We believe");
		wprint (&gamevp, 26, 120, 2, "that you are very important and it");
		wprint (&gamevp, 26, 130, 2, "is our  main goal to release games");
		wprint (&gamevp, 26, 140, 2, "that you like to play.  Please let");
		wprint (&gamevp, 26, 150, 2, "us know of any questions, comments");
		wprint (&gamevp, 26, 160, 2, "or suggestions you may have.      ");
		pageflip (); setpagemode (0); break;
		case 3:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 28, 24, 1, "OTHER RELEASES FROM EPIC!");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 40,  2, "            Super ZZT:            ");
		wprint (&gamevp, 26, 80,  2, "           Best of ZZT:           ");
		wprint (&gamevp, 26, 120, 2, "          ZZT's Revenge:          ");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 50,  2, "The highly-demanded sequel to  ZZT");
		wprint (&gamevp, 26, 60,  2, "featuring a brilliantly colored 4-");
		wprint (&gamevp, 26, 70,  2, "way scrolling playfield.          ");
		wprint (&gamevp, 26, 90,  2, "A collection of the  best and most");
		wprint (&gamevp, 26, 100, 2, "intriquing  game  boards  from our");
		wprint (&gamevp, 26, 110, 2, "game design contest.              ");
		wprint (&gamevp, 26, 130, 2, "The latest and greatest ZZT series");
		wprint (&gamevp, 26, 140, 2, "ever created. This release has six");
		wprint (&gamevp, 26, 150, 2, "huge game worlds. It may be months");
		wprint (&gamevp, 26, 160, 2, "before you complete all of these. ");
		pageflip (); setpagemode (0); break;
		case 4:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 28, 24, 1, "OTHER RELEASES FROM EPIC!");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 40,  2, "           Drum Blaster           ");
		wprint (&gamevp, 26, 80,  2, "       Castle of the Winds:       ");
		wprint (&gamevp, 26, 120, 2, "        Jill of the Jungle        ");
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 50,  2, "Earth-shaking sound effects can be");
		wprint (&gamevp, 26, 60,  2, "played on your PC speaker. A Sound");
		wprint (&gamevp, 26, 70,  2, "blaster is recommended. Great fun!");
		wprint (&gamevp, 26, 90,  2, "A 2-part fantasy role playing game");
		wprint (&gamevp, 26, 100, 2, "set in Norse mythology.  Our first");
		wprint (&gamevp, 26, 110, 2, "release for windows.              ");
		wprint (&gamevp, 26, 130, 2, "A Nintendo-style arcade  game with");
		wprint (&gamevp, 26, 140, 2, "smooth-scrolling  animation,  lots");
		wprint (&gamevp, 26, 150, 2, "of action and some surprises.     ");
			fontcolor (&gamevp, 13, -1);
		wprint (&gamevp, 26, 160, 2, "  Thanks again for your support!  ");
		pageflip (); setpagemode (0);
		};
	};

void credit (int16_t page) {
	int16_t x, y;
	char tempstr[2];
	wprint (&statvp, 20, 22, 1, itoa (page, tempstr, 10));
	switch (page) {
		case 1:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 10, -1);
			wprint (&gamevp, 88, 37, 1, "Written by");
			wprint (&gamevp, 64, 51, 1, "Allen W. Pilgrim");
			fontcolor (&gamevp, 11, -1);
			wprint (&gamevp, 80, 85, 1, "Animation by");
			wprint (&gamevp, 80, 99, 1, "Bryon Nieman");
			fontcolor (&gamevp, 14, -1);
			wprint (&gamevp, 60, 133, 1, "Ray-Traced art by");
			wprint (&gamevp, 64, 147, 1, "Samuel Goldstein");
			pageflip (); setpagemode (0); break;
		case 2:
			clrvp (&gamevp, 0); setpagemode (1);
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
					drawshape (&gamevp, pcx_sh+x+y*16, x*16, y*16);
					};
				};
			fontcolor (&gamevp, 10, -1);
		wprint (&gamevp, 26, 24,  2, "  REALIZING THAT A PROJECT OF THIS");
		wprint (&gamevp, 26, 34,  2, "SIZE IS RARELY ACCOMPLISHED BY ONE");
		wprint (&gamevp, 26, 44,  2, "PERSON, I WOULD LIKE TO EXPRESS MY");
		wprint (&gamevp, 26, 54,  2, "DEEPEST THANKS TO  SEVERAL PEOPLE.");
			fontcolor (&gamevp, 11, -1);
		wprint (&gamevp, 26, 64,  2, "TO DAN FROELICH FOR HIS INCREDIBLE");
		wprint (&gamevp, 26, 74,  2, "MUSICAL PIECES.  TO  BRYON  NIEMAN");
		wprint (&gamevp, 26, 84,  2, "FOR HIS DRAWINGS AND  SUGGESTIONS.");
		wprint (&gamevp, 26, 94,  2, "TO MY BROTHER JEFF WHO CONTRIBUTED");
		wprint (&gamevp, 26, 104, 2, "MANY IDEAS AND  HELPED ME TEST IT.");
			fontcolor (&gamevp, 14, -1);
		wprint (&gamevp, 26, 114, 2, "SPECIAL THANKS TO TIM SWEENEY  WHO");
		wprint (&gamevp, 26, 124, 2, "PATIENTLY TAUGHT ME HOW TO PROGRAM");
		wprint (&gamevp, 26, 134, 2, "IN C! MOST OF ALL I WOULD TO THANK");
		wprint (&gamevp, 26, 144, 2, "MY WIFE KATRINA FOR HER SUPPORT OF");
		wprint (&gamevp, 26, 154, 2, "THIS PROJECT AND ENCOURAGEMENT!   ");
			fontcolor (&gamevp, 12, -1);
		wprint (&gamevp, 26, 164, 2, "SINCERELY,  ALLEN PILGRIM (AUTHOR)");
		pageflip (); setpagemode (0);
		};
	};

void help (void) {
	defwin (&menu_win, 4, 32, 11, 7, 0, 0, textbox);
	drawwin (&menu_win);
	snd_play (1,19);
	fontcolor (&menu_win.inside, 15, -1);
		wprint (&menu_win.inside, 16, 5, 1, "--- HELP MENU ---");
	fontcolor (&menu_win.inside, 12, -1);
		wprint (&menu_win.inside, 8, 17, 2, "SPACE BAR or SHIFT to fire");
		wprint (&menu_win.inside, 8, 27, 2, "ALT key launches missiles!");
	fontcolor (&menu_win.inside, 10, -1);
		wprint (&menu_win.inside, 8, 37, 2, "Press 'P' to pause game");
		wprint (&menu_win.inside, 8, 47, 2, "Press 'S' to save game");
		wprint (&menu_win.inside, 8, 57, 2, "Press 'L' to load game");
	fontcolor (&menu_win.inside, 11, -1);
		wprint (&menu_win.inside, 8, 67, 2, "F2 enables EXPERT mode");
		wprint (&menu_win.inside, 8, 77, 2, "F3 enables GRANNY mode");
	fontcolor (&menu_win.inside, 13, -1);
		wprint (&menu_win.inside, 8, 87, 2, "Ctrl-S toggles sound");
	fontcolor (&menu_win.inside, 14, -1);
		wprint (&menu_win.inside, 8, 97, 2, "Press ESC or 'Q' to QUIT");
	};

void cheat_msg (void) {
	defwin (&menu_win, 4, 40, 11, 6, 0, 0, textbox);
	drawwin (&menu_win);
	snd_play (1,19);
	fontcolor (&menu_win.inside, 15, -1);
		wprint (&menu_win.inside, 16, 6, 1, "EPIC'S CHEAT MODE");
	fontcolor (&menu_win.inside, 10, -1);
		wprint (&menu_win.inside, 8, 20, 2, "You have accessed a secret");
		wprint (&menu_win.inside, 8, 32, 2, "cheat code that gives you:");
	fontcolor (&menu_win.inside, 11, -1);
		wprint (&menu_win.inside, 8, 44, 2, "> Maximum shields     ");
		wprint (&menu_win.inside, 8, 56, 2, "> Maximum bombs       ");
		wprint (&menu_win.inside, 8, 68, 2, "> Maximum triple fire ");
	fontcolor (&menu_win.inside, 14, -1);
		wprint (&menu_win.inside, 11, 80, 2, "   - Press any key -   ");
	};

int16_t askquit (void) {
	defwin (&menu_win, 4, 72, 11, 2, 0, 0, textbox);
	drawwin (&menu_win);
	snd_play (1,19);
	fontcolor (&menu_win.inside, 14, -1);
	wprint (&menu_win.inside, 23, 8, 2, "Do you really want to");
	wprint (&menu_win.inside, 8, 18, 2, "leave all this excitement?");
	rest (2); key = toupper(key);
	return (key);
	};

void rest (int16_t num) {
	switch (num) {
		case 1:
			do {checkctrl0 (0);}	while (key != escape); break;
		case 2:
			do {checkctrl0 (0);}
			while ((key==0)&&(fire1==0)&&(fire2==0)&&(dx1==0)&&(dy1==0));
			break;
		case 3:
			do {checkctrl0 (0);}
			while ((key!=escape)&&(key!=200)&&(key!=208)); break;
		case 4:
			do {
				checkctrl0 (0);
				upd_objs ();
				upd_end ();
				refresh (pagemode);
				purgeobjs ();
			} while (key != escape); break;
		};
	};

void cmds (void) {
	fontcolor (&cmdvp, 12, 8);
	clearvp (&cmdvp);
	wprint (&cmdvp, 0, 4, 2, "COMMANDS");
	fontcolor (&cmdvp, 11, 8);
	wprint (&cmdvp, 8, 16, 1, "NEXT");
	wprint (&cmdvp, 8, 28, 1, "PAGE");
	drawshape (&cmdvp, 0x1105, 18, 40);
	wprint (&cmdvp, 8, 52, 1, "LAST");
	wprint (&cmdvp, 8, 64, 1, "PAGE");
	drawshape (&cmdvp, 0x1104, 18, 76);
	wprint (&cmdvp, 8, 88, 1, "EXIT");
	drawshape (&cmdvp, 0x1108, 13, 100);
	fontcolor (&statvp, 13, 0);
	clearvp (&statvp);
	wprint (&statvp, 8, 10, 1, "PAGE");
	};

void music (void) {
	if (pl.level==4 || pl.level==7 || pl.level==10 ||
		pl.level==13 || pl.level==16 || pl.level==19 ||
		pl.level==22 || pl.level==25 || pl.level==28) {
		sb_playtune (song1[pl.level]);
		};
	};

int16_t loadsavewin (char *msg, char *blankmsg) {
	static int16_t cur = 0;
	int16_t c;
	char s[12]; char tempstr[2];
	dx1hold = 1; dy1hold = 1; fire1off = 1;
	defwin (&menu_win, 9, 32, 6, 7, 0, 0, textbox);	// 96x112
	drawwin (&menu_win);
	fontcolor (&menu_win.inside, 15, -1);
	wprint (&menu_win.inside, 12, 4, 1, msg);
	fontcolor (&menu_win.inside, 12, -1);
	wprint (&menu_win.inside, 0, 8, 1, "____________");
	wprint (&menu_win.inside, 0, 84, 1, "____________");
	fontcolor (&menu_win.inside, 11, -1);
	for (c = 0; c < numsaves; c++)
		wprint (&menu_win.inside, 16, 20+c*10, 2, itoa(c+1, s, 10));
	fontcolor (&menu_win.inside, 14, -1);
	for (c = 0; c < numsaves; c++) {
		if (strlen(savename[c]) != 0)
			wprint (&menu_win.inside, 28, 20+c*10, 2, savename[c]);
		else
			wprint (&menu_win.inside, 28, 20+c*10, 2, blankmsg);
			};
	fontcolor (&menu_win.inside, 11, -1);
	wprint (&menu_win.inside, 6, 100, 2, "<ESC>");
	wprint (&menu_win.inside, 42, 100, 2, "TO ABORT");
	fontcolor (&menu_win.inside, 15, 1);
	for (c = 0; c < 11; c++)
		wprint (&menu_win.inside, 4, 20 + c * 6, 2, " ");

	do {
		tempstr[1] = 0;
		checkctrl0 (0);
		c = (c & 7) + 1; tempstr[0] = c;
		wprint (&menu_win.inside, 4, 20 + cur * 10, 2, tempstr);
		delay (100);
		wprint (&menu_win.inside, 4, 20 + cur * 10, 2, " ");
		cur = (max(0, min(cur + dx1 + dy1, numsaves - 1)));
		} while ((!fire1) && (key != enter) && (key != escape));
	if (key==escape) return (-1);
	return (cur);
	};

int16_t loadgame (void) {
	int16_t num, gamefile, c;
	char tempstr[16];
	char boardname[32];

	num = loadsavewin ("LOAD GAME", "<empty>");
	if ((num >= 0) && (strlen(savename[num]) != 0)) {
		itoa (num, tempstr, 10);
//		strcpy (boardname,path);
		strcpy (boardname, gamename);
		strcat (boardname, ".");
		strcat (boardname, tempstr);
		gamefile = _open (boardname, O_BINARY);
		if ((gamefile < 0) || (filelength(gamefile) <= 0)) {
			close (gamefile);
			}
		else {
			read (gamefile, &o_pl, sizeof(o_pl));
			read (gamefile, &o_extra, sizeof(o_extra));
			};
		close (gamefile);
		return (1);
		};
	return (0);
	};

void savegame (void) {
	int16_t num, gamefile;
	char s[savelen];
	char tempstr[16];
	char boardname[32];

	num = loadsavewin ("SAVE GAME", "");
	if (num >= 0) {
		strcpy (s, savename[num]);
		fontcolor (&menu_win.inside, 15, 1);
		wprint (&menu_win.inside, 28, 20+num*10, 2, "         ");
		winput (&menu_win.inside, 28, 20+num*10, 2, s, 9);
		if ((key != escape) && (strlen(s) != 0)) {
			strcpy (savename[num], s);
			itoa (num, tempstr, 10);
//			strcpy (boardname,path);
			strcpy (boardname, gamename);
			strcat (boardname, ".");
			strcat (boardname, tempstr);
			gamefile = _creat (boardname, 0644);
			if (gamefile >= 0) {
				write (gamefile, &o_pl, sizeof(o_pl));
				write (gamefile, &o_extra, sizeof(o_extra));
				};
			close (gamefile);
			savecfg ();
			};
		};
	};

void o_stats (void) {
	memcpy (&o_pl, &pl, sizeof(pltype));
	o_extra = extra_ship;
	};

void n_stats (void) {
	memcpy (&pl, &o_pl, sizeof(pltype));
	extra_ship = o_extra;
	};

void play (int16_t demoflg, int16_t loadflg) {
	int16_t n, t, begclock, temppage;
	int16_t speed = 0; int16_t turtle = 0;
	if (loadflg) {
		n_stats ();
		statmodflg |= mod_screen;
		cmd_win ();
		}
	else extra_ship = 60000;

	if (!demoflg) snd_play (8,14);
	sb_playtune (song1[pl.level]);

	o_stats ();
	level(demoflg); drawboard (); setpagemode (1);
	text (k_msg2,1);
	gameover = 0; gamecount = 0; winflg = 0;

	  do	{
		begclock = getclock();
		gamecount++;
		checkctrl (1);  // 0 for standard repeat, 1 to check key hold
		switch (toupper (key)) {
			case 'P':		// Pause mode
				text ("Game is now paused--Press any key to continue",0);
				refresh (pagemode);
				rest (2); text (k_msg2,1); break;
			case 19:		// Sound Toggle
				soundf=!soundf; break;
			case k_f1:
				pageflip (); setpagemode (0);
				help (); rest (2);
				setpagemode (1);
				moddrawboard (); break;
			case k_f2: speed=!speed; break;
			case k_f3: turtle=!turtle; break;
//			case k_f4: if (macrecord) macrecend (); break;
			case 'S':		// SAVE
				temppage = pagedraw; pagedraw = pageshow; setpages ();
				savegame (); cmd_win ();
				pagedraw = temppage; setpages ();
				moddrawboard (); break;
			case 'L':		// LOAD
				temppage = pagedraw; pagedraw = pageshow; setpages ();
				if (loadgame()==1) {
					pagedraw = temppage; setpages ();
					refresh (pagemode);	pageflip (); setpagemode (0);
					pageflip (); setpagemode (0);
					n_stats (); statmodflg |= mod_screen; cmd_win ();
					sb_playtune (song1[pl.level]);	o_stats ();
					level(0); drawboard (); setpagemode (1);
					gameover = 0; gamecount = 0; winflg = 0;
					}
				else {
					pagedraw = temppage; setpages (); moddrawboard ();
					}; break;
			case escape:
			case 'Q':
				if (!winflg) {
					pageflip (); setpagemode (0);
					askquit ();
					setpagemode (1);
					moddrawboard ();
					if (key == 'Y') gameover = 1;
					}; break;
			case '=':		// Cheat mode
				rest (2); key = toupper(key);
				if (key=='`') {
					pageflip (); setpagemode (0);
					cheat_msg (); rest (2);
					setpagemode (1); moddrawboard (); snd_play (2,12);
					for (t = 0; t < 4; t++) {
						if (pl.health < 5) pl.health++;
						};
					for (t = 0; t < 30; t++) {
						if (inv_cnt(inv_triple) <= 29)
							add_inv (inv_triple);
						};
					for (t = 0; t < 4; t++) {
						if (inv_cnt(inv_bomb) < 5) add_inv(inv_bomb);
						};
					statmodflg |= mod_screen;
					};
				if (key=='I') {	// Cheat mode (invincibility)
					snd_play (2,17);
					text ("You are temporarily * INVINCIBLE *",0);
					addobj (obj_shield, objs[0].x, objs[0].y, 0, 0);
					};
				if (key=='W') {	// Cheat mode (extra wingmen)
					snd_play (2,13);
					if (class_cnt(class_wing1)==0)
						addobj (obj_wing1, objs[0].x, objs[0].y, 0, 0);
					if (class_cnt(class_wing2)==0)
						addobj (obj_wing2, objs[0].x, objs[0].y, 0, 0);
					if ((pl.level==5)||(pl.level==10)||(pl.level==15)||
					(pl.level==20)||(pl.level==25)||(pl.level==30)) {
					if (class_cnt(class_wing3)==0)
						addobj (obj_wing3, objs[0].x, objs[0].y, 0, 0);
					if (class_cnt(class_wing4)==0)
						addobj (obj_wing4, objs[0].x, objs[0].y, 0, 0);
						};
					text ("You have WINGMEN for a limited time",0);
					}; break;
			case k_f5:		// Zap key
				for (n = 1; n < num_objs; n++) {
					killobj (n);
					};
				weapon_cnt = 0; fire_cnt = 0;
				objs[0].count2 = 0;
				};

			upd_objs ();
			if (speed) upd_objs ();
			upd_objs2 ();
			refresh (pagemode);
			purgeobjs ();
			if ((class_cnt(class_enemy)==0)&&(enemy_flag==0)&&
				(objs[0].state!=st_die)) {
				pl.level++; o_stats ();
				if (pl.level < 31) {
					music ();
					refresh (pagemode);
					pageflip (); setpagemode (0);
					level(0); drawboard (); setpagemode(1);
					}
				else {
					sb_playtune (song1[31]);
					game_end ();
					};
				};
			if (pl.score >= extra_ship) {
				if (inv_cnt(inv_player) < 3) {
					extra_ship += 100000; add_inv(inv_player);
					snd_play (7,19);
					text ("Congratulations! You earned an EXTRA ship!",0);
					}
				else {
					extra_ship += 100000; pl.score += 20000;
					snd_play (2,19);
					text ("$20,000 POINT BONUS",0);
					};
				};
			if (cnt_down > 0) cnt_down--;
			if ((cnt_down==0) && (text_flg==0)) text (k_msg2,1);
			if ((demoflg) && (!macplay)) gameover = 1;
		while ((getclock()-begclock) < (turtle+1)) continue;
	} while (!gameover);
	if ((!winflg) && (!demoflg)) {
		text ((char*)end_msg[xr_random(5)],0); refresh (pagemode);
		setpagemode (0); printhi (1);
		};
	};

int main (int argc, char *argv[]) {
	int16_t n;
	strcpy (tempname,"temp");
	loadcfg ();
	scrn_1 ();					// beginning screen
	cfg_init (argc, argv);
	snd_init (audioname);
	gc_init ();					// initialize gamectrl
	if (!doconfig()) goto abortstart;
	gr_init ();					// init graphics
	clrpal ();
	savecfg ();

	shm_init (shapename);

	shm_want [1]=1; 	// Tell SHM we need these shape tables in memory!
	shm_want [2]=1;
	shm_want [7]=1;

	shm_do ();
	clearvp (&mainvp);
	wait ();
	snd_do ();
	sb_playtune (song1[0]);

	shm_want [3]=1;
	shm_want [4]=1;
	shm_want [5]=1;
	shm_want [6]=1;
	shm_want [8]=1;
	shm_want [9]=1;
	shm_want [10]=1;
	shm_want [11]=1;
	shm_want [12]=1;
	shm_want [13]=1;
	shm_want [14]=1;
	shm_want [15]=1;
	shm_want [16]=1;
	shm_want [17]=1;
	shm_want [18]=1;
	shm_want [19]=1;
	shm_want [20]=1;
	shm_want [21]=1;
	shm_want [22]=1;
	shm_want [23]=1;
	shm_want [24]=1;

//TEMP	intro ();

	init_objinfo ();
	init_info ();
	init_brd ();
	init_win ();
	init_inv ();
	cmd_win ();
	do {checkctrl0(0);}
		while ((dx1!=0)||(dy1!=0)||(key!=0)||(fire1)||(fire2));
	menu ();

	fadeout();
	shm_exit();
	gr_exit();
	abortstart:
	gc_exit();
	snd_exit();
	}

void rexit (int16_t n) {
	char errnum[2];

	shm_exit();
	gr_exit();
	gc_exit();
	snd_exit();

	clrscr();
	scrn_1 ();
	gotoxy (1,5);
	clrscr();
	cputs ("Sorry, error <");
	cputs (itoa(n,errnum,10));
	cputs ("> has occurred.\r\n");
	cputs ("Please type HELPME for troubleshooting information.\r\n");
	cputs ("\r\n");
	if (n==9) {
		cputs ("  Problem: Not enough free RAM to run KILOBLASTER.\r\n");
		cputs ("Solutions: Boot clean with no TSR's loaded.\r\n");
		if (vocflag) {
			cputs ("           Turn off SOUND BLASTER sound effects.");
			};
		}; delay (1000); window (1,1,80,25); gotoxy (1,25); exit(1);
	};
