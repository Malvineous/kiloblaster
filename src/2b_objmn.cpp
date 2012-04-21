//	2B_OBJMN.C
//
// Kiloblaster v.2 Object Management
//
// Written by Allen W. Pilgrim

#include <stdlib.h>
#include <mem.h>
#include "\develop\kilo2\include\gr.h"
#include "\develop\kilo2\include\keyboard.h"
#include "\develop\kilo2\include\windows.h"
#include "\develop\kilo2\include\gamectrl.h"
#include "\develop\kilo2\include\music.h"
#include "\develop\kilo2\include\2blaster.h"

extern int level_flg;

void addobj (int kind, int x, int y, int xd, int yd) {
	if ((num_objs + 1) >= (max_objs)) return;
		objs [num_objs].kind   = kind;
		objs [num_objs].x      = x;
		objs [num_objs].y      = y;
		objs [num_objs].xl     = objinfo[kind].xl;
		objs [num_objs].yl     = objinfo[kind].yl;
		objs [num_objs].xd     = xd;
		objs [num_objs].yd     = yd;
		objs [num_objs].state  = st_begin;
		objs [num_objs].count  = 0;
		objs [num_objs].count2 = 0;
		objs [num_objs].count3 = 0;
		objs [num_objs].flags  = objinfo[kind].flags;
		objs [num_objs].class  = objinfo[kind].class;
		if (objs[num_objs].class == class_enemy) enemy_cnt++;
		if (objs[num_objs].class == class_weapon1) fire_cnt++;
		if (objs[num_objs].class == class_weapon2) weapon_cnt++;
		num_objs++;
	};

void add_inv (int inv_type) {
	if (pl.num_inv >= 41) return;
	statmodflg |= mod_screen;
	pl.inv[pl.num_inv] = inv_type;
	pl.num_inv++;
	};

int take_inv (int inv_type) {
	int c, d;
	for ( c = 0; c < pl.num_inv; c++) {
		if (pl.inv[c] == inv_type) {
			for (d = c + 1; d < pl.num_inv; d++) {
				pl.inv[d - 1] = pl.inv[d];
				};
			pl.num_inv--;
			statmodflg |= mod_screen;
			return (1);
			};
		};
	return (0);
	};

int inv_cnt (int inv_type) {
	int c, d;
	d = 0;
	for (c = 0; c < pl.num_inv; c++) {
		d += (pl.inv[c]==inv_type);
		};
	return (d);
	};

void init_inv (void) {
	pl.num_inv = 0;
	pl.score = 0;
	pl.level = 1;
	pl.health = 5;
	while (take_inv (inv_player)) continue;
	add_inv (inv_player); add_inv (inv_player);
	add_inv (inv_bomb);	add_inv (inv_bomb);	add_inv (inv_bomb);
	add_inv (inv_bomb);	add_inv (inv_bomb);
	};

void init_objs (void) {
	num_objs = 0;
	addobj (obj_player, 0, 0, 0, 0);
	fire_cnt = 0;
	weapon_cnt = 0;
	star_flag = 0;
	enemy_cnt = 0;	enemy_flag = 1;
	bonus_flg1 = 0; bonus_flg2 = 0; bonus_flg3 = 0;
	level_flg = 4;
	x_pnt = scrn_x/3; y_pnt = scrn_y/3;
	x_pnt2 = rand()%224; y_pnt2 = rand()%156;
	x_pnt3 = rand()%224; y_pnt3 = 36;
	};

void p_new (void) {
	objs[0].count2 = 1;
	addobj (obj_jump1, scrn_x/2-9, scrn_y-20, 0, 0);
	objs[0].state = st_begin;
	};

void cosmos (void) {
	addobj (obj_stars, 7, 45, 0, 0); addobj (obj_stars, 23, 134, 0, 0);
	addobj (obj_stars, 32, 38, 0, 0); addobj (obj_stars, 45, 86, 0, 0);
	addobj (obj_stars, 85, 20, 0, 0); addobj (obj_stars, 145, 150, 0, 0);
	addobj (obj_stars, 175, 45, 0, 0);	addobj (obj_stars, 225, 120, 0, 0);
	};

void p_hit (int take) {
	statmodflg |= mod_screen;
	pl.health -= take;
	if (pl.health > 0) {
		snd_play (1,1);
		}
	else {
		pl.health = 0;
		objs[0].state = st_die;
		snd_play (3,2);
		};
	};

void killobj (int n) {
	objs[n].kind = obj_killme;
	};

void refresh (int pagemode) {
	int n, x, y;
	if (pagemode) {
		if (statmodflg) {
			stat_win ();
			statmodflg &= (pagedraw + 1) * mod_page0;
			};

	for (x = 0; x < end_x; x++) {
		for (y = 0; y < end_y; y++) {
			if (modflg[x][y] & mod_screen) {
				drawcell (x,y);
				modflg[x][y] &= ((pagedraw+1)*mod_page0)^0xffff;
				};
			};
		};

	for (n = num_objs - 1; n >= 0; n--) {
		if (mod_screen) {
			(*objinfo[objs[n].kind].msg) (n, msg_draw, 0);
			};
		}; pageflip ();
	}
	else {
		if (statmodflg) {
			stat_win ();
			statmodflg = 0;
			};

	for (x = 0; x < end_x; x++) {
		for (y = 0; y < end_y; y++) {
			drawcell (x, y);
			modflg[x][y] &= (mod_screen^0xffff);
			};
		};
	};
};

void upd_objs (void) {
	int cnt1, cnt2, x, y, startx, starty, endx, endy;
	for (cnt1 = 0; cnt1 < num_objs; cnt1++) {
		startx = objs[cnt1].x>>4;
		starty = objs[cnt1].y>>4;
		endx = (objs[cnt1].x + objs[cnt1].xl + 15)>>4;
		endy = (objs[cnt1].y + objs[cnt1].yl + 15)>>4;
			if (startx < 0) startx = 0;
			if (starty < 0) starty = 0;
			if (endx > end_x) endx = end_x;
			if (endy > end_y) endy = end_y;
		for (x = startx; x < endx; x++) {
			for (y = starty; y < endy; y++) {
				modflg[x][y] |= mod_screen;
				};
			};
		(*objinfo[objs[cnt1].kind].msg) (cnt1, msg_update, 0);

	if (objs[cnt1].flags==0) {
		for (cnt2 = 0; cnt2 < num_objs; cnt2++) {
			if (cnt1 != cnt2) {
				if (
					(objs[cnt2].x < (objs[cnt1].x + objs[cnt1].xl)) &&
					(objs[cnt1].x < (objs[cnt2].x + objs[cnt2].xl)) &&
					(objs[cnt2].y < (objs[cnt1].y + objs[cnt1].yl)) &&
					(objs[cnt1].y < (objs[cnt2].y + objs[cnt2].yl)) ) {
						(*objinfo[objs[cnt1].kind].msg)
						(cnt1, msg_touch, cnt2);
						};
					};
				};
			};
		};
	};

void purgeobjs (void) {
	int c, d;		// Purge objects of type obj_killme
	d = 0;
	for (c = 0; c < num_objs; c++) {
		if (objs[c].kind != obj_killme) {
			if (c != d) {
				memcpy (&objs[d], &objs[c], sizeof(objs[c]));
				}; d++;
			};
		}; num_objs = d;
	};

void move_obj (int n, int new_x, int new_y) {
	if (new_y < 0) new_y = 0;
	else if (new_y > (scrn_y - objs[n].yl))
		new_y = scrn_y - objs[n].yl;
	if (new_x < 0) new_x = 0;
	else if (new_x > (scrn_x - objs[n].xl))
		new_x = scrn_x - objs[n].xl;
	objs[n].x = new_x;
	objs[n].y = new_y;
	};

void seekplayer (int n, int *dx, int *dy) {
	*dx = (objs[0].x > objs[n].x) - (objs[0].x < objs[n].x);
	*dy = (objs[0].y > objs[n].y) - (objs[0].y < objs[n].y);
	};

void attract (int n, int *ax, int *ay) {		// attraction point
	*ax = (x_pnt > objs[n].x) - (x_pnt < objs[n].x);
	*ay = (y_pnt > objs[n].y) - (y_pnt < objs[n].y);
	};

void attract2 (int n, int *ax, int *ay) {		// attraction point
	*ax = (x_pnt2 > objs[n].x) - (x_pnt2 < objs[n].x);
	*ay = (y_pnt2 > objs[n].y) - (y_pnt2 < objs[n].y);
	};

void attract3 (int n, int *ax, int *ay) {		// attraction point
	*ax = (x_pnt3 > objs[n].x) - (x_pnt3 < objs[n].x);
	*ay = (y_pnt3 > objs[n].y) - (y_pnt3 < objs[n].y);
	};

int class_cnt (int c) {
	int n;
	int count = 0;
	for (n = 0; n < num_objs; n++) {
		if (objinfo[objs[n].kind].class==c) count++;
		};
	return (count);
	};

void upd_end (void) {
	if (random(5)==0) addobj (obj_jump2, rand()%236, rand()%172, 0, 0);
	if (random(10)==0) addobj (obj_explode6, rand()%240, rand()%176, 0, 0);
	if (random(15)==0) explosion (rand()%240, rand()%176, 5);
	};