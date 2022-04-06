/*
  written by Brian Whaley 9-7-86
  12690 Magna Carta Rd.
  Herndon, Va     22071
  functions #'s 9, 12, 13, and 14 are not implemented
*/

struct {
  int ax, bx, cx, dx, si, di, ds, es;
       } inregs, outregs;

void mstatus( mstat, nbuttons)   /* reset mouse & ret stat; 0 if installed */
int *mstat, *nbuttons;
{
  inregs.ax = 0;
  int86( 51, &inregs, &outregs);
  *mstat = outregs.ax;
  *nbuttons = outregs.bx;
}

void mpos( mbt, mx, my)   /* return pos and buttons 1,2,3 (lft,rght,both) */
int *mbt, *mx, *my;
{
  inregs.ax = 3;
  int86( 51, &inregs, &outregs);
  *mbt = outregs.bx;
  *mx = outregs.cx;
  *my = outregs.dx;
}

void mbuttonp( mbt, mbstat, mbp, mx, my)
int mbt, *mbstat, *mbp, *mx, *my;
{   /* mbstat = button status, 0,1 down, up
       mbp 0,1 left, right (button number pressed since last call)
       mx,my coords of last press  */
  inregs.ax = 5;
  inregs.bx = mbt;
  int86( 51, &inregs, &outregs);
  *mbstat = outregs.ax;
  *mbp = outregs.bx;
  *mx = outregs.cx;
  *my = outregs.dx;
}

void mshow()
{
  inregs.ax = 1;
  int86( 51, &inregs, &outregs);
}

void mbuttonr( mbt, mbstat, mbp, mx, my)
int mbt, *mbstat, *mbp, *mx, *my;
{   /* mbstat = button status, 0,1 down, up
       mbp 0,1 left, right (button number released since last call)
       mx,my coords of last press  */
  inregs.ax = 6;
  inregs.bx = mbt;
  int86( 51, &inregs, &outregs);
  *mbstat = outregs.ax;
  *mbp = outregs.bx;
  *mx = outregs.cx;
  *my = outregs.dx;
}

void mtshape( scn, cur)   /* software cursor */
int scn, cur;
{
  inregs.ax = 10;
  inregs.bx = 0;
  inregs.cx = scn;
  inregs.dx = cur;
  int86( 51, &inregs, &outregs);
}

void mmotion( hcnt, vcnt)   /* motion in mickeys/sec (i think) */
int *hcnt, *vcnt;
{
  inregs.ax = 11;
  int86( 51, &inregs, &outregs);
  *hcnt = outregs.cx;
  *vcnt = outregs.dx;
}

