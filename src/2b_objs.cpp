//	2B_OBJS.C
//
// Kiloblaster v.2 Objects
//
// Written by Allen W. Pilgrim

#include <stdlib.h>
#include <string.h>
#include "\develop\kilo2\include\gr.h"
#include "\develop\kilo2\include\keyboard.h"
#include "\develop\kilo2\include\windows.h"
#include "\develop\kilo2\include\gamectrl.h"
#include "\develop\kilo2\include\music.h"
#include "\develop\kilo2\include\2blaster.h"

extern char k_msg2[];

int msg_bullet2 (int n, int msg, int z) {		 // enemy bullet
	int sh, dx, dy;
	switch (msg) {
		case msg_update:
			objs[n].count=(objs[n].count+1)&7;
			seekplayer (n,&dx,&dy);
			objs[n].x+=dx*2; objs[n].y+=objs[n].yd;
			objs[n].yd++;
			if (objs[n].y>=scrn_y) killobj (n); return (1);
		case msg_draw:
			sh=0x50b+objs[n].count/2;
			drawshape (&gamevp,sh,objs[n].x,objs[n].y); break;
		}; return (0);
	};

int msg_spinner (int n, int msg, int z) {		 // enemy bullet
	int sh, dx, dy;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			seekplayer (n, &dx, &dy);
			objs[n].x += dx*2; objs[n].y += objs[n].yd;
			objs[n].yd++;
			if (objs[n].y >= scrn_y) killobj (n); return (1);
		case msg_draw:
			sh = 0x503 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y); break;
		}; return (0);
	};

void explosion (int x, int y, int num) {
	int c;
	for (c = 0; c < num; c++) {
		addobj (obj_explode4, x, y, 0, 0);
		objs[num_objs-1].xd = random (7) - 3;
		objs[num_objs-1].yd = random (11) - 8;
		};
	};

int msg_explode1 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 16) killobj (n); return (1);
		case msg_draw:
			sh = 0xf00 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_explode2 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 14) killobj (n); return (1);
		case msg_draw:
			sh = 0xf0a + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_explode3 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 12) killobj (n); return (1);
		case msg_draw:
			sh = 0xf00 + 21 + objs[n].count/4;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_explode4 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].count3==0) {
				switch (random(6)) {
					case 0: objs[n].count3 = 1; break;
					case 1: objs[n].count3 = 2; break;
					case 2: objs[n].count3 = 3; break;
					case 3: objs[n].count3 = 4; break;
					case 4: objs[n].count3 = 5; break;
					case 5: objs[n].count3 = 6;
					};
				};
			if (++objs[n].count2 >= 30) killobj (n);
			if (++objs[n].yd > 6) objs[n].yd = 6;
			move_obj (n, objs[n].x+objs[n].xd, objs[n].y+objs[n].yd);
			if (objs[n].y+objs[n].yl>=scrn_y) objs[n].yd=-objs[n].yd;
			return (1);
		case msg_draw:
			if (objs[n].count3==1) sh = 0x1000 + objs[n].count/2;
			if (objs[n].count3==2) sh = 0x1004 + objs[n].count/2;
			if (objs[n].count3==3) sh = 0x1008 + objs[n].count/2;
			if (objs[n].count3==4) sh = 0x100c + objs[n].count/2;
			if (objs[n].count3==5) sh = 0x1010 + objs[n].count/2;
			if (objs[n].count3==6) sh = 0x1014 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_explode5 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count2++ >= 20) killobj (n); return (1);
		case msg_draw:
			sh = 0xd04 + random(5);
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_explode6 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 20) killobj (n); return (1);
		case msg_draw:
			sh = 0x1700 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_triple (int n, int msg, int z) {		// triple fire power
	int sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].x > scrn_x) killobj(n);
			if (objs[n].count2++ == 20) {
				objs[n].yd = -objs[n].yd;
				objs[n].count2 = 0;
				};
			objs[n].x += objs[n].xd; objs[n].y += objs[n].yd; return (1);
		case msg_draw:
			sh = 0xd00 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_bonus1 (int n, int msg, int z) {		// pulsar--gives missiles
	int sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			sh = 0x1600 + 80 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_bonus2 (int n, int msg, int z) {		// spinach--invincibility
	switch (msg) {
		case msg_update:
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0x1200, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_bonus3 (int n, int msg, int z) {		// apple--shields
	switch (msg) {
		case msg_update:
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0x1201, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_bonus4 (int n, int msg, int z) {		// banana--points
	switch (msg) {
		case msg_update:
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0x1202, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_bonus5 (int n, int msg, int z) {		// strawberry--wingmen
	switch (msg) {
		case msg_update:
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0x1203, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_stars (int n, int msg, int z) {		// stars--just for looks!
	int sh;
	switch (msg) {
		case msg_draw:
			sh = 0xe00 + random (8);
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (1);
	};

int msg_falling (int n, int msg, int z) {	// falling star--just for looks!
	switch (msg) {
		case msg_update:
			objs[n].x += objs[n].xd; objs[n].y += objs[n].yd;
			if (objs[n].y >= scrn_y) killobj(n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0xe01, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_score (int n, int msg, int z) {
	int c;
	char tempstr[12];
	switch (msg) {
		case msg_update:
			if (objs[n].count==0) {
				pl.score += points;	statmodflg |= mod_screen;
				objs[n].count2 = points; objs[n].count = 1;
				objs[n].xl = strlen(ultoa(points, tempstr, 10))*8;
					if (objs[n].x + objs[n].xl >= scrn_x)
						objs[n].x=scrn_x-objs[n].xl;
				};
			objs[n].x += objs[n].xd; objs[n].y += objs[n].yd;
			if (++objs[n].count3==40) killobj(n); return (1);
		case msg_draw:
			c = random(5) + 10;
			fontcolor (&gamevp, c, -1);
			wprint (&gamevp, objs[n].x, objs[n].y, 1,
			ultoa (objs[n].count2, tempstr, 10));
		}; return (0);
	};

int msg_level1 (int n, int msg, int z) {
	int c;
	char tempstr[4];
	char dest[12];
	char *src1 = "LEVEL ", *src2 = itoa(pl.level, tempstr, 10);
	strcpy(dest, src1);
	strcat(dest, src2);

	switch (msg) {
		case msg_update:
			if (++objs[n].count2==50) objs[n].yd = -8;
			objs[n].y += objs[n].yd;
			if (objs[n].y + 8 < 0) killobj(n); return (1);
		case msg_draw:
			c = random(5) + 10;
			fontcolor (&gamevp, c, -1);
			wprint (&gamevp, objs[n].x, objs[n].y, 1, dest);
		}; return (0);
	};

int msg_shield (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count2 >= 240)
				objs[n].count = (objs[n].count + 1)&3;
			move_obj (n, objs[0].x-2, objs[0].y-2);
			if (++objs[n].count2==340) killobj (n); return (1);
		case msg_draw:
			sh = 0x804 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y); break;
		case msg_touch:
			switch (objs[z].kind) {
				case obj_enemy1:
				case obj_enemy2:
				case obj_enemy3:
				case obj_enemy4:
				case obj_enemy5:
				case obj_enemy6:
				case obj_enemy7:
				case obj_enemy8:
				case obj_enemy9:
				case obj_enemya:
				case obj_enemyb:
				case obj_enemyc:
				case obj_enemyd:
				case obj_enemye:
				case obj_enemyf:
				case obj_enemyg:
				case obj_enemyh:
				case obj_enemyi:
				case obj_enemyj:
				case obj_enemyk:
					snd_play (1,7);
					if (objs[z].count3 < 1) {
						killobj (z);
						}
					else {
						objs[z].xd = -objs[z].xd;
						objs[z].yd = -objs[z].yd;
						}; break;
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					snd_play (1,7);
					explosion (objs[z].x, objs[z].y, 5);
					killobj (z);
				};
		}; return (0);
	};

int msg_laser (int n, int msg, int z) {		 // enemy laser weapon
	switch (msg) {
		case msg_update:
			if (objs[n].count2++ > 2) killobj (n); return (1);
		case msg_draw:
			drawshape (&gamevp, 0x600, objs[n].x, objs[n].y); break;
		}; return (0);
	};

int msg_max (int n, int msg, int z) {		// max shields
	int sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			objs[n].y += objs[n].yd;
			if (objs[n].y > scrn_y) killobj(n); return (1);
		case msg_draw:
			sh = 0x160f + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_mines (int n, int msg, int z) {		// space mines
	int sh, x, y;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&3; return (1);
		case msg_draw:
			sh = 0xf00 + 18 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y); break;
		case msg_touch:
			switch (objs[z].kind) {
				case obj_player:
					if (class_cnt(class_shield)==0) {
						p_hit (2); snd_play (1,5);
						x = objs[n].x - 11; y = objs[n].y - 9;
						addobj (obj_explode3, x, y, 0, 0);
						explosion (objs[n].x, objs[n].y, 5);
						text ("You hit an enemy space mine!",0);
						killobj (n);
						}; break;
				case obj_shield:
					addobj (obj_explode2, objs[n].x, objs[n].y, 0, 0);
					explosion (objs[n].x, objs[n].y, 5);
					text ("Your shield was destroyed by a space mine",0);
					killobj (n); killobj (z); snd_play (1,5);
				};
		}; return (0);
	};

void eject (int x, int y, int num) {
	int c;
	for (c = 0; c < num; c++) {
		addobj (obj_ejected, x, y, 0, 0);
		objs[num_objs-1].xd = random (7) - 3;
		objs[num_objs-1].yd = random (11) - 8;
		};
	};

int msg_ejected (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&3;
			if (++objs[n].count2 >= 20) killobj (n);
			if (++objs[n].yd > 12) objs[n].yd = 12;
			move_obj (n,objs[n].x+objs[n].xd,objs[n].y+objs[n].yd);
			return (1);
		case msg_draw:
			sh = 0x1403 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_killme (int n, int msg, int z) {
	return (0);
	};

int msg_demo (int n, int msg, int z) {		// DEMO message
	switch (msg) {
		case msg_draw:
			drawshape (&gamevp, 0xd0e, 8, 8);
		}; return (1);
	};

int msg_jump1 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 28) {
				killobj (n);
				objs[0].y = scrn_y - 22;
				objs[0].count2 = 0;
				text (k_msg2, 1);
				}; return (1);
		case msg_draw:
			sh = 0x1800 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};

int msg_jump2 (int n, int msg, int z) {
	int sh;
	switch (msg) {
		case msg_update:
			if (objs[n].count++ >= 13) killobj (n); return (1);
		case msg_draw:
			sh = 0x1800 + objs[n].count/2;
			drawshape (&gamevp, sh, objs[n].x, objs[n].y);
		}; return (0);
	};