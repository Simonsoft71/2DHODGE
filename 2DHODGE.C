#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <alloc.h>
#include <time.h>
#include <dir.h>
#include <string.h>
#include <int1_3.h>
#include <grafdsp.h>
#include <dos.h>

#define ON_color  	15
#define OFF_color  	0
#define LIM_DIM_WIN  	420
#define LifeWinUP	45
#define LifeWinDW  	320

int GreenMenu[]= { 00,00,00,
		   55,63,55,
		   31,41,31,
		   12,28,12
		   };

char *MenuStart[] = { "A mid \"k\"",
		      "All \"k\" p=1",
		      "All \"k\" p=.5",
		      "Random p=0.5",
		      "Random p=0.xx",
		      "1í1í1í1í...",
		      "Draw",
		     };

char *MenuProb[] = { "p=0.01",
		     "p=0.02",
		     "p=0.03",
		     "p=0.05",
		     "p=0.10",
		     "p=0.20",
		     "p=0.40",
		     "p=0.80",
		     "p=0.85",
		     "p=0.95",
		     "p=0.99"
		      };



int MAXV = 158;
int MAXH = 158;
int DV   = 55;
int DH   = 55;
int prob = 0.2;
int deltaLW = LifeWinDW-LifeWinUP;
int iciclo;
int jciclo;
/*---------------------- Parametri ----------------------*/
float p1 = 0.3234;
float p2 = 1;
int g_vel = 0;
int kappa = 13;
int maxkappa = 256;
int show = 0;


typedef char Gen[158+2][158+2];
union  REGS  regs,regs2;
struct SREGS sregs;
char *default_msg = "2D-HodgePodge, Gen.'94: 2D parametric CA";

/* prototipi funzioni: */

void Desk		( void);
void GenNulla		( Gen *gp);
void Out_gen		( int dx, int dy, int time, Gen *gp);
void Itera_Rule		( Gen *vgp, Gen *ngp);
void Itera		( int gen_t, Gen *gp);
void CollegaBordi	( Gen *gp);
void PSlide		( int bl, int sl, int br, int ls, int kappa );
void PrintKappa		( int k );
int GetSlide		( int bl, int sl, int br, int ls, int kappa );
unsigned char SWProb	( float p );
void Field		( int x1, int y1, int len, char *titolo, char *text );
void OutCoord		( int x, int y, int xc, int yc );
char GetSpace		( int *nv, int *nh );
void Saluti		( void );
char DrawBit		( Gen *ng );


int main( int argc, char *argv[] )
{
  struct Gitem S[]  = { 10, 405, "K-",
		       248, 405, "K+",
			 5,  5,  "Quit",
		       304, 405, "Show",
			14, 365, "Start gen >",
		       304, 365, "Window",
			64, 405, "                  ",
			450,415, "g+",
			395,415, "g-"};

  int Drive = 9,
      Modo  = 2, i, buttons = 9;
  int gen_t  = 4,
      gent_hlp,
      prob_hlp;
  int nv, nh, x, y;
  char buf[15];
  char item;
  Gen g[2];
  void *Menu=NULL;

  if( INIT_MOUSE()==1) {
   printf("\nSorry, this program requires the MOUSE. Buy it, stingy!...");
   exit(0);
   }

  DV = atoi(argv[1]);
  DH = atoi(argv[2]);
  if( DV==0 && DH==0) {
			DH=20;
			DV=20;
			}
  if( argc>2) {
    if( DV > MAXV) {
		 DV=MAXV;
		 printf("Max vertical # of cells:  %d!\n",MAXV);
		 }
    if( DH > MAXH) {
		 DH=MAXH;
		 printf("Max horizontal # of cells: %d!\n",MAXH);
		 }
     }
  printf("Options: 2DHODGE [vert][horiz], hit a key to begin...\n");
  getch();

  registerbgidriver( EGAVGA_driver);
  initgraph( &Drive, &Modo,"");
  MOUSE_LIMITS(0,0,639,479);
  MOUSECURSOR(1);
  Desk();
  for (i=0; i<=buttons-1; i++) GENbutton(S[i].x, S[i].y, S[i].Gi, BTmask);
  Cornice(64,405,144,430,GIU);

  strcpy( buf, MenuStart[gen_t-1]);
  Field(155, 366, 15, "Actual:", buf);

  sprintf( buf, "%d", g_vel);
  Field(405, 380, 5, "Actual:", buf);

  PSlide( 66, 15, 175, 150, kappa );
  srand((unsigned int)time(NULL));

  Itera( gen_t, g);

  do {
  if( kbhit() ) {
		 Saluti();
		 exit(1);
		 }
  /*-Main loop-*/
  item = SELECTbutton( S, buttons);
  if( item==1) {
		 kappa--;
		 if( kappa < 2) kappa = maxkappa;
		 PSlide( 66, 15, 175, 150, kappa);
		 Itera(gen_t, g);
		 } else
  if( item==2) {
		 kappa++;
		 if( kappa > maxkappa ) kappa = 2;
		 PSlide(66, 15, 175, 150, kappa);
		 Itera(gen_t, g);
		 } else
  if( item==4) {
		show = 1;
		Itera(gen_t, g);
		show = 0;
		} else
  if( item==5) {
		/* Start Gen */
		BarraHelp("Choose the initial configuration of the universe...");
		gent_hlp = gen_t;
		gen_t = GetItemOpts(MOUSEPOS().x-35,
		S[4].y-60,130,MenuStart,7,Menu);
		if( gen_t==5) {
		   prob_hlp = prob;
		   prob = GetItemOpts(MOUSEPOS().x-20,
		   MOUSEPOS().y-135,70,MenuProb,11,Menu);
		   if( prob==0) prob = prob_hlp;
		}
		if( gen_t==0) gen_t = gent_hlp;
		if( gen_t==7) DrawBit(g);
		Itera(gen_t, g);
		if( gen_t==7) gen_t = gent_hlp;
	       } else
  if( item==6) {
		nv = DV;
		nh = DH;
		if( GetSpace( &nv, &nh)>=0) {
					     DV = nv;
					     DH = nh;
					     }
		Itera(gen_t, g);
		} else
  if( item==7) {
		kappa = GetSlide( 66, 15, 175, 150, kappa );
		PSlide( 66, 15, 175, 150, kappa);
		Itera(gen_t, g);
		}
  if( item==8) {
		g_vel++;
		sprintf( buf, "%d", g_vel);
		Field(405, 380, 5, "Actual:", buf);
		}
  if( item==9) {
		g_vel--;
		sprintf( buf, "%d", g_vel);
		Field(405, 380, 5, "Actual:", buf);
		}



  } while( item!=3 );
  Saluti();
  return 1;
}

void GenNulla(Gen *gp)
{
    int  v, h;
    for (v = 0; v <= DV-1; v++)
	for( h = 0; h <= DH-1; h++)
	(*gp)[v][h]=0;
}

unsigned char SWProb( float p)    /* Setta una cella con probabilita' p */
{
 int r;
 char ret = 0;

 r = random(10000);
 if( r < p*10000) ret=random(kappa);
 return ret;
}


void Start_Gen( int gen_type, Gen *gp)
{
  int v,h,i;
  char buf[50], allk = 0;
  float Dens[] = { 0.5,
		  0.01, 0.02, 0.03, 0.05, 0.10,
		  0.20, 0.40, 0.80, 0.85, 0.95, 0.99 };

  /* Un 1 a met…,
     Un k p=1/2 ,
     Tutti k p=1,
     Random p=0.5,
     Random p=0.xx,
     1í1í1í1í...,
     Draw */

  strcpy( buf, MenuStart[gen_type-1]);
  Field(155, 366, 15, "Actual:", buf);
  if( gen_type==7) return;
  allk = random(kappa-1)+1;
  for (v = 0; v <= DV-1; v++)
     for( h = 0; h <= DH-1; h++)
     {
       if( gen_type==1 || gen_type==7) (*gp)[v][h] = 0; else
       if( gen_type==2) (*gp)[v][h] = allk; else
       if( gen_type==3) (*gp)[v][h] = SWProb(1); else
       if( gen_type==4) (*gp)[v][h] = SWProb(0.5); else
       if( gen_type==5) (*gp)[v][h] = SWProb(Dens[prob]); else
       if( gen_type==6) (*gp)[v][h] = h%2;
     }
  if( gen_type==1 ) (*gp)[DV/2][DH/2] = SWProb(1);
}

void Itera_Rule(Gen *vgp, Gen *ngp)
{
  int v,h;
  int sum, infect = 0, sick = 0;
  char
  A, B, C,
  D, CE,F,
  G, H, I;

  GenNulla(ngp);
  for (v = 1; v <= DV-2; v++)
    for (h = 1; h <= DH-2; h++) {

	   CE = (*vgp)[v][h];
	   infect = 0;
	   sick = 0;

	   A = (*vgp)[v+1][h-1];
	   B = (*vgp)[v+1][h];
	   C = (*vgp)[v+1][h+1];

	   D = (*vgp)[v][h-1];
	   CE= (*vgp)[v][h];
	   F = (*vgp)[v][h+1];

	   G = (*vgp)[v-1][h-1];
	   H = (*vgp)[v-1][h];
	   I = (*vgp)[v-1][h+1];

	   sum = A+B+C+D+F+G+H+I+CE ;

	   if( A< kappa-1 && A>0) infect++; else
	   if( A==kappa-1) sick++;

	   if( B< kappa-1 && B>0) infect++; else
	   if( B==kappa-1) sick++;

	   if( C< kappa-1 && C>0) infect++; else
	   if( C==kappa-1) sick++;

	   if( D< kappa-1 && D>0) infect++; else
	   if( D==kappa-1) sick++;

	   if( F< kappa-1 && F>0) infect++; else
	   if( F==kappa-1) sick++;

	   if( G< kappa-1 && G>0) infect++; else
	   if( G==kappa-1) sick++;

	   if( H< kappa-1 && H>0) infect++; else
	   if( H==kappa-1) sick++;

	   if( I< kappa-1 && I>0) infect++; else
	   if( I==kappa-1) sick++;

	   if( CE==0) CE = infect/p1 + sick/p2; else
	   if( CE > 0 && CE < kappa-1)
			CE = min(kappa-1, sum/(infect+0.0000002)+g_vel); else
	   if( CE==kappa-1) CE = 0;

	   (*ngp)[v][h] = CE;           /* t+1 */
	    }
}

void Itera( int gen_t, Gen *gp)
{
 int i, time=0, gcorr = 0, gnew, dx = 10, dy = 10, step = 3;
 char buf[10];

 Cornice(455,55,465+DH,65+DV,SUPIENO);
 setfillstyle(1, 0);
 MOUSECURSOR(0);
 bar(8,LifeWinUP,8+LIM_DIM_WIN,LifeWinDW);
 MOUSECURSOR(1);
 sprintf(buf, "%dx%d", DV, DH);
 GText(455, 70+DV, 3, buf);
 Start_Gen( gen_t, gp);
 if (show) {
	     setfillstyle(1, 1);
	     MOUSECURSOR(0);
	     bar(0,0,640,480);
	     MOUSECURSOR(1);
	     Out_gen( dx, dy, time, gp);
	     sprintf(buf, "t=%d", time);
	     GText( dx, dy+DV+5, 0, buf);
	     } else Out_gen( 460, 60, time, gp);

 setcolor(0);
 MOUSE_LIMITS(0,320,639,479);
 do {
      time++;
      gnew = 1-gcorr;                         /*indice nuova gen <5>*/
      Itera_Rule(gp+gcorr,gp+gnew);

      if( show && (time % step)==0) {
      dx = dx + DH + 10;

      Out_gen( dx, dy, time, gp+gnew);
      sprintf(buf, "t=%d", time);
      GText( dx, dy+DV+5, 0, buf);

      if (dx > 500) {
      dy = dy + DV + 20;
      dx = -DH;
      }

      }
      else if( !show) Out_gen( 460, 60, time, gp+gnew);
      gcorr = gnew;
      if( IN_MOUSE(0,0,999,999,1)==0
	  || IN_MOUSE(0,0,999,999,1)==1) break;
      if( time%(LIM_DIM_WIN-2)==0) {
      setfillstyle(1, 0);
      bar(8,LifeWinUP,8+LIM_DIM_WIN,LifeWinDW);
      }
  } while( !kbhit());
 MOUSE_LIMITS(0,0,639,479);
}

void Out_gen(int dx, int dy, int time, Gen *gp)
{
  long ColCell[256];
  int v, h, col = 0;
  int dxd=10, hg=0;
  unsigned long life=0;

  for (v = 0; v < kappa; v++) ColCell[v]=0;

  for (v = 0; v <= DV-1; v++) {
    for (h = 0; h <= DH-1; h++) {
	col = (*gp)[v][h];
	ColCell[ col ]++;
	if( col < kappa && col > 0)
			{
			 putpixel(dx+h,dy+v, (col % kappa)+3 );
			 life = life + 1;
                         if( IN_MOUSE(0,0,999,999,1)==0
				|| IN_MOUSE(0,0,999,999,1)==1) break;
			 }
		else putpixel(dx+h,dy+v,0);   /* Cellula quiescente */
	}
  }

  if( show==0) for (v = 0; v < kappa; v++)
  {
   hg = ColCell[v]*deltaLW/(DV*DH);
   putpixel(dxd+time%(LIM_DIM_WIN-2), LifeWinDW-hg , (v % kappa)+3);
   }
}


void Desk( void)
{
 int i;

 NewColorMenu(GreenMenu);
 Win(0,0,640,480,2,3,1,"2D parametric CA, 9-cell neighbourhood",0);
 Cornice(8,358,392,398,SU);
 for (i=LifeWinUP+4; i<=LifeWinDW; i += 4) line(LIM_DIM_WIN+12, i,
						LIM_DIM_WIN+18, i);
 for (i=4; i<=15; i++) setpalette(i,i);

 setrgbpalette(4,38,6,6);
 setrgbpalette(5,63,32,0);
 setrgbpalette(6,63,55,0);
 setrgbpalette(7,52,63,34);
 setrgbpalette(8,16,62,20);
 setrgbpalette(9,25,49,17);
 setrgbpalette(10,0,40,36);
 setrgbpalette(11,0,23,47);
 setrgbpalette(12,9,0,47);
 setrgbpalette(13,34,0,31);
 setrgbpalette(14,50,0,38);
 setrgbpalette(15,63,0,44);
 Cornice(10,325,280,350,SU);
 for (i=4; i<=15; i++) {
 setfillstyle(1,i);
 bar((i-4)*20+20, 330, (i-4)*20+40, 345);
 }
}

void PSlide( int bl, int sl, int br, int ls, int kappa )
{
 /* Big-left        >bl±±±±±±±±±±sl±br±<  */
 /* Slide lungh. */
 /* Big-right    */
 int x=bl;

 Cornice(64,405,64+br,430,GIUPIENO);
 x = kappa*ls/maxkappa;
 Cornice(bl+x,408, bl+x+sl, 428, SU);
 PrintKappa( kappa );
}

int GetSlide( int bl, int sl, int br, int ls, int kappa )
{
 char ext = -1;

 /* Big-left        >bl±±±±±±±±±±sl±br±<  */
 /* Slide lungh. */
 /* Big-right    */

 int x, oldkappa = kappa;

 BarraHelp("Left Click <- Confirm, Right Click -> Abort");
 Cornice(bl-2,405,bl-2+br,430,GIUPIENO);
 MOUSE_LIMITS(bl,405,bl+br-sl-4,430);
 Cornice(MOUSEPOS().x,408, MOUSEPOS().x+sl, 428, SU);
 kappa = (MOUSEPOS().x-bl)*maxkappa/ls+2;
 WAIT_MOUSE();

 do {
 Cornice(bl-2,405,bl-2+br,430,GIUPIENO);
 kappa = (MOUSEPOS().x-bl)*maxkappa/ls+2;
 MOUSECURSOR(0);
 Cornice(MOUSEPOS().x,408, MOUSEPOS().x+sl, 428, SU);
 MOUSECURSOR(1);
 WAIT_MOUSE();
 ext = IN_MOUSE(0,0,640,480,1);
 PrintKappa(kappa);
 } while( ext==-1);
 if( ext==1) kappa = oldkappa;
 return kappa;
}

void Group( int x1, int y1, int x2, int y2, char *text)
{
  Cornice(x1,y1,x2,y2,SU);
  setcolor( L_TASTO );
  line( x2+1,y1+1,x2+1,y2+1);
  line( x2-1,y2+1,x1-1,y2+1);
  setcolor(  D_TASTO );
  line( x1-1,y2, x1-1,y1-1);
  line( x1, y1-1, x2, y1-1);
  setfillstyle(1, N_TASTO );
  bar(x1+8,y1-3,x1+8+(strlen(text)+2)*8,y1+3);
  GText(x1+17,y1-3,3,text);
}

void Field( int x1, int y1, int len, char *titolo, char *text )
{
 GText( x1,y1,3, titolo);
 setfillstyle(1, L_TASTO);
 bar( x1, y1+11, x1+8*(len+1), y1+23);
 setcolor(  D_TASTO);
 rectangle(x1-1, y1+10, x1+8*(len+1)+1, y1+24);
 GText(x1+3, y1+14, 0, text);
}

void OutCoord(int x, int y, int xc, int yc)
{
  char cvx[8],cvy[8];

  Cornice(x,y-2,x+70,y+8,SUPIENO);
  itoa(xc,cvx,10);
  itoa(yc,cvy,10);
  strcat(cvx,",");
  GText(x+3,y,4,strcat(cvx,cvy));
}


char GetSpace( int *nv, int *nh )
{
   struct pointtype START, FINE;
   int ox = 460,
       oy = 60;
   char ok = 0;

   BarraHelp("Window sizing.                         Delta: ëx,ëy");
   MOUSECURSOR(0);
   setfillstyle(1, B_TASTO);
   bar(ox, oy, ox + MAXH, oy + MAXV );
   MOUSECURSOR(1);
   MOUSE_LIMITS(ox, oy, ox + MAXH, oy + MAXV );
   SET_MOUSE_POS( ox+DH, oy+DV );
   MOUSE_PRESSED(0);

   while( !MOUSE_PRESSED(0) ) {

	FINE = MOUSEPOS();

	   setlinestyle(DASHED_LINE, 1, 1);
	   setcolor(L_TASTO);
	   MOUSECURSOR(0);
	   rectangle(ox, oy, FINE.x, FINE.y);
	   MOUSECURSOR(1);
	   setlinestyle(SOLID_LINE, 1, 1);

	*nh = MOUSEPOS().x-ox;
	*nv = MOUSEPOS().y-oy;

	WAIT_MOUSE();

	   setcolor(B_TASTO);
	   MOUSECURSOR(0);
	   rectangle(ox, oy, FINE.x, FINE.y);
	   MOUSECURSOR(1);

	OutCoord(432,468,MOUSEPOS().x-ox, MOUSEPOS().y-oy);
	if( MOUSE_PRESSED(1)) ok=-1;
   }

   if( ok==-1) BarraHelp("The old dimension is kept...");
   MOUSE_LIMITS(0,0,639,460);
   setfillstyle(1, N_TASTO);
   MOUSECURSOR(0);
   bar(ox-10, oy-10, ox + MAXH + 10, oy + MAXV + 30);
   MOUSECURSOR(1);
   return ok;
}

void PrintKappa( int k )
{
  int buf[30];

  sprintf(buf," Number of microstates k: %d", k );
  Cornice(8,440,300,460,SUPIENO);
  GText(10,446,1,buf);
}

void Saluti( void )
{
 textmode(C80);
 printf("Thank you for using 2DHODGE, see you...\n");
 printf("The author is available on Internet at: magi@ghost.dsi.unimi.it");
}

char DrawBit( Gen *ng )
{
   int ox = 460,
       oy = 60, x, y;
   char ok = 0;

   BarraHelp("Draw the start configuration.  Left Click=on, Right Click=stop");
   MOUSE_PRESSED(0);
   setfillstyle(1, B_TASTO);
   GenNulla( ng );
   Out_gen( 460, 60, 1, ng );

   MOUSE_LIMITS(ox, oy, ox + DH-1, oy + DV-1);
   while( IN_MOUSE(0,0,640,480,1)!=1 && ok==0) {

   if( IN_MOUSE(0,0,640,480,1)==0) {
	x = MOUSEPOS().x;
	y = MOUSEPOS().y;
        MOUSECURSOR(0);
	putpixel( x, y, 1);
	MOUSECURSOR(1);

	(*ng)[y-oy][x-ox] = 1;
	}
   if( IN_MOUSE(0,0,640,480,1)==1) ok=1;
   }
   MOUSE_LIMITS(0,0,639,460);
   IN_MOUSE(0,0,640,480,1);
}

