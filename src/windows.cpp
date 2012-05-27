//	Windows.C: Window controller

#include <string.h>
#include "include/gr.h"
#include "include/keyboard.h"
#include "include/windows.h"

char cursorchar;
int16_t  curhi, curlo, curback;					// current color scheme

void defwin (wintype *win,int16_t x8,int16_t y,int16_t xl16,int16_t yl16,int16_t h16,int16_t v16,int16_t flags) {
	win->winflags=flags;

	initvp (&(win->border),248);
	win->border.vpx=x8*8;
	win->border.vpy=y;
	win->border.vpxl=xl16*16+16;
	win->border.vpyl=yl16*16+((flags&textbox)?16:28);

	initvp (&(win->inside),248);
	win->inside.vpx=x8*8+8;
	win->inside.vpy=y+((flags&textbox)?8:16);
	win->inside.vpxl=xl16*16;
	win->inside.vpyl=yl16*16;

	if (h16!=0) {
		win->inside.vpxl-=(h16*16+8);
		win->inside.vpx+=h16*16+8;
		};

	initvp (&(win->topleft),8);
	win->topleft.vpx=x8*8+8;
	win->topleft.vpy=y+16;
	win->topleft.vpxl=h16*16;
	win->topleft.vpyl=v16*16+5;

	win->winx8=x8;
	win->winx=x8*8;
	win->winy=y;
	win->winxl16=xl16;
	win->winxl=xl16*16;
	win->winyl16=yl16;
	win->winyl=yl16*16;

	win->winh16=h16;
	win->winh=h16*16;
	win->winv16=v16;
	win->winv=v16*16;
	};

void drawwin (wintype *win) {
	int16_t c,d;
		// 1. Clear border vp;
		// 2. Draw border characters,  etc.

	clearvp (&win->border);
	if (win->winflags&textbox) {
		drawshape (&win->border,0x440b,0,0);
		drawshape (&win->border,0x440d,win->winxl+8,0);
		drawshape (&win->border,0x4410,0,win->winyl+8);
		drawshape (&win->border,0x4412,win->winxl+8,win->winyl+8);
		for (c=1; c<(win->winxl16*2+1); c++) {
			drawshape (&win->border,0x440c,c*8,0);
			drawshape (&win->border,0x4411,c*8,win->winyl+8);
			};
		for (c=1; c<(win->winyl16*2+1); c++) {
			drawshape (&win->border,0x440e,0,c*8);
			drawshape (&win->border,0x440f,win->winxl+8,c*8);
			};
		for (c=0; c<(win->winxl16*2); c++) {
			for (d=0; d<(win->winyl16*2); d++) {
				drawshape (&win->inside,0x4413,c*8,d*8);
				};
			};
		}				// end of:  if (win->winflags&textbox)
	else if (win->winflags&dialog) {				// for dialog box and journal
		drawshape (&win->border,0x4702,0,0);
		drawshape (&win->border,0x4707,win->winxl+8,win->winyl+16);
		drawshape (&win->border,0x4705,0,win->winyl+16);
		for (c=0; c<(win->winxl16); c++) {
			if ((c==0)|(c!=win->winh16)) {
				drawshape (&win->border,0x4704,c*16+8,0);
				drawshape (&win->border,0x4706,c*16+8,win->winyl+16);
				};
			};
		drawshape (&win->border,0x4703,win->winxl+8,0);
		for (c=0; c<(win->winyl16); c++) {
			drawshape (&win->border,0x4701,win->winxl+8,16*c+16);
			if ((c==0)||(c!=win->winv16)) {
				drawshape (&win->border,0x4700,0,c*16+16);
				};
			};

		if (win->winh16>0) {
			drawshape (&win->border,0x470a,win->winh+8,0);
			for (c=0; c<win->winyl16; c++) {
				if ((c==0)||(c!=win->winv16)) {
					drawshape (&win->border,0x4709,win->winh+8,c*16+16);
					};
				};
			drawshape (&win->border,0x4708,win->winh+8,win->winyl+16);
			};
		}
	else {
		drawshape (&win->border,0x4402,0,0);
		drawshape (&win->border,0x4407,win->winxl+8,win->winyl+16);
		drawshape (&win->border,0x4405,0,win->winyl+16);
		for (c=0; c<(win->winxl16); c++) {
			if ((c==0)|(c!=win->winh16)) {
				drawshape (&win->border,0x4404,c*16+8,0);
				drawshape (&win->border,0x4406,c*16+8,win->winyl+16);
				};
			};
		drawshape (&win->border,0x4403,win->winxl+8,0);
		for (c=0; c<(win->winyl16); c++) {
			drawshape (&win->border,0x4401,win->winxl+8,16*c+16);
			if ((c==0)||(c!=win->winv16)) {
				drawshape (&win->border,0x4400,0,c*16+16);
				};
			};

		if (win->winh16>0) {
			drawshape (&win->border,0x440a,win->winh+8,0);
			for (c=0; c<win->winyl16; c++) {
				if ((c==0)||(c!=win->winv16)) {
					drawshape (&win->border,0x4409,win->winh+8,c*16+16);
					};
				};
			drawshape (&win->border,0x4408,win->winh+8,win->winyl+16);
			};
		};
	};

void undrawwin (wintype *win) {
	clearvp (&(win->border));
	};

void wprint (vptype *vp, int16_t x, int16_t y, int16_t font, char *text) {
	int16_t fontx;
	int16_t c;

	if ((curhi!=vp->vphi)||(curback!=vp->vpback)) {
		fontcolor (vp, vp->vphi, vp->vpback);
		};

	switch (font) {
		case 1: fontx=8; break;			// 8x8
		case 2: fontx=6; break;			// 6x6
		default: fontx=0;
		};
	if (fontx!=0) {
		for (c=0; c<strlen(text); c++) {
			drawshape (vp,(font<<8)+(text[c]&0x7f),x+fontx*c,y);
			// (font<<8) left shift twice = font*8*8
			};
		};
	};

int16_t wgetkey (vptype *vp, int16_t x, int16_t y, int16_t font) {
	char tempstr[2];
	int16_t oldclock;
	tempstr [1]=0;

	while (!k_pressed()) {
		oldclock=getclock();
		do {} while (oldclock==getclock());
		cursorchar=(cursorchar&7)+1;
		tempstr[0]=cursorchar;
		wprint (vp,x,y,font,tempstr);
		};
	wprint (vp,x,y,font," ");
	return (k_read());
	};

void winput (vptype *vp, int16_t x, int16_t y, int16_t font, char *text, int16_t maxlen) {
	int16_t key;
	int16_t fontx;
	int16_t templen;
	char tempstr[2];
	int16_t firstflag=1;

	switch (font) {
		case 1: fontx=8; break;			// 8x8
		case 2: fontx=6; break;			// 6x6
		default: fontx=0;
		};
	wprint (vp,x,y,font,text);
	tempstr[1]=0;
	do {
		key=wgetkey (vp,x+fontx*strlen(text),y,font);
		if ((key>=32)&&(key<128)) {
			if (firstflag) {
				firstflag=0;
				while (text[firstflag]!='\0') text [firstflag++]=' ';
				text [firstflag++]=' ';
				text [firstflag]='\0';
				wprint (vp,x,y,font,text);
				text[0]='\0';
				};
			if (strlen (text)<maxlen) {
				templen=strlen(text);
				text [templen]=key;
				text [templen+1]=0;
				tempstr[0]=key;
				wprint (vp,x+fontx*templen,y,font,tempstr);
				};
			}
		else if ((key==k_bs)||(key==k_left)) {
			if (strlen(text)>0) text [strlen(text)-1]=0;
			};
		firstflag=0;
		} while ((key!=enter)&&(key!=escape));
	};

void titlewin (wintype *win, char *text, int16_t flg) {
	if (flg==0) wprint (&win->border,16+(win->winxl+win->winh)/2-
		4*strlen(text),4,1,text);
	else wprint (&win->border,16+(win->winxl+win->winh)/2-
		4*strlen(text)-32,4,1,text);
	};

void initvp (vptype *vp, int16_t bkgnd) {
	vp->vpox=0;
	vp->vpoy=0;
	vp->vphi=1;
	vp->vpback=bkgnd;
	};

void clearvp (vptype *vp) {
	clrvp (vp, vp->vpback);
	};

void fontcolor (vptype *vp, int16_t hi, int16_t back) {
//	const int16_t hitab[8]={39,9,119,144,160,134,161,15};
//	const int16_t hitab[8]={39,9,10,11,12,134,71,15};			// original

//	if (hi<8) hi=hitab[hi];
	if (back==0) back=248;
	if (hi==0) hi=11;
	if (hi==8) {														// yellow cursor
		hi=6;	fntcolor (hi,14,back);
		}
	else fntcolor (hi,hi,back);
	vp->vphi   = hi;
   vp->vpback = back;
   curhi      = hi;
   curback    = back;
	};
