//	2B_EDITR.C
//
// Kiloblaster v.2 Editor
//
// Written by Allen W. Pilgrim

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include "\develop\kilo2\include\gr.h"
#include "\develop\kilo2\include\keyboard.h"
#include "\develop\kilo2\include\windows.h"
#include "\develop\kilo2\include\gamectrl.h"
#include "\develop\kilo2\include\config.h"
#include "\develop\kilo2\include\2blaster.h"

int x, y, cell;
extern char cfgfname[];
extern char ext[];

void fill_brd (void);

void infname (char *msg, char *fname, int num) {
	fontcolor (&statvp, 10, 0);
	clearvp (&statvp);
	wprint (&statvp, 2, 2, 1, msg);
	fontcolor (&statvp, 13, 0);
	if (num) wprint (&statvp, 4, 26, 2, "NO EXT.");
	winput (&statvp, 0, 14, 2, fname, 8);
	};

void edit_brd (void) {
	int tempint;
	int lastcell = b_blank;	// last placed item on the board
	char tempstr[12];		// temporary string, used for input
	char tempfname[12];
	tempstr[0] = '\0';
	tempfname[0] = '\0';
	init_brd (); clrvp (&cmdvp, 0); clrvp (&statvp, 0);
	x = 0, y = 0, cell = 0;
	if (pcx_sh==0) loadboard ("menu");
	drawboard ();

	fontcolor (&cmdvp, 14, 0);
	wprint (&cmdvp, 2, 4, 1, "L");
	wprint (&cmdvp, 2, 16, 1, "S");
	wprint (&cmdvp, 2, 28, 1, "A");
	wprint (&cmdvp, 2, 40, 1, "C");
	wprint (&cmdvp, 2, 52, 1, "F");
	wprint (&cmdvp, 4, 76, 2,  "<space>");
	wprint (&cmdvp, 4, 98, 2,  "<enter>");
	fontcolor (&cmdvp, 13, 0);
	wprint (&cmdvp, 10, 4, 1, "OAD");
	wprint (&cmdvp, 10, 16, 1, "AVE");
	wprint (&cmdvp, 10, 28, 1, "DD");
	wprint (&cmdvp, 10, 40, 1, "LEAR");
	wprint (&cmdvp, 10, 52, 1, "ILL");
	fontcolor (&cmdvp, 11, 0);
	wprint (&cmdvp, 0, 64, 1,  "REPEAT");
	wprint (&cmdvp, 8, 86, 1, "COPY");

  do	{
	drawshape (&gamevp, 0x4400 + 16, x * 16, y * 16);	// cursor
	checkctrl0 (0);
	drawcell (x, y);		// draw board cell (erases cursor!)

	x += dx1;	y += dy1;
	if (x < 0) x = 0;		// bounds checking routine
	if (x >= end_x) x = end_x - 1;
	if (y < 0) y = 0;
	if (y >= end_y) y = end_y - 1;

	switch (toupper(key)) {
		case 'A' :			// A = add new background
			clearvp (&statvp);
			wprint (&statvp, 2, 2, 1, "NAME:");
			winput (&statvp, 2, 14, 1, tempstr, 5);
			strupr (tempstr);

			for (tempint = 0; tempint < b_maxbkgnd; tempint++) {
				if (strcmp (tempstr, info[tempint].na) == 0) {
					board[x][y] = tempint;
					lastcell = tempint;
					};
				}; break;

		case ' ' :	// SPACE = put down last background again
			board[x][y] = lastcell;
			break;

		case 13 :		// RETURN =  'pick up' whatever is under cursor
			lastcell = board[x][y];
			break;

		case 'F' :	// Fills entire screen with selected pattern
			fill_brd ();
			for (x = 0; x < end_x; x++) {
				for (y = 0; y < end_y; y++) {
				board[x][y] = cell;
				};
			};
			drawboard (); break;

		case 'C' :	// clears screen and sets everything to "0"
			init_brd ();
			x = 0, y = 0, cell = 0; break;

		case 'Z':
			infname ("LOAD:", tempfname, 0);
			if (tempfname[0] != '\0') {
				loadbrd (tempfname);
				drawboard ();
				}; break;

		case 'L':
			infname ("LOAD:", tempfname, 1);
			if (tempfname[0] != '\0') {
				loadboard (tempfname);
				drawboard ();
				}; break;

		case 'S':
			infname ("SAVE:", tempfname, 1);
			if (tempfname[0] != '\0') saveboard (tempfname); break;
		};
		key = (toupper(key));
	} while (key != escape);
}

/*
void init_brd (void) {
	int x, y;
	clrvp (&gamevp, 0);
	for (x = 0; x < end_x; x++) {
		for (y = 0; y < end_y; y++) {
			board[x][y] = 0;
			modflg[x][y] = 0;
			};
		};
	};
*/

/*
void drawboard (void) {
	int x, y;
	statmodflg = 0;
	refresh (0);
	};

void drawcell (int x, int y) {
	if (board[x][y]==0) {
		drawshape (&gamevp, pcx_sh + x + y * 16, x * 16, y * 16);
		}
	else {
		drawshape (&gamevp, info[board[x][y]].sh, x * 16, y * 16);
		};
	}
*/

void fill_brd () {
	cell = getch();
	switch (cell) {
		case '0': cell = 0; break;	// blank
		case '1':	cell = 1; break;	// wall1
		case '2': cell = 2; break;	// wall2
		case '3': cell = 3; break;	// wall3
		case '4': cell = 4; break;	// wall4
		case '5': cell = 5; break;	// wall5
		case '6': cell = 6; break;	// wall6
		case '7': cell = 7; break;	// wall7
		case '8': cell = 8; break;	// wall8
		case '9': cell = 9; break;	// wall9
		case 'a': cell = 10; break;	// walla
		case 'b': cell = 11; break;	// wallb
		case 'c': cell = 12; break;	// wallc
		case 'd': cell = 13; break;	// walld
		case 'e': cell = 14; break;	// walle
		case 'f': cell = 15; break;	// wallf
		default : cell = 0; break;	// default
		};
	};

void loadbrd (char *fname) {			// loads a board with .BAK extension
	int boardfile;
	char dest[12];
	char *src1 = ".bak";
	strcpy(dest, fname);
	strcat(dest, src1);
	boardfile = _open (dest, O_BINARY);
	if (!read (boardfile, &board, sizeof(board))); // rexit(1);
	_close (boardfile);
	};

/*
void loadboard (char *fname) {		// loads a board with .BRD extension
	int boardfile;
	char dest[12];
	char *src1 = ext;
	strcpy(dest, fname);
	strcat(dest, src1);
	boardfile = _open (dest, O_BINARY);
	if (!read (boardfile, &board, sizeof(board))); // rexit(1);
	_close (boardfile);
	};

void saveboard (char *fname) {		// saves a board with .BRD extension
	int boardfile;
	char dest[12];	char dest2[12];
	char *src1 = ext; char *src2 = ".bak";
	strcpy(dest, fname); strcat(dest, src1);
	strcpy(dest2, fname); strcat(dest2, src2);
	boardfile = creatnew (dest, 0);
	if (boardfile== -1) {
		rename (dest, dest2);
		boardfile = _creat (dest, 0);
		if (!write (boardfile, &board, sizeof(board))) rexit(5);
		}
	else {
		if (!write (boardfile, &board, sizeof(board))) rexit(5);
		};
		_close (boardfile);
	};

void loadcfg (void) {
	int cfgfile, c;
	char ourname[64];
//	strcpy (ourname, path);
	strcpy (ourname, cfgfname);
	cfgfile = _open (ourname, O_BINARY);
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
	int cfgfile;
	char ourname[64];
//	strcpy (ourname, path);
	strcpy (ourname, cfgfname);
	cfgfile = _creat (ourname, 0);
	if (cfgfile >= 0) {
		write (cfgfile, &hiname, sizeof(hiname));
		write (cfgfile, &hiscore, sizeof(hiscore));
		write (cfgfile, &savename, sizeof(savename));
		write (cfgfile, &cf, sizeof (cf));
		};
	close (cfgfile);
	};
*/