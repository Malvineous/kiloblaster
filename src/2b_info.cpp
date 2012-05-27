//	2B_INFO.C
//
// Kiloblaster v.2 Info
//
// Written by Allen W. Pilgrim

#include <stdlib.h>
#include "include/gr.h"
#include "include/keyboard.h"
#include "include/windows.h"
#include "include/gamectrl.h"
#include "include/2blaster.h"

void init_info(void) {
	int16_t c;
	for (c=0; c < b_maxbkgnd; c++) {
		info [c].sh    = 0x4400;		// blank shape
		info [c].na    = "";		// untitled
		info [c].flags = 0;			// no flags
		};

	info [b_blank         ].sh    = 0x4400+0;
	info [b_blank         ].na    = "0";
	info [b_blank         ].flags = 0;
	info [b_wall1         ].sh    = 0x4400+1;
	info [b_wall1         ].na    = "1";
	info [b_wall1         ].flags = 0;
	info [b_wall2         ].sh    = 0x4400+2;
	info [b_wall2         ].na    = "2";
	info [b_wall2         ].flags = 0;
	info [b_wall3         ].sh    = 0x4400+3;
	info [b_wall3         ].na    = "3";
	info [b_wall3         ].flags = 0;
	info [b_wall4         ].sh    = 0x4400+4;
	info [b_wall4         ].na    = "4";
	info [b_wall4         ].flags = 0;
	info [b_wall5         ].sh    = 0x4400+5;
	info [b_wall5         ].na    = "5";
	info [b_wall5         ].flags = 0;
	info [b_wall6         ].sh    = 0x4400+6;
	info [b_wall6         ].na    = "6";
	info [b_wall6         ].flags = 0;
	info [b_wall7         ].sh    = 0x4400+7;
	info [b_wall7         ].na    = "7";
	info [b_wall7         ].flags = 0;
	info [b_wall8         ].sh    = 0x4400+8;
	info [b_wall8         ].na    = "8";
	info [b_wall8         ].flags = 0;
	info [b_wall9         ].sh    = 0x4400+9;		// stonewall
	info [b_wall9         ].na    = "9";
	info [b_wall9         ].flags = 0;
	info [b_walla         ].sh    = 0x4400+10;
	info [b_walla         ].na    = "A";
	info [b_walla         ].flags = 0;
	info [b_wallb         ].sh    = 0x4400+11;
	info [b_wallb         ].na    = "B";
	info [b_wallb         ].flags = 0;
	info [b_wallc         ].sh    = 0x4400+12;
	info [b_wallc         ].na    = "C";
	info [b_wallc         ].flags = 0;
	info [b_walld         ].sh    = 0x4400+13;
	info [b_walld         ].na    = "D";
	info [b_walld         ].flags = 0;
	info [b_walle         ].sh    = 0x4400+14;
	info [b_walle         ].na    = "E";
	info [b_walle         ].flags = 0;
	info [b_wallf         ].sh    = 0x4400+15;
	info [b_wallf         ].na    = "F";
	info [b_wallf         ].flags = 0;
	info [b_wallg         ].sh    = 0x4400+22;
	info [b_wallg         ].na    = "G";
	info [b_wallg         ].flags = 0;
	info [b_wallh         ].sh    = 0x4400+23;
	info [b_wallh         ].na    = "H";
	info [b_wallh         ].flags = 0;
	info [b_walli         ].sh    = 0x4400+24;
	info [b_walli         ].na    = "I";
	info [b_walli         ].flags = 0;
	info [b_wallj         ].sh    = 0x4400+25;
	info [b_wallj         ].na    = "J";
	info [b_wallj         ].flags = 0;
	info [b_wallk         ].sh    = 0x4400+26;
	info [b_wallk         ].na    = "K";
	info [b_wallk         ].flags = 0;
	info [b_walll         ].sh    = 0x4400+27;
	info [b_walll         ].na    = "L";
	info [b_walll         ].flags = 0;
	info [b_wallm         ].sh    = 0x4400+28;
	info [b_wallm         ].na    = "M";
	info [b_wallm         ].flags = 0;
	info [b_walln         ].sh    = 0x4400+29;
	info [b_walln         ].na    = "N";
	info [b_walln         ].flags = 0;
	info [b_wallo         ].sh    = 0x4400+30;
	info [b_wallo         ].na    = "O";
	info [b_wallo         ].flags = 0;
	info [b_wallp         ].sh    = 0x4400+31;
	info [b_wallp         ].na    = "P";
	info [b_wallp         ].flags = 0;
	info [b_wallq         ].sh    = 0x4400+32;
	info [b_wallq         ].na    = "Q";
	info [b_wallq         ].flags = 0;
	info [b_wallr         ].sh    = 0x4400+33;
	info [b_wallr         ].na    = "R";
	info [b_wallr         ].flags = 0;
	info [b_walls         ].sh    = 0x4400+34;
	info [b_walls         ].na    = "S";
	info [b_walls         ].flags = 0;
	info [b_wallt         ].sh    = 0x4400+35;
	info [b_wallt         ].na    = "T";
	info [b_wallt         ].flags = 0;
	info [b_wallu         ].sh    = 0x4400+36;
	info [b_wallu         ].na    = "U";
	info [b_wallu         ].flags = 0;
	};