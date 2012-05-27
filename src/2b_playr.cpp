//	2B_PLYR.C
//
// Kiloblaster v.2 Player control routines
//
// Written by Allen W. Pilgrim

#include <stdlib.h>
#include "include/gr.h"
#include "include/keyboard.h"
#include "include/windows.h"
#include "include/gamectrl.h"
#include "include/music.h"
#include "include/2blaster.h"

int16_t msg_player (int16_t n, int16_t msg, int16_t z) {
	int16_t sh, t;
	switch (msg) {
		case msg_update: switch (objs[n].state) {
			case st_begin:
				objs[n].count = (objs[n].count + 1)&7;
				objs[n].x += dx1 * 8; objs[n].y += dy1 * 8;

		if (objs[n].count2==0) {
			if (objs[n].x < 0) objs[n].x = 0;
			if (objs[n].x>=scrn_x-objs[n].xl) objs[n].x=scrn_x-objs[n].xl;
			if (objs[n].y <= 48) objs[n].y = 48;
			if (objs[n].y>=scrn_y-objs[n].yl) objs[n].y=scrn_y-objs[n].yl;
			}
		else {
			objs[n].x = scrn_x/2 - objs[n].xl/2;
			objs[n].y = scrn_y + 18;
			};

			if ((fire2) && (weapon_cnt==0) && (objs[n].count2==0)) {
				if (take_inv (inv_bomb)) {
					snd_play (1,11);
					addobj (obj_missile,objs[n].x+1,objs[n].y-10,0,-4);
				if (class_cnt(class_wing1)==1) {
					addobj (obj_bomb, wing1_x+1, wing1_y-8, 0, -4);
					};
				if (class_cnt(class_wing2)==1) {
					addobj (obj_bomb, wing2_x+1, wing2_y-8, 0, -4);
					};
				if (class_cnt(class_wing3)==1) {
					addobj (obj_bomb, wing3_x+1, wing3_y-8, 0, -4);
					};
				if (class_cnt(class_wing4)==1) {
					addobj (obj_bomb, wing4_x+1, wing4_y-8, 0, -4);
					};
				};
			};

			if ((key==(' ')||(fire1)) && (fire_cnt==0) &&
				(objs[n].count2==0)) {
				if (take_inv(inv_triple)) {
					snd_play (1,10);
					addobj (obj_bullet, objs[n].x+7, objs[n].y-4, 1,-4);
					addobj (obj_bullet, objs[n].x+3, objs[n].y-4,-1,-4);
					addobj (obj_bullet, objs[n].x+5, objs[n].y-4, 0,-4);
				if (class_cnt(class_wing1)==1) {
					addobj (obj_bullet, wing1_x+3, wing1_y-4, -1, -4);
					addobj (obj_bullet, wing1_x+4, wing1_y-4,  0, -4);
					addobj (obj_bullet, wing1_x+5, wing1_y-4,  1, -4);
					};
				if (class_cnt(class_wing2)==1) {
					addobj (obj_bullet, wing2_x+3, wing2_y-4, -1, -4);
					addobj (obj_bullet, wing2_x+4, wing2_y-4,  0, -4);
					addobj (obj_bullet, wing2_x+5, wing2_y-4,  1, -4);
					};
				if (class_cnt(class_wing3)==1) {
					addobj (obj_bullet, wing3_x+3, wing3_y-4, -1, -4);
					addobj (obj_bullet, wing3_x+4, wing3_y-4,  0, -4);
					addobj (obj_bullet, wing3_x+5, wing3_y-4,  1, -4);
					};
				if (class_cnt(class_wing4)==1) {
					addobj (obj_bullet, wing4_x+3, wing4_y-4, -1, -4);
					addobj (obj_bullet, wing4_x+4, wing4_y-4,  0, -4);
					addobj (obj_bullet, wing4_x+5, wing4_y-4,  1, -4);
					};
				}
				else {
					snd_play (1,15);
					addobj (obj_platinum,objs[n].x+3,objs[n].y-10,0,-4);
				if (class_cnt(class_wing1)==1) {
					addobj (obj_platinum, wing1_x+1, wing1_y-10, 0, -4);
					};
				if (class_cnt(class_wing2)==1) {
					addobj (obj_platinum, wing2_x+1, wing2_y-10, 0, -4);
					};
				if (class_cnt(class_wing3)==1) {
					addobj (obj_platinum, wing3_x+1, wing3_y-10, 0, -4);
					};
				if (class_cnt(class_wing4)==1) {
					addobj (obj_platinum, wing4_x+1, wing4_y-10, 0, -4);
					};
				};
			}; break;

			case st_die:
				text ("Your fighter ship has been destroyed", 0);
				addobj (obj_explode3, objs[0].x-9, objs[0].y-4, 0, 0);
				explosion (objs[0].x+2, objs[0].y+2, 5);
				if (take_inv (inv_player)) {
					pl.health = 5; p_new ();
					}
				else {
					gameover = 1;
					}; break;
				}; break;

		case msg_draw:
			sh = 0x800 + objs[n].count/2;
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
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					if ((class_cnt(class_shield)==0) &&
						(objs[0].count2==0)) {
						p_hit (1);
						explosion (objs[n].x, objs[n].y, 5);
						if (--objs[z].count3 < 1) killobj (z);
					}; break;
				case obj_triple:		// triple fire power
					for (t = 0; t < 10; t++) {
						if (inv_cnt(inv_triple) <= 29)
							add_inv (inv_triple);
						};
					points = 1000;
					addobj (obj_score, objs[n].x, objs[n].y, 0, -2);
					addobj (obj_explode5, objs[n].x-4, objs[n].y-4,0,0);
					text ("You now have TRIPLE FIREPOWER!",0);
					snd_play (2,12); killobj (z); break;
				case obj_bonus1:		// pulsating orb
					if (inv_cnt(inv_bomb) < 5) {
					switch (xr_random(2)) {
						case 0:
							add_inv(inv_bomb);
							text ("You gained an EXTRA bomb!!!",0);
							break;
						case 1:
							add_inv(inv_bomb);
							add_inv(inv_split);
							text ("*** MULTIPLE WARHEAD MISSILE ***",0);
							};
						}
					else {
						points = 640; addobj
						(obj_score, objs[n].x, objs[n].y, -4, 2);
						};
					statmodflg |= mod_screen;
					addobj (obj_explode5, objs[n].x, objs[n].y, 0, 0);
					snd_play (1,6); killobj (z); break;
				case obj_bonus2:		// spinach
					points = 2400;
					if (objs[n].x < 128)
						addobj(obj_score, objs[n].x, objs[n].y, 4, -2);
					else
						addobj(obj_score, objs[n].x, objs[n].y, -4,-2);
					text ("You are temporarily * INVINCIBLE *",0);
					addobj (obj_explode5, objs[n].x-4, objs[n].y-4,0,0);
					addobj (obj_shield, objs[0].x, objs[0].y, 0, 0);
					snd_play (2,17); killobj (z); break;
				case obj_bonus3:		// apple
					if (pl.health < 5) {
						pl.health++;
						text ("Shield Level increased by ONE!",0);
						}
					else {
						points = 1224;
						addobj(obj_score,objs[n].x,objs[n].y,4,0);
						};
					statmodflg |= mod_screen;
					addobj (obj_explode5, objs[n].x-4, objs[n].y-4,0,0);
					snd_play (1,18); killobj (z); break;
				case obj_bonus4:		// banana
					points = 1000;
					addobj(obj_score, objs[n].x, objs[n].y, 0, -2);
					text ("1,000 Bonus Points!!!",0);
					addobj (obj_explode5, objs[n].x-4, objs[n].y-4,0,0);
					snd_play (1,18); killobj (z); break;
				case obj_bonus5:		// strawberry
					points = 400;
					if (class_cnt(class_wing1)==0) {
						addobj (obj_wing1, objs[0].x, objs[0].y, 0, 0);
						points += 400;};
					if (class_cnt(class_wing2)==0) {
						addobj (obj_wing2, objs[0].x, objs[0].y, 0, 0);
						points += 400;};
					if ((pl.level==5)||(pl.level==10)||(pl.level==15)||
					(pl.level==20)||(pl.level==25)||(pl.level==30)) {
					if (class_cnt(class_wing3)==0) {
						addobj (obj_wing3, objs[0].x, objs[0].y, 0, 0);
						points += 400;};
					if (class_cnt(class_wing4)==0) {
						addobj (obj_wing4, objs[0].x, objs[0].y, 0, 0);
						points += 400;};
						};
					addobj (obj_score, objs[n].x, objs[n].y, 0, -2);
					text ("You have WINGMEN for a limited time",0);
					addobj (obj_explode5, objs[z].x-4, objs[z].y-4,0,0);
					snd_play (2,13); killobj (z); break;
				case obj_max:			// max shields
					points = 1000;
					for (t = 0; t < 4; t++) {
						if (pl.health < 5) {
							pl.health++;
							points -= 200;
							};
						};
					addobj (obj_score, objs[n].x, objs[n].y, 0, -2);
					addobj (obj_explode5, objs[n].x-1, objs[n].y-1,0,0);
					text ("Shields are now at maximum!",0);
					snd_play (2,12); killobj (z);
				};
		}; return (0);
	};

int16_t msg_wing1 (int16_t n, int16_t msg, int16_t z) {
	int16_t sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].count2==0) {
				addobj (obj_jump2, objs[0].x-18, objs[0].y-19, 0, 0);
				objs[n].count2 = 1;
				};
			if (objs[n].count3==0) {
				objs[n].x = (objs[0].x - 5) - objs[n].xl;
				objs[n].y = (objs[0].y - 5) - objs[n].yl;
					if (objs[n].x <= 0) objs[n].x = 0;
					if (objs[n].y <= 48) objs[n].y = 48;
					}
			else	objs[n].y += objs[n].yd;
			wing1_x = objs[n].x; wing1_y = objs[n].y;
			if (objs[n].count2++==300) {
				objs[n].yd = -8;
				objs[n].count3 = 1;
				};
			if (objs[n].y <= -10) killobj(n); break;

		case msg_draw:
			switch (pl.level) {
				case 1:; case 2:; case 3:; case 4:; case 5:;
				case 26:; case 27:; case 28:; case 29:; case 30:
					sh = 0x80a + objs[n].count/4; break;
				case 6:; case 7:; case 8:; case 9:; case 10:;
				case 21:; case 22:; case 23:; case 24:; case 25:
					sh = 0x810 + objs[n].count/4; break;
				case 11:; case 12:; case 13:; case 14:; case 15:
					sh = 0x806 + objs[n].count/2; break;
				case 16:; case 17:; case 18:; case 19:; case 20:
					sh = 0x80e + objs[n].count/4;
					};
			if (objs[n].count2 >= 14)
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
				case obj_mines:
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					killobj (n); snd_play (1,7);
					addobj (obj_explode6, objs[n].x, objs[n].y, 0, 0);
					explosion (objs[n].x, objs[n].y, 5);
					if (--objs[z].count3 < 1) killobj (z);
				};
		}; return (0);
	};

int16_t msg_wing2 (int16_t n, int16_t msg, int16_t z) {
	int16_t sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].count2==0) {
				addobj (obj_jump2, objs[0].x+18, objs[0].y-19, 0, 0);
				objs[n].count2 = 1;
				};
			if (objs[n].count3==0) {
				objs[n].x = (objs[0].x + objs[0].xl) + 5;
				objs[n].y = (objs[0].y - 5) - objs[n].yl;
					if (objs[n].x + objs[n].xl >= scrn_x)
						objs[n].x = scrn_x - objs[n].xl;
					if (objs[n].y <= 48) objs[n].y = 48;
					}
			else	objs[n].y += objs[n].yd;
			wing2_x = objs[n].x; wing2_y = objs[n].y;
			if (objs[n].count2++==300) {
				objs[n].yd = -8;
				objs[n].count3 = 1;
				};
			if (objs[n].y <= -10) killobj(n); break;

		case msg_draw:
			switch (pl.level) {
				case 1:; case 2:; case 3:; case 4:; case 5:;
				case 26:; case 27:; case 28:; case 29:; case 30:
					sh = 0x80a + objs[n].count/4; break;
				case 6:; case 7:; case 8:; case 9:; case 10:;
				case 21:; case 22:; case 23:; case 24:; case 25:
					sh = 0x810 + objs[n].count/4; break;
				case 11:; case 12:; case 13:; case 14:; case 15:
					sh = 0x806 + objs[n].count/2; break;
				case 16:; case 17:; case 18:; case 19:; case 20:
					sh = 0x80e + objs[n].count/4;
					};
			if (objs[n].count2 >= 14)
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
				case obj_mines:
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					killobj (n); snd_play (1,7);
					addobj (obj_explode6, objs[n].x, objs[n].y, 0, 0);
					explosion (objs[n].x, objs[n].y, 5);
					if (--objs[z].count3 < 1) killobj (z);
				};
		}; return (0);
	};

int16_t msg_wing3 (int16_t n, int16_t msg, int16_t z) {
	int16_t sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].count2==0) {
				addobj (obj_jump2, objs[0].x-49, objs[0].y-32, 0, 0);
				objs[n].count2 = 1;
				};
			if (objs[n].count3==0) {
				objs[n].x = (objs[0].x - 36) - objs[n].xl;
				objs[n].y = (objs[0].y - 18) - objs[n].yl;
					if (objs[n].x <= 0) objs[n].x = 0;
					if (objs[n].y <= 48) objs[n].y = 48;
					}
			else	objs[n].y += objs[n].yd;
			wing3_x = objs[n].x; wing3_y = objs[n].y;
			if (objs[n].count2++==300) {
				objs[n].yd = -8;
				objs[n].count3 = 1;
				};
			if (objs[n].y <= -10) killobj(n); break;

		case msg_draw:
			switch (pl.level) {
				case 5:; case 30:
					sh = 0x80a + objs[n].count/4; break;
				case 10:;	case 25:
					sh = 0x810 + objs[n].count/4; break;
				case 15:
					sh = 0x806 + objs[n].count/2; break;
				case 20:
					sh = 0x80e + objs[n].count/4;
					};
			if (objs[n].count2 >= 14)
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
				case obj_mines:
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					killobj (n); snd_play (1,7);
					addobj (obj_explode6, objs[n].x, objs[n].y, 0, 0);
					explosion (objs[n].x, objs[n].y, 5);
					if (--objs[z].count3 < 1) killobj (z);
				};
		}; return (0);
	};

int16_t msg_wing4 (int16_t n, int16_t msg, int16_t z) {
	int16_t sh;
	switch (msg) {
		case msg_update:
			objs[n].count = (objs[n].count + 1)&7;
			if (objs[n].count2==0) {
				addobj (obj_jump2, objs[0].x+49, objs[0].y-32, 0, 0);
				objs[n].count2 = 1;
				};
			if (objs[n].count3==0) {
			objs[n].x = (objs[0].x + objs[0].xl) + 36;
			objs[n].y = (objs[0].y - 18) - objs[n].yl;
				if (objs[n].x + objs[n].xl >= scrn_x)
					objs[n].x = scrn_x - objs[n].xl;
				if (objs[n].y <= 48) objs[n].y = 48;
					}
			else	objs[n].y += objs[n].yd;
			wing4_x = objs[n].x; wing4_y = objs[n].y;
			if (objs[n].count2++==300) {
				objs[n].yd = -8;
				objs[n].count3 = 1;
				};
			if (objs[n].y <= -10) killobj(n); break;

		case msg_draw:
			switch (pl.level) {
				case 5:; case 30:
					sh = 0x80a + objs[n].count/4; break;
				case 10:;	case 25:
					sh = 0x810 + objs[n].count/4; break;
				case 15:
					sh = 0x806 + objs[n].count/2; break;
				case 20:
					sh = 0x80e + objs[n].count/4;
					};
			if (objs[n].count2 >= 14)
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
				case obj_mines:
				case obj_bullet2:
				case obj_spinner:
				case obj_laser:
					killobj (n); snd_play (1,7);
					addobj (obj_explode6, objs[n].x, objs[n].y, 0, 0);
					explosion (objs[n].x, objs[n].y, 5);
					if (--objs[z].count3 < 1) killobj (z);
				};
		}; return (0);
	};