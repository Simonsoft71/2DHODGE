#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <mem.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <int1_3.h>

int Calcattr (int sfondo, int chcol)
{
  return (sfondo << 4)+chcol << 8;
}

void Dirscreen( int x1, int y1, int x2, int y2, int attrib)
{
 unsigned int far *screen  = MK_FP (0xB800,0);

 for (jciclo=(y1-1); jciclo<=(y2-1); jciclo++)
 for (iciclo=(x1-1); iciclo<=(x2-1); iciclo++)
   screen[iciclo+jciclo*80] = attrib;
}


void Ombscreen( int x1, int y1, int x2, int y2, char attrib)
{
 unsigned char far *colscr = MK_FP (0xB800,0);

 for (jciclo=(y1-1); jciclo<=(y2-1); jciclo++)
 for (iciclo=(x1-1); iciclo<=(x2-1); iciclo++)
   colscr[(iciclo+jciclo*80)*2+1] = attrib;
}


void Wrtd( int xpos, int ypos, int colsf, int colch, char *msg)
{
 unsigned int far *screen  = MK_FP (0xB800,0);

 int at = Calcattr(colsf,colch);
 xpos=(ypos-1)*80+(xpos-1);
 for (iciclo=0; iciclo <= strlen(msg)-1; iciclo++)
   screen[xpos+iciclo]= at + *(msg+iciclo);
}


void Xwin( int xu, int yu, int xd, int yd, int colsf, int colbr, char colomb,
	   int tipbor )
{
  char *angoli[] = { "ÛÛÛÛ" , "É»È¼" };      /* Rispett. tipbor=1, tipbor=2 */
  char *bordi[]  = {  "ÛßÜ" , "ºÍÍ"  };	     /* Rispett. tipbor=1, tipbor=2 */
  unsigned int far *screen  = MK_FP (0xB800,0);
  int at;

  colbr++;
  tipbor--;
  yu--;
  at = Calcattr (colsf,colbr);
  Ombscreen(xu+2,yu+2,xd+2,yd+1,colomb);
  Dirscreen(xu+1,yu+2,xd-1,yd-1,at+32);
  for (iciclo=xu; iciclo<=xd-2; iciclo++)             /* Bordi orizzontali */
   {
    screen[iciclo+yu*80]= at + bordi[tipbor][1];
    screen[iciclo+(yd-1)*80]= at + bordi[tipbor][2];
    }
  for (iciclo=yu+2; iciclo<=yd-1; iciclo++)           /* Bordi verticali   */
   {
   screen[(iciclo-1)*80+xu-1] = at + bordi[tipbor][0];
   screen[(iciclo-1)*80+xd-1] = at + bordi[tipbor][0];
   }
  screen[yu*80+xu-1] = at + angoli[tipbor][0];
  screen[yu*80+xd-1] = at + angoli[tipbor][1];
  screen[(yd-1)*80+xu-1] = at + angoli[tipbor][2];
  screen[(yd-1)*80+xd-1] = at + angoli[tipbor][3];
}


void Beep(int Freq, int Delay)
{
  sound(Freq);
  delay(Delay);
  nosound();
}


void Newfont(int Nch, int Newtable[])
{
   regs.h.ah = 0x11;
   regs.h.al = 0x00;
   regs.x.cx = 1;
   regs.x.dx = Nch;
   regs.h.bl = 0;
   regs.h.bh = 16;
   sregs.es = FP_SEG(Newtable);
   int86x(0x10,&regs, &regs, &sregs);
}


void Asscolor(int pal, int valr)
{
   regs.h.ah = 0x10;
   regs.h.al = 0x00;
   regs.h.bl = pal;
   regs.h.bh = valr;
   int86(0x10, &regs, &regs2);
}



void CURSORE_OFF(void)
{
    regs.h.ah = 0x01;
    regs.h.ch = 16;
    regs.h.cl = 0;
    int86(0x10, &regs, &regs2);
}



void CURSORE_ON(void)
{
     regs.h.ah = 0x01;
     regs.h.ch = 6;
     regs.h.cl = 7;
     int86(0x10,&regs,&regs2);
}



void Restcol(void)
{
    for (iciclo=0;  iciclo<6; iciclo++)  Asscolor(iciclo,iciclo);
    for (iciclo=8; iciclo<16; iciclo++)  Asscolor(iciclo,48+iciclo);
    Asscolor(7,7);
    Asscolor(6,20);
}



void Tex(int a,int b)
{
 textcolor(b);
 textbackground(a);
}


void SCROLL(int AH)
  {
   MOUSECURSOR(0);
   regs.h.ah=AH;
   regs.h.al=0x01;
   regs.h.bh=0x1F;
   regs.h.ch=0x00;
   regs.h.cl=0x00;
   regs.h.dh=22;
   regs.h.dl=80;
   int86(0x10,&regs,&regs2);
   MOUSECURSOR(1);
  }


/*
-------------------------------------- Prototipi per gestire il mouse -------
   Attenzione : funzionano solo con driver MOUSE.COM compatibile Microsoft(r)
------------------------------------------------------------------------------
*/

void MOUSEINT(int AX)
{
  regs.x.ax = AX;
  int86( 0x33, &regs, &regs2);
}



int INIT_MOUSE(void)
{
  MOUSEINT(0);
  if( regs2.x.bx == 0) return 1;
  else return 0;                                 /* Install mouse ok...*/
}



void MOUSECURSOR(int CursoreOn_Off)
{
   if( CursoreOn_Off == 1) MOUSEINT(1);              /* Cursor Acceso */
   else MOUSEINT(2);		                     /* Cursor Spento */
}



int MOUSE_PRESSED(int tasto)
{
  regs.x.bx = tasto;            /* 0=tasto sinistra, 1=destro, 2=centrale */
  MOUSEINT(5);
  if( regs2.x.bx >= 1) return 1;           /* Mouse premuto almeno 1 volta */
  else return 0;
}


int MOUSE_RELEASED(int tasto)
{
   regs.x.bx = tasto;
   MOUSEINT(6);
   if( regs2.x.bx >=1) return 1;        /* Mouse rilasciato almeno 1 volta */
   else return 0;
}


void MOUSE_LIMITS(int x1, int y1, int x2, int y2)
{
					      /* Limiti min/max orizzon. */
  regs.x.cx = x1;
  regs.x.dx = x2;
  MOUSEINT(7);
					      /* Limiti min/max verticali */
  regs.x.cx = y1;
  regs.x.dx = y2;
  MOUSEINT(8);
}


void SET_MOUSE_POS (int x, int y)
{
 regs.x.cx = x;
 regs.x.dx = y;
 MOUSEINT(4);
}


void MOUSPEED ( int h_ratio, int v_ratio)
{
  regs.x.cx = h_ratio;
  regs.x.dx = v_ratio;
  MOUSEINT(0x0f);

  /*    Velocita'           h_ratio  v_ratio
	-------------------------------------
	Velocissimo               2       2
	Veloce                    6       6
	Di default circa.....    10      10
	Lento                    28      28
	Lumaca                   70      70
  */
}


int MOUSE_IN_BOX(int x1, int y1, int x2, int y2,int RAPP)

						/* RAPP deve essere :
						   8 in modo testo
						   1 in modo grafico */
{
   MOUSEINT(3);
   regs2.x.cx = regs2.x.cx / RAPP +1 ;
   regs2.x.dx = regs2.x.dx / RAPP +1 ;
   return ((regs2.x.cx >= x1) && ( regs2.x.cx <= x2)
    && ( regs2.x.dx >= y1) && ( regs2.x.dx <= y2));
}


int IN_MOUSE( int x1, int y1, int x2, int y2, int RAPP)
{
  register int mbut, mx, my;

  regs.x.ax = 3;
  int86( 51, &regs, &regs2);
  mbut = regs2.x.bx;
  mx = regs2.x.cx / RAPP +1;
  my = regs2.x.dx / RAPP +1;
  if ((mx >= x1) && ( mx <= x2)
    && ( my >= y1) && ( my <= y2)) return mbut-1;
			      else return -1;
}


struct pointtype MOUSEPOS(void)
{
  struct pointtype POZ;
  MOUSEINT(3);
  POZ.x = regs2.x.cx;
  POZ.y = regs2.x.dx;
  return POZ;
}

/* --------------------- Fine mouse ---------------------- */

