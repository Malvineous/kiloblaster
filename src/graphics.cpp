// GRAPHICS.C: Graphics Driver Controller

#include <stdlib.h>
#include <ctype.h>
#include "include/gr.h"
#include "include/keyboard.h"

#define Red 0
#define Green 1
#define Blue 2

byte x_ourmode;
vptype mainvp;

//	Cmtab:
//		0: Standard shape
//		1: Font
//		2: Erase shape
//		3: Underlay shape

uint16_t cmtab [4][256];
//	CGA: Color (lo) & Mask (hi) table 0 shape 1 font 2 erase shape
//	VGA: Color (lo)/0=Mask
//	EGA: Color (0-15) or 16=Mask Out

int16_t pixelsperbyte;
int16_t origmode;
int16_t pagemode, pageshow, pagedraw;
int16_t showofs, drawofs, pagelen;

void *LOST;

extern void plot_cga (int16_t x, int16_t y, byte color);
extern void plot_ega (int16_t x, int16_t y, byte color);
extern void plot_vga (int16_t x, int16_t y, byte color);
extern void line_cga (int16_t x0, int16_t y0, int16_t x1, int16_t y1, byte color);

p_rec vgapal={
0,0,0,0,0,42,0,36,0,18,41,52,
48,0,0,23,13,33,28,16,4,32,36,40,
12,12,12,10,30,63,0,60,41,32,52,63,
63,0,0,49,39,57,63,62,32,63,63,63,
//0,0,0,0,0,42,0,42,0,0,42,42,
//42,0,0,42,0,42,42,21,0,42,42,42,
//21,21,21,21,21,63,21,63,21,21,63,63,
//63,21,21,63,21,63,63,63,21,63,63,63,
7,7,7,14,14,14,27,27,27,32,32,32,
37,37,37,47,47,47,52,52,52,57,57,57,
25,10,0,37,10,0,50,10,0,63,10,0,
0,21,0,12,21,0,25,21,0,37,21,0,
50,21,0,63,21,0,0,31,0,12,31,0,
25,31,0,37,31,0,50,31,0,63,31,0,
0,42,0,12,42,0,25,42,0,37,42,0,
50,42,0,63,42,0,12,52,0,25,52,0,
37,52,0,50,52,0,63,52,0,12,63,0,
25,63,0,37,63,0,50,63,0,63,63,0,
0,0,12,12,0,12,25,0,12,37,0,12,
50,0,12,63,0,12,0,10,12,12,10,12,
25,10,12,37,10,12,50,10,12,63,10,12,
0,21,12,12,21,12,25,21,12,37,21,12,
50,21,12,63,21,12,0,31,12,12,31,12,
25,31,12,37,31,12,50,31,12,63,31,12,
0,42,12,12,42,12,25,42,12,37,42,12,
50,42,12,63,42,12,12,52,12,25,52,12,
37,52,12,50,52,12,63,52,12,12,63,12,
25,63,12,37,63,12,50,63,12,63,63,12,
0,0,25,12,0,25,25,0,25,37,0,25,
50,0,25,63,0,25,0,10,25,12,10,25,
25,10,25,37,10,25,50,10,25,63,10,25,
0,21,25,12,21,25,25,21,25,37,21,25,
50,21,25,63,21,25,0,31,25,12,31,25,
25,31,25,37,31,25,50,31,25,63,31,25,
0,42,25,12,42,25,25,42,25,37,42,25,
50,42,25,63,42,25,12,52,25,25,52,25,
37,52,25,50,52,25,63,52,25,12,63,25,
25,63,25,37,63,25,50,63,25,63,63,25,
0,0,37,12,0,37,25,0,37,37,0,37,
50,0,37,63,0,37,0,10,37,12,10,37,
25,10,37,37,10,37,50,10,37,63,10,37,
0,21,37,12,21,37,25,21,37,37,21,37,
50,21,37,63,21,37,0,31,37,12,31,37,
25,31,37,37,31,37,50,31,37,63,31,37,
0,42,37,12,42,37,25,42,37,37,42,37,
50,42,37,63,42,37,12,52,37,25,52,37,
37,52,37,50,52,37,63,52,37,12,63,37,
25,63,37,37,63,37,50,63,37,63,63,37,
0,0,50,12,0,50,25,0,50,37,0,50,
50,0,50,63,0,50,0,10,50,12,10,50,
25,10,50,37,10,50,50,10,50,63,10,50,
0,21,50,12,21,50,25,21,50,37,21,50,
50,21,50,63,21,50,0,31,50,12,31,50,
25,31,50,37,31,50,50,31,50,63,31,50,
0,42,50,12,42,50,25,42,50,37,42,50,
50,42,50,63,42,50,12,52,50,25,52,50,
37,52,50,50,52,50,63,52,50,12,63,50,
25,63,50,37,63,50,50,63,50,63,63,50,
0,0,63,12,0,63,25,0,63,37,0,63,
50,0,63,63,0,63,0,10,63,12,10,63,
25,10,63,37,10,63,50,10,63,63,10,63,
0,21,63,12,21,63,25,21,63,37,21,63,
50,21,63,63,21,63,0,31,63,12,31,63,
25,31,63,37,31,63,50,31,63,63,31,63,
0,42,63,12,42,63,25,42,63,37,42,63,
50,42,63,63,42,63,12,52,63,25,52,63,
37,52,63,50,52,63,63,52,63,12,63,63,
25,63,63,37,63,63,50,63,63,63,63,63};

const int16_t DacWrite=0x3C8;
const int16_t DacRead=0x3C7;
const int16_t DacData=0x3C9;
const int16_t input_status_1=0x3DA;
const int16_t vbi_mask=0x8;

char pixvalue;

void pixaddr_cga (int16_t x,int16_t y,char **vidbuf,unsigned char *bitc) {
//   *vidbuf = (void*) (0xb8000000 + (x/4) + 0x2000*(y&1) + 0x50*(y/2));
//   *bitc = (x&3)*2;
};

void pixaddr_ega (int16_t x,int16_t y,char **vidbuf,unsigned char *bitc) {
//   *vidbuf = (void*) (0xa0000000 + y*40 + x/8 + drawofs);
//   *bitc = (x&7);
};

void pixaddr_vga (int16_t x,int16_t y,char **vidbuf,unsigned char *bitc) {
	*vidbuf=(void*) (0xa0000000+drawofs+(80*y)+(x>>2));
	*bitc=(x&3);
	};

//		n<0 = erase
void drawshape (vptype *vp, int16_t n, int16_t x, int16_t y) {
	char far *tblptr;
	char *shapeptr;
	byte xlb, yl;
	int16_t colortbl;
	int16_t ns=(n&0xff);
	int16_t nt=(n>>8);

	if (nt&0x40) {colortbl=3; nt^=0x40;}
	else {colortbl=shm_flags[nt]&shm_fontf;};
	if ((nt<=0)||(nt>=shm_maxtbls)) return;
	if (shm_tbladdr[nt]==NULL) {
		shm_want[nt]=1; shm_do();
		if (shm_tbladdr[nt]==NULL) shm_tbladdr[nt]=LOST;
		};
	if (shm_tbladdr[nt]!=LOST) {
		tblptr=(shm_tbladdr [nt])+(ns*4);
		//shapeptr=shm_tbladdr [nt]+*(int*) tblptr;
		shapeptr=shm_tbladdr [nt]+*(uint16_t*) tblptr;
		xlb=(char) *(tblptr+2); yl=(char) *(tblptr+3);
		x-=vp->vpox; y-=vp->vpoy;
		if ((y<vp->vpyl)&&((y+yl)>=0)&&
			(x<vp->vpxl)&&((x+xlb*pixelsperbyte)>=0)) {
			ldrawsh_vga (vp,x,y,xlb,yl,shapeptr,colortbl);
			};
		};
	};

//	Plot:  Need to adjust color for proper video mode

void plot (vptype *vp, int16_t x, int16_t y, int16_t color) {
	if ((x>=0) && (y>=0) && (x<vp->vpxl) && (y<vp->vpyl)) {
		plot_vga (x+vp->vpx,y+vp->vpy,(char) color);
		};
	};

void waitsafe (void) {
return;
	do {} while (!(inportb(0x3da)&8));
	};

void fontcolor_vga (int16_t hi, int16_t lo, int16_t back) {
	cmtab [1][0]=255;
	cmtab [1][1]=lo;
	cmtab [1][2]=hi;
	if (back==-1) {cmtab[1][3]=255;}
	else {cmtab [1][3]=back;}
	};

void fntcolor (int16_t hi, int16_t lo, int16_t back) {
	fontcolor_vga (lo, hi, back);
	};

void initcolortabs_vga (void) {
	int16_t c;
	for (c=0; c<256; c++) {
		cmtab[0][c]=c;
		cmtab[2][c]=0;
		cmtab[3][c]=c;
		};
	cmtab[0][0]=255;
	cmtab[2][0]=255;
	};

void setpages (void) {
	showofs=pageshow*pagelen;
	drawofs=pagedraw*pagelen;
	};

void setpagemode (int16_t mode) {
	if (mode) {
		pagemode=1;
		pagedraw=1-pageshow;
		setpages();
		lcopypage();
		}
	else {
		pagemode=0;
		pagedraw=pageshow;
		drawofs=showofs;
		};
	};

int16_t getportnum (void) {
	return (*(int16_t*)(0x00400063));
	};

void pageflip (void) {
	SDL_Flip(::screen);
return;
	int16_t portnum;
	pageshow=!pageshow;
	pagedraw=!pagedraw;

	setpages();
	portnum=getportnum();
	do {} while ((inportb(0x3da)&8));	// Wait for end of retrace
	outport (portnum,0x0c+(showofs&0xff00));
	outport (portnum,0x0d+((showofs&0x00ff)<<8));
	do {} while (!(inportb(0x3da)&8));	// Wait for next retrace start
	};

void wait_vbi(void) {
	do {} while ((inportb(0x3da)&8)!=0);
	do {} while ((inportb(0x3da)&8)==0);
	};

void vga_setpal(void) {
	int16_t start=15;
	int16_t number=241;
	uint16_t i;

	if ((start>256)||(start<15)||((start+number)>256)) return;
	waitsafe();
	SDL_Color clr[256];
	for (i=0;i<number-start;i++) {
		clr[i].r = pal6to8(vgapal[(start+i)*3+Red  ]);
		clr[i].g = pal6to8(vgapal[(start+i)*3+Green]);
		clr[i].b = pal6to8(vgapal[(start+i)*3+Blue ]);
		};

	SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, clr, start, number);

	setcolor (1,0,0,42); setcolor (2,0,36,0); setcolor (3,18,41,52);
	setcolor (4,48,0,0); setcolor (5,23,13,33); setcolor (6,28,16,4);
	setcolor (7,32,36,40); setcolor (8,12,12,12); setcolor (9,10,30,63);
	setcolor (10,0,60,41); setcolor (11,32,52,63); setcolor (12,63,0,0);
	setcolor (13,49,39,57); setcolor (14,63,62,32);
	};

void readpal(p_rec palette) {
	int16_t start=0;
	int16_t number=256;
	uint16_t i;

	if ((start>256)|(start<0)|((start+number)>256)) return;
	for (i=start;i<(start+number);i++) {
		outportb(DacRead,i);
		palette[i*3+Red]=inportb(DacData);
		palette[i*3+Green]=inportb(DacData);
		palette[i*3+Blue]=inportb(DacData);
		};
	};

void clrpal(void) {
	SDL_Color clr[256];
	memset(clr, 0, sizeof(clr));
	SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, clr, 0, 256);
	};

void fadein(void) {
	//p_rec currentpal;
	SDL_Color currentpal[256];
	int16_t temp, i, cycle;

	for (cycle=0;cycle<64;cycle+=2) {
		for (i=0;i<256;i++){
			currentpal[i].r = (pal6to8(vgapal[i*3+Red  ]) * cycle) >> 6;
			currentpal[i].g = (pal6to8(vgapal[i*3+Green]) * cycle) >> 6;
			currentpal[i].b = (pal6to8(vgapal[i*3+Blue ]) * cycle) >> 6;
			};
		waitsafe();
		SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, currentpal, 0, 256);
		SDL_Flip(::screen);
		usleep(750/64*1000);
		};
	};

void setcolor (int16_t c, int16_t n1, int16_t n2, int16_t n3) {
	SDL_Color n;
	n.r = pal6to8(n1);
	n.g = pal6to8(n2);
	n.b = pal6to8(n3);
	SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, &n, c, 1);
	SDL_Flip(::screen);
	};

void fadeout(void) {
	SDL_Color currentpal[256];
	int16_t i, temp, cycle;

	for (cycle=63;cycle>=0;cycle-=2) {
		for (i=0;i<256;i++){
			currentpal[i].r = (pal6to8(vgapal[i*3+Red  ]) * cycle) >> 6;
			currentpal[i].g = (pal6to8(vgapal[i*3+Green]) * cycle) >> 6;
			currentpal[i].b = (pal6to8(vgapal[i*3+Blue ]) * cycle) >> 6;
			};
		waitsafe();
		SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, currentpal, 0, 256);
		SDL_Flip(::screen);
		usleep(750/64*1000);
		};
	};

void gr_init (void) {
	int16_t ch;
	struct REGPACK preg;
//	Save old mode:
	preg.r_ax=0x0f00;
	intr (0x10,&preg);
	origmode=preg.r_ax&0xff;
	pagemode=0;
	pageshow=0;
	pagedraw=0;
	showofs=0;
	drawofs=0;
	mainvp.vpx=0; mainvp.vpy=0; mainvp.vpxl=320; mainvp.vpyl=200;
	mainvp.vpox=0; mainvp.vpoy=0;
	pagelen=16384;
	x_ourmode=x_vga;
	for (ch=0; ch<16; ch++) {
		preg.r_ax=0x1000;
		preg.r_bx=ch*0x100+ch;
		intr (0x10,&preg);
		};
	preg.r_ax=0x1200;
	preg.r_bx=0x0031;
	intr (0x10,&preg);
	preg.r_ax=0x0013;
	intr (0x10,&preg);

	initcolortabs_vga();
	fontcolor_vga (0x2a,0x22,0);
	pixelsperbyte=1;

	clrpal();
	outport (0x3c4,0x0604);					// Seq: Disable chain4 & oddeven
	outport (0x3ce,0x4005);					// Gr: turn off oddeven
	outport (0x3d4,0x0014);					// Turn off dword mode
	outport (0x3d4,0xe317);					// Turn on byte mode
	clrvp (&mainvp,0);
	vga_setpal();

	LOST=malloc (1);
	};

void gr_exit (void) {
	struct REGPACK preg;
	preg.r_ax=0x0000+origmode;				//	call BIOS to switch back
	intr (0x10,&preg);
	};

//#if 0
void dim (void) {
	SDL_Color currentpal[256];
	int16_t temp,i,cycle;

	for (cycle=63;cycle>=31;cycle-=6) {
		for (i=0;i<256;i++){
			currentpal[i].r = (pal6to8(vgapal[i*3+Red  ]) * cycle) >> 6;
			currentpal[i].g = (pal6to8(vgapal[i*3+Green]) * cycle) >> 6;
			currentpal[i].b = (pal6to8(vgapal[i*3+Blue ]) * cycle) >> 6;
			};
		waitsafe();
		SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, currentpal, 0, 256);
		SDL_Flip(::screen);
		usleep(750/64*1000);
		};
	};

void undim (void) {
	SDL_Color currentpal[256];
	int16_t temp,i,cycle;

	for (cycle=32;cycle<64;cycle+=6) {
		for (i=0;i<256;i++){
			currentpal[i].r = (pal6to8(vgapal[i*3+Red  ]) * cycle) >> 6;
			currentpal[i].g = (pal6to8(vgapal[i*3+Green]) * cycle) >> 6;
			currentpal[i].b = (pal6to8(vgapal[i*3+Blue ]) * cycle) >> 6;
			};
		waitsafe();
		SDL_SetPalette(::screen, SDL_LOGPAL | SDL_PHYSPAL, currentpal, 0, 256);
		SDL_Flip(::screen);
		usleep(750/64*1000);
		};
	};
//#endif
