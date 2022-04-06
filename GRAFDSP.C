#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <alloc.h>
#include <time.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <grafdsp.h>

/*
  Def. colori
*/
#define B_TASTO           0           /* Background */
#define L_TASTO           1	      /* Light tasto  = lato chiaro   */
#define N_TASTO           2	      /* Normal tasto = lato normale  */
#define D_TASTO           3	      /* Dark tasto   = lato scuro    */
#define RETICOLO	  8

/*
  Def. per GENbutton()
*/
#define BTmask 		  101
#define BTlocked	  102
#define BTclick		  103

#define BTh               26         /* Altezza totale del tasto */
#define BTspace		  4          /* Spazio tra label ed estremi tasto */
#define BTchar            8	     /* Larghezza di un carattere */


#define ON	          1
#define OFF		  0
#define CORNICE		  1               /* Salvataggio aree grafiche */
#define BLOCCO		  2
#define SU 	 	  1
#define SUPIENO		  2
#define GIU		  3
#define GIUPIENO	  4

extern int BTnumber;
extern char *default_msg;

int GrayMenu[] = { 00,00,00,          /* B_TASTO/Background */
		   63,63,63,	      /* L_TASTO */
		   43,43,43,          /* N_TASTO */
		   20,20,20	      /* D_TASTO */
		   };



  /************************************************************************
   **									 **
   **		Implementazione delle funzioni grafiche 		 **
   **								 	 **
   ************************************************************************/



/*-------------------------------------------------------------------------*/
void Win(int x1, int y1, int x2, int y2, char col, char coltit,
	char colbordo, char *msg, int WINUM)
{
 char buffer[40];

 MOUSECURSOR(0);
 setfillstyle(1,col);
 bar(x1,y1,x2,y2);
 if( y2-y1 > 25 ) {             /* Evita titoli per finestre molto piccole */
		   setfillstyle(1,colbordo);
		   bar(x1+5,y1+4,x2-5,y1+18);
		   Cornice(x1+5,y1+4,x2-5,y1+18,SU);
		   }
 Cornice(x1,y1,x2,y2,SU);
 Cornice(x1+1,y1+1,x2-1,y2-1,SU);
 line(x1+3,y2-12,x2-2,y2-12);

				/* Se e' una Dialog Box non mostra il numero
				   progressivo della finestra */

 settextjustify(1,1);
 setcolor(coltit);
 if( col==1) sprintf(buffer,"%d - %s",WINUM,msg);
     else sprintf(buffer,"%s",msg);
 if( strlen(msg)*8+30 < x2-x1) GText(x1+(x2-x1)/2,y1+12,coltit, buffer);
 settextjustify(0,2);
 MOUSECURSOR(1);
}



/*-------------------------------------------------------------------------*/
void SalvaBox(int x1, int y1, int x2, int y2, char tipo, void *buf[4])
{
   unsigned size[4];
   int block;

   if( x2 <= x1) Swap(&x1,&x2);
   if( y2 <= y1) Swap(&y1,&y2);

   MOUSECURSOR(0);
   if (tipo==CORNICE)
	{
	size[0] = imagesize(x1,y1,x2,y1);
	size[1] = imagesize(x2,y1,x2,y2);
	size[2] = imagesize(x1,y2,x2,y2);
	size[3] = imagesize(x1,y1,x1,y2);
	for (block=0; block<=3; block++)
	     buf[block] = malloc(size[block]);
	getimage(x1,y1,x2,y1,buf[0]);
	getimage(x2,y1,x2,y2,buf[1]);
	getimage(x1,y2,x2,y2,buf[2]);
	getimage(x1,y1,x1,y2,buf[3]);
	} else {
		size[0] = imagesize(x1,y1,x2,y2);
		buf[0]  = malloc(size[0]);
		getimage(x1,y1,x2,y2,buf[0]);
		}
   MOUSECURSOR(1);
}


/*-------------------------------------------------------------------------*/
void TornaBox(int x1, int y1, int x2, int y2, char tipo, void *buf[4])
{
   int block;

   if( x2 <= x1) Swap(&x1,&x2);
   if( y2 <= y1) Swap(&y1,&y2);
   MOUSECURSOR(0);
   if( tipo==CORNICE)
	{
	putimage(x1,y1,buf[0], COPY_PUT);
	putimage(x2,y1,buf[1], COPY_PUT);
	putimage(x1,y2,buf[2], COPY_PUT);
	putimage(x1,y1,buf[3], COPY_PUT);
	for (block=0; block<=3; block++) free(buf[block]);
	}
   else {
	putimage(x1,y1,buf[0], COPY_PUT);
	free(buf[0]);
	}
   MOUSECURSOR(1);
}


/*-------------------------------------------------------------------------*/
void GText(int x, int y, char ombra, char *testo)
{
/*  Ombra :   1 = Testo con ombra chiara
	      2 = Testo colore L_TASTO
	      3 = Testo colore D_TASTO
	      altrimenti... Testo Background
*/

  MOUSECURSOR(0);
  if( ombra==1) {
		setcolor(L_TASTO);
		outtextxy(x+1,y+1,testo);
		setcolor(B_TASTO);
		outtextxy(x,y,testo);
		}
  else {
       setcolor(B_TASTO);
       if( ombra==2) setcolor(L_TASTO);
       if( ombra==3) setcolor(D_TASTO);
       outtextxy(x,y,testo);
       }
  MOUSECURSOR(1);
}


/*-------------------------------------------------------------------------*/
void NewColorMenu(int *CTable)
{
  int i;

  setrgbpalette(0,CTable[0],CTable[1],CTable[2]);
  setrgbpalette(1,CTable[3],CTable[4],CTable[5]);
  setrgbpalette(2,CTable[6],CTable[7],CTable[8]);
  setrgbpalette(3,CTable[9],CTable[10],CTable[11]);
}

/*-------------------------------------------------------------------------*/
void Cornice(int x1, int y1, int x2, int y2, char LUCE)
{
  MOUSECURSOR(0);
  if( LUCE==SUPIENO
    || LUCE==GIUPIENO) {
			setfillstyle(1,N_TASTO);
			bar(x1,y1,x2,y2);
			}
  if( LUCE <= SUPIENO) setcolor(L_TASTO);
		  else setcolor(D_TASTO);
  line(x1,y2-1,x1,y1);
  line(x1,y1,x2,y1);
  if( LUCE <= SUPIENO) setcolor(D_TASTO);
		  else setcolor(L_TASTO);
  line(x1+1,y2,x2,y2);
  line(x2,y1+1,x2,y2);
  MOUSECURSOR(1);
}


/*-------------------------------------------------------------------------*/
void MenuMask(int xini, int yini, int large, char *MenuOpts[], int dim)
{
  int i;

  MOUSECURSOR(0);
  Cornice(xini,yini,xini+large,yini+dim*14+23,SUPIENO);
  Cornice(xini+1,yini+1,xini+large-1,yini+dim*14+22,SU);
  for (i=1; i <= dim; i++)
  GText(xini+10, yini+i*14,0,MenuOpts[i-1]);
  MOUSECURSOR(1);
}



/*-------------------------------------------------------------------------*/
void OKbutton(int x, int y)
{
  unsigned char exit = 0;
  int dx = 70,
      dy = 30;

  setcolor(B_TASTO);
  MOUSECURSOR(0);
  rectangle(x-1,y-1,x+dx+1,y+dy+1);
  rectangle(x-2,y-2,x+dx+2,y+dy+2);
  MOUSECURSOR(1);
  Cornice(x,y,x+dx,y+dy,SUPIENO);
  Cornice(x+1,y+1,x+dx-1,y+dy-1,SU);
  GText(x+dx/2-7,y+dy/2-4,1,"Ok");
  MOUSE_PRESSED(1);
  do
  {
    if( IN_MOUSE(x,y,x+dx,y+dy,1)==0)
       {
	MOUSE_RELEASED(0);
	Cornice(x,y,x+dx,y+dy,GIUPIENO);
	Cornice(x+1,y+1,x+dx-1,y+dy-1,GIU);
	GText(x+dx/2-6,y+dy/2-3,3,"Ok");
	while (!MOUSE_RELEASED(0)) {}
        Cornice(x,y,x+dx,y+dy,SUPIENO);
	Cornice(x+1,y+1,x+dx-1,y+dy-1,SU);
	GText(x+dx/2-7,y+dy/2-4,1,"Ok");
	if( MOUSE_IN_BOX(x,y,x+dx,y+dy,1)) exit = 1;
	}
  } while( !exit && !MOUSE_PRESSED(1)) ;
}


/*-------------------------------------------------------------------------*/
int GENbutton(int x, int y, char *MSG, char Tipo)
{
/*-- char Tipo ---------------------------
     BTmask(101)   : solo disegno maschera
     BTlocked(102) : tasto "locked"
     BTclick(103)  : animazione pressione


     Ritorna 1 se e' stato premuto con verifica ( solo per BTclick )
*/

  int len = (strlen(MSG) +BTspace ) *BTchar;
  int exit= -1;

  if( Tipo==BTmask) {
    /*
    Button Mask
    */
    MOUSECURSOR(0);
    setcolor(B_TASTO);
    rectangle(x-1,y-1,x+len+1,y+BTh+1);
    rectangle(x-2,y-2,x+len+2,y+BTh+2);
    MOUSECURSOR(1);
    Cornice(x,y,x+len,y+BTh,SUPIENO);
    Cornice(x+1,y+1,x+len-1,y+BTh-1,SU);
    GText(x+16,y+10,1,MSG);
    } else if( Tipo==BTclick)
   { /*
     Click button
     */
     if( MOUSE_IN_BOX(x,y,x+len,y+BTh-1,1)) {
		Cornice(x,y,x+len,y+BTh,GIUPIENO);
		Cornice(x+1,y+1,x+len-1,y+BTh-1,GIU);
		GText(x+17,y+11,3,MSG);
		while (!MOUSE_RELEASED(0)) { }
		if( MOUSE_IN_BOX(x,y,x+len,y+BTh-1,1)) exit = 1;
		Cornice(x,y,x+len,y+BTh,SUPIENO);
		Cornice(x+1,y+1,x+len-1,y+BTh-1,SU);
		GText(x+16,y+10,1,MSG);
		}
      } else {
	   /*
	   Locked button
	   */
	   MOUSECURSOR(0);
   	   setcolor(B_TASTO);
	   rectangle(x-1,y-1,x+len+1,y+BTh+1);
	   setcolor(N_TASTO);
	   rectangle(x-2,y-2,x+len+2,y+BTh+2);
           MOUSECURSOR(1);
	   Cornice(x,y,x+len,y+BTh,SUPIENO);
	   Cornice(x+1,y+1,x+len-1,y+BTh-1,SU);
	   GText(x+16,y+10,3,MSG);
	   }
  return exit;
}



/*-------------------------------------------------------------------------*/
char SELECTbutton( struct Gitem *G, char dim )
{
  int xm, ym, len;
  char i, exit = -1;

  do
  {
   if( IN_MOUSE(0,0,1024,800,1)==0)
   {
   MOUSE_RELEASED(0);
   xm = MOUSEPOS().x;
   ym = MOUSEPOS().y;
   for (i=0; i<=dim-1; i++)
    {
     len = (strlen(G[i].Gi) +BTspace)*BTchar;
     if( ym >= G[i].y && ym <= G[i].y+30
     && xm >= G[i].x && xm <= G[i].x+len)
      {
       if( GENbutton(G[i].x,G[i].y,G[i].Gi,BTclick)==1) exit = i;
       break;
       }
    }
   }
  } while( exit==-1);
  return exit+1;
}



/*-------------------------------------------------------------------------*/
char GetItemOpts(int x1, int y1, int amp, char *MenuOpts[],
		int size_vett, void *buf[4])
{
  int ok = 0,
      ys,yf,
      y2   = y1+size_vett*14+23;
  char ptr = 1;


  SalvaBox(x1,y1,x1+amp,y2,BLOCCO,buf);
  MenuMask(x1,y1,amp,MenuOpts,size_vett);
  WAIT_MOUSE();
  do
  {
  ys = y1+(ptr-1)*14+10;
  yf = y1+(ptr)*14+10;
  if( MOUSE_IN_BOX(x1,ys,x1+amp,yf,1))
      {
      Cornice(x1+6,ys,x1+amp-6,yf,GIU);
      WAIT_MOUSE();
      }
  else  {
	  setcolor(N_TASTO);
	  MOUSECURSOR(0);
	  rectangle(x1+6,ys,x1+amp-6,yf);
	  MOUSECURSOR(1);
	  if( MOUSEPOS().y >= yf) ptr++;
	  if( MOUSEPOS().y <= ys) ptr--;
	}
  if( ptr > size_vett) ptr=size_vett;
  if( ptr < 1 ) ptr=1;
/*
   USCITA per rinucia selezione
*/
  if( MOUSE_RELEASED(0)) {
			  ok = 1;
			  break;
			  }
  if( MOUSE_RELEASED(1)) {
			  ptr = 0;
			  break;
			  }
  } while( MOUSE_IN_BOX(x1,y1-17,x1+amp,y2,1) && !ok );
  if( !ok) ptr=0;
  TornaBox(x1,y1,999,999,BLOCCO,buf);
  BarraHelp("");
  return ptr;
}



/*-------------------------------------------------------------------------*/
void WAIT_MOUSE(void)
{
 int x = MOUSEPOS().x;
 int y = MOUSEPOS().y;

 do {}
 while( x==MOUSEPOS().x
   &&  y==MOUSEPOS().y
   && IN_MOUSE(0,0,1024,800,1)!=0
   && IN_MOUSE(0,0,1024,800,1)!=1);
}




/*-------------------------------------------------------------------------*/
char *XInput(int x, int y, char colsf, char colch, char *start_msg, char maxch)
{
   int a, chread = 0;
   char buffer[1] = "", outword[80] = "";

   maxch--;
   setfillstyle(1,colsf);
   bar(x-3,y-2,x+8*maxch+8,y+11);
   strcpy(outword,start_msg);
   setcolor(colch);
   if (start_msg !="") outtextxy(x,y,outword);
   chread = strlen(outword);

   a=getch();
   if( a!=8 && a!=13 && chread!=0) {
			  setfillstyle(1,colsf);
			  bar(x-3,y-2,x+8*maxch+8,y+11);
			  outword[0]='\0';
			  sprintf(outword,"%c",a);
                          outtextxy(x,y,outword);
			  }
   chread = strlen(outword);
   x=x+8*chread;

   do {
    if( a!=13) a=getch();
    if( a==8 && chread > 0) { x -= 8;
			      setfillstyle(1,colsf);
			      bar(x,y,x+8,y+8);
			      outword[chread-1] = '\0';
			      chread--; }
    if( a!=0 && a!=13 && a!=27
       && a!=8 && chread <= maxch) {
				    sprintf(buffer,"%c",a);
				    outtextxy(x,y,buffer);
				    strcat(outword,buffer);
				    x += 8;
				    chread++; }
  } while( a!=27 && a!=13);
  if( a==27) outword[0]='\0';
  return outword;
}



/*-------------------------------------------------------------------------*/
void Interroga(int x, int y)
{
 int col = 7;

 MOUSECURSOR(0);
 setcolor(col);
 setfillstyle(1,col);
 circle(x,y,18);
 circle(x,y,13);
 floodfill(x,y-14,col);
 setcolor(0);
 MOUSECURSOR(1);
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
 GText(x-6,y-21,0,"?");
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
}



/*-------------------------------------------------------------------------*/
void Esclama(int x, int y)
{
 int col = 7;

 MOUSECURSOR(0);
 setcolor(col);
 moveto(x,y-20);
 lineto(x-20,y+16); lineto(x+20,y+16); lineto(x,y-20);
 moveto(x,y-12);
 lineto(x-14,y+12); lineto(x+14,y+12); lineto(x,y-12);
 setfillstyle(1,0);
 bar(x-1,y-7,x+1,y+5);
 bar(x-1,y+8,x+1,y+10);
 setfillstyle(1,col);
 floodfill(x,y+14,col);
 MOUSECURSOR(1);
}



/*-------------------------------------------------------------------------*/
void BarraHelp(char *infbar)
{

 Cornice(0,463,639,479,SUPIENO);
 if( !strcmp(infbar,""))
   GText(5,468,0,default_msg);
   else GText(18,468,0,infbar);
}



/*-------------------------------------------------------------------------*/
void Swap(int *val1, int *val2)
{
 int temp;

 temp = *val1;
 *val1 = *val2;
 *val2 = temp;
}









