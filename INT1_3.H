/*
   ----------------------------------------------------------------------
   Unita' di inclusione "INT1_3" versione 1.C                    Lug.1992
   ----------------------------------------------------------------------
   Modulo di routines e procedure al Bios,
   gestione schermo,
   interrupt,
   mestione mouse,ecc..

   *************************************************************************
   **		Definizione di tipi e dichiarazione delle variabili      **
   ************************************************************************/



extern int 	iciclo,
	jciclo;

extern union  REGS      regs,regs2;
extern struct SREGS     sregs;

/**************************************************************************
   **								  	 **
   **           Prototipi delle funzioni di interfaccia                  **
   **								 	 **
   ************************************************************************


int  Calcattr   ( int sfondo, int chcol);
void Dirscreen  ( int x1, int y1, int x2, int y2, int attrib);
void Ombscreen  ( int x1, int y1, int x2, int y2, char attrib);
void Wrtd       ( int xpos, int ypos, int colsf, int colch, char *msg);
void Xwin       ( int xu, int yu, int xd, int yd, int colsf,
		  int colbr, char colomb, int tipbor );
void Beep       ( int Freq, int Delay);
void Newfont    ( int Nch, int Newtable[]);
void Asscolor   ( int pal, int valr);
void Restcol    ( void );
void CURSORE_OFF( void );
void CURSORE_ON ( void );
void Tex        ( int a,int b);
void SCROLL     ( int AH);
char *GetSData  ( void);
char *GetSTime  ( void);
char CodScan    ( int *fl );


/**************************************************************************
   **           Prototipi per la gestione del Mouse grafico/testo        **
   ************************************************************************/

void MOUSEINT       ( int AX);
int  INIT_MOUSE     ( void);
void MOUSPEED       ( int h_ratio, int v_ratio);
void MOUSECURSOR    ( int CursoreOn_Off);
int  MOUSE_PRESSED  ( int tasto);
int  MOUSE_RELEASED ( int tasto);
void MOUSE_LIMITS   ( int x1, int y1, int x2, int y2);
void SET_MOUSE_POS  ( int x, int y);
void MOUSPEED       ( int h_ratio, int v_ratio);
int  MOUSE_IN_BOX   ( int x1, int y1, int x2, int y2, int RAPP);
int  IN_MOUSE       ( int x1, int y1, int x2, int y2, int RAPP);
struct pointtype MOUSEPOS(void);