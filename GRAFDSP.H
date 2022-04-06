/*

   ----------------------------------------------------------------------
   Unita' di inclusione "GRAFDSP" versione 1.C                   Nov.1992
   ----------------------------------------------------------------------
   Modulo di procedure di gestione schermo grafico,
   Finestre, SalvaBox, Dialog Box, Input stringhe, Menu', GraphText...
   ecc..


*/
#include <graphics.h>
#include <dos.h>
#include <int1_3.h>


/**************************************************************************
   **									 **
   **		Definizione di tipi e dichiarazione delle variabili      **
   **									 **
   ** N.B.: Tutti i char rappresentano un Byte numerico 0..255           **
   ************************************************************************/

extern char ColorMenu[];
extern char *Tasti[];
extern int kbeep[];

/*
  Def. colori
*/
#define B_TASTO           0           /* Background e colore labels */
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


struct Gitem {
	      int x;
	      int y;
	      char *Gi;
	     };

struct WLimits {
	      int x1;
	      int y1;
	      int x2;
	      int y2;
	      };

  /************************************************************************
   **									 **
   **		Prototipi delle funzioni :				 **
   **								 	 **
   ************************************************************************/

void Swap          ( int *val1, int *val2);
void BarraHelp     ( char *infbar);
void Win           ( int x1,  int y1, int x2, int y2,char col,
		      char coltit, char colbordo, char *msg, int WINUM);
void SalvaBox      ( int x1, int y1, int x2, int y2, char tipo, void *buf[4]);
void TornaBox      ( int x1, int y1, int x2, int y2, char tipo, void *buf[4]);
void MenuMask      ( int xini, int yini, int large, char *MenuOpts[], int dim);
void Cornice       ( int x1, int y1, int x2, int y2, char LUCE);
void GText         ( int x, int y, char ombra, char *testo);
char GetItemOpts   ( int x1, int y1, int amp, char *MenuOpts[],
		      int size_vett, void *buf[4]);
void OKbutton      ( int x, int y);
void WAIT_MOUSE    ( void);
char *XInput       ( int x, int y, char colsf, char colch,
		      char *start_msg, char maxch);
void NewColorBar   ( char c1, char col, int TN);
int  GENbutton     ( int x, int y, char *MSG, char Tipo);
char SELECTbutton  ( struct Gitem *G, char dim );

void Esclama       ( int x, int y);
void Interroga     ( int x, int y);
void NewColorMenu  ( int *CTable );
void Desktop       ( struct Gitem *G );
void key1          ( void );
void key2          ( void );
void key3          ( void );
void key4          ( void );
void Keybeep       ( int key, int dely );
void Ascolta       ( char tipo );
void HELP	   ( void);
