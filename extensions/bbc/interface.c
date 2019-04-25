// Brandy BASIC V graphics extension module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "libbbc.h"
#include "cbasic.h"
#include "../../basext.h"

#define TOFLOAT(x) ((float64)(x))
#define TOINT(x) ((int32)(x))

DIM AS SDL_Surface PTR screen;
DIM AS static int32 lastrandom;
DIM AS static int32 randomoverflow;
DIM AS static float64 floatvalue;

/**************************
 Extension Module Functions
**************************/

besVERSION_NEGOTIATE
  RETURN_FUNCTION((int)INTERFACE_VERSION);
besEND

besSUB_START
  DIM AS long *p;
  besMODULEPOINTER = besALLOC(sizeof(long));
  IF (besMODULEPOINTER EQ NULL) THEN_DO RETURN_FUNCTION(0);
  p = (long*)besMODULEPOINTER;
  RETURN_FUNCTION(0);
besEND

besSUB_FINISH
  DIM AS long *p;
  p = (long*)besMODULEPOINTER;
  IF (p EQ NULL) THEN_DO RETURN_FUNCTION(0);
  RETURN_FUNCTION(0);
besEND

/*********************
 BBC Graphic Functions
*********************/

FUNCTION LOCAL float64 randomfraction(void)
BEGIN_FUNCTION
  uint32 reversed = ((lastrandom>>24)&0xFF)|((lastrandom>>8)&0xFF00)|((lastrandom<<8)&0xFF0000)|((lastrandom<<24)&0xFF000000);
  RETURN_FUNCTION(TOFLOAT(reversed) / 4294967296.0);
END_FUNCTION

LOCAL SUB nextrandom(void)
BEGIN_SUB
  DIM AS int n;
  DEF_FOR (n = 0 TO n < 32 STEP INCR n)
  BEGIN_FOR
    int newbit = ((lastrandom>>19) ^ randomoverflow) & 1;
    randomoverflow = lastrandom>>31;
    lastrandom = (lastrandom<<1) | newbit;
  NEXT
END_SUB

besFUNCTION(BBC_OPEN)
  DIM AS int rtnval;
  DIM AS const char *title;
  besARGUMENTS("[z]")
    AT title
  besARGEND
  srand((unsigned)time(NULL));
  lastrandom=rand();
  randomoverflow = 0;
  rtnval = _OPEN();
  IF (title EQ NULL) THEN_DO title = "";
  SDL_WM_SetCaption(title, 0);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_CLOSE)
  _CLOSE();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_MODE)
  DIM AS int32 mode;
  besARGUMENTS("i")
    AT mode
  besARGEND
  _MODE(mode);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_MODESTR)
  DIM AS int32 xres, yres, colours, greys, xeig, yeig, rate;
  besARGUMENTS("iiiiiii")
    AT xres, AT yres, AT colours, AT greys, AT xeig, AT yeig, AT rate
  besARGEND
  _MODESTR(xres, yres, colours, greys, xeig, yeig, rate);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_NEWMODE)
  DIM AS int32 xres, yres, bpp, rate;
  besARGUMENTS("iiii")
    AT xres, AT yres, AT bpp, AT rate
  besARGEND
  _NEWMODE(xres, yres, bpp, rate);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_MODEFN)
  DIM AS int rtnval;
  rtnval = _MODEFN();
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_ORIGIN)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _ORIGIN(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_GCOL)
  DIM AS int32 action, colour, tint;
  besARGUMENTS("ii[i]")
    AT action, AT colour, AT tint
  besARGEND
  _GCOL(action, colour, tint);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_GCOLRGB)
  DIM AS int32 action, background, red, green, blue;
  besARGUMENTS("iiiii")
    AT action, AT background, AT red, AT green, AT blue
  besARGEND
  _GCOLRGB(action, background, red, green, blue);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_GCOLNUM)
  DIM AS int32 action, background, colnum;
  besARGUMENTS("iii")
    AT action, AT background, AT colnum
  besARGEND
  _GCOLNUM(action, background, colnum);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_COLOURFN)
  DIM AS int32 red, green, blue, rtnval;
  besARGUMENTS("iii")
    AT red, AT green, AT blue
  besARGEND
  rtnval = _COLOURFN(red, green, blue);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_MAPCOLOUR)
  DIM AS int32 colour, physcolour;
  besARGUMENTS("ii")
    AT colour, AT physcolour
  besARGEND
  _MAPCOLOUR(colour, physcolour);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_SETCOLOUR)
  DIM AS int32 background, red, green, blue;
  besARGUMENTS("iiii")
    AT background, AT red, AT green, AT blue
  besARGEND
  _SETCOLOUR(background, red, green, blue);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_SETCOLNUM)
  DIM AS int32 background, colnum;
  besARGUMENTS("ii")
    AT background, AT colnum
  besARGEND
  _SETCOLNUM(background, colnum);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_DEFCOLOUR)
  DIM AS int32 colour, red, green, blue;
  besARGUMENTS("iiii")
    AT colour, AT red, AT green, AT blue
  besARGEND
  _DEFCOLOUR(colour, red, green, blue);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_COLOURTINT)
  DIM AS int32 colour, tint;
  besARGUMENTS("ii")
    AT colour, AT tint
  besARGEND
  _COLOURTINT(colour, tint);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_TINT)
  DIM AS int32 action, tint;
  besARGUMENTS("ii")
    AT action, AT tint
  besARGEND
  _TINT(action, tint);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_TINTFN)
  DIM AS int32 x, y, rtnval;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  rtnval = _TINTFN(x, y);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_PLOT)
  DIM AS int32 code, x, y;
  besARGUMENTS("iii")
    AT code, AT x, AT y
  besARGEND
  _PLOT(code, x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_POINTFN)
  DIM AS int32 x, y, rtnval;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  rtnval = _POINTFN(x, y);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_MOVE)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _MOVE(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_MOVEBY)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _MOVEBY(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_DRAW)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _DRAW(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_DRAWBY)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _DRAWBY(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_POINT)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _POINT(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_POINTBY)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _POINTBY(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_POINTTO)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _POINTTO(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_LINE)
  DIM AS int32 x1, y1, x2, y2;
  besARGUMENTS("iiii")
    AT x1, AT y1, AT x2, AT y2
  besARGEND
  _LINE(x1, y1, x2, y2);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_CIRCLE)
  DIM AS int32 x, y, radius, isfilled;
  besARGUMENTS("iiii")
    AT x, AT y, AT radius, AT isfilled
  besARGEND
  _CIRCLE(x, y, radius, isfilled);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_ELLIPSE)
  DIM AS int32 x, y, majorlen, minorlen, isfilled;
  DIM AS float64 angle;
  besARGUMENTS("iiiiri")
    AT x, AT y, AT majorlen, AT minorlen, AT angle, AT isfilled
  besARGEND
  _ELLIPSE(x, y, majorlen, minorlen, angle, isfilled);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_DRAWRECT)
  DIM AS int32 x1, y1, width, height, isfilled;
  besARGUMENTS("iiiii")
    AT x1, AT y1, AT width, AT height, AT isfilled
  besARGEND
  _DRAWRECT(x1, y1, width, height, isfilled);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_MOVERECT)
  DIM AS int32 x1, y1, width, height, x2, y2, ismove;
  besARGUMENTS("iiiiiii")
    AT x1, AT y1, AT width, AT height, AT x2, AT y2, AT ismove
  besARGEND
  _MOVERECT(x1, y1, width, height, x2, y2, ismove);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_FILL)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _FILL(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_FILLBY)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _FILLBY(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_OFF)
  _OFF();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_ON)
  _ON();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_SETCURSOR)
  DIM AS int32 underline;
  besARGUMENTS("i")
    AT underline
  besARGEND
  _SETCURSOR(underline);
  besRETURNVALUE = NULL;
besEND

// BBC_VDU
besFUNCTION(BBC_VDU)
  DIM AS int32 charvalue;
  besARGUMENTS("i")
    AT charvalue
  besARGEND
  _VDU(charvalue);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_VDUSTR)
  DIM AS char PTR vdustr;
  besARGUMENTS("s")
    AT vdustr
  besARGEND
  _VDUSTR(vdustr);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_VDUFN)
  DIM AS int vduvar, rtnval;
  besARGUMENTS("i")
    AT vduvar
  besARGEND
  rtnval = _VDUFN(vduvar);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_NEWLINE)
  _NEWLINE();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_POS)
  DIM AS int32 rtnval;
  rtnval = _POS();
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_VPOS)
  DIM AS int32 rtnval;
  rtnval = _VPOS();
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_TAB)
  DIM AS int32 x, y;
  besARGUMENTS("ii")
    AT x, AT y
  besARGEND
  _TAB(x, y);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(BBC_RND)
  DIM AS int32 value;
  besARGUMENTS("i")
    AT value
  besARGEND
  IF (value < 0) THEN
    lastrandom = value;
 	  srand((unsigned)time(NULL));
    value=rand();
    randomoverflow = 0;
    besRETURN_LONG(value);
  ELSE_IF (value EQ 0) THEN
    besRETURN_DOUBLE(randomfraction());
  ELSE_IF (value EQ 1) THEN
    nextrandom();
    besRETURN_DOUBLE(randomfraction());
  ELSE
  	nextrandom();
  	besRETURN_LONG(TOINT((randomfraction()*TOFLOAT(value))+1));
  END_IF	
besEND  

besFUNCTION(BBC_SHIFT)
  DIM AS int bp, co, v, x, y, d, p, ar;
  besARGUMENTS("ii[i]")
    AT v, AT p, AT ar
  besARGEND
  IF (ar EQ NULL) THEN_DO ar = 0;
  x = 0xffffffff & v;
  IF (p >= 0) THEN
    y = x << p; // bit shift left
  ELSE
    y = x >> (-p); // bit shift right
  END_IF
//
// SUPPORT FOR ARITHMETIC RIGHT SHIFTS
//
  IF ((ar) AND (p < 0) AND (x & 0x80000000)) THEN
    d = 31 + p;
    IF (d < 0) THEN_DO d = 0; // LIMIT
    bp = 0x80000000;
    DEF_FOR (co = 31 TO co >= d STEP DECR co)
    BEGIN_FOR
      y = y | bp;
      bp = bp >> 1;
    NEXT
  END_IF
  besRETURN_LONG(y);
besEND

besFUNCTION(BBC_ROTATE)
  DIM AS int co, v, x, y, d, p;
  besARGUMENTS("ii")
    AT v, AT p
  besARGEND
  x = 0xffffffff & v;
  d = p & 0x1f;
  DEF_FOR (co = 1 TO co <= d STEP INCR co)
  BEGIN_FOR
    y = x << 1;
    IF (x & 0x80000000) THEN_DO y |= 1;
    x = y;
  NEXT
    besRETURN_LONG(x & 0xffffffff);
besEND

besFUNCTION(BBC_GETKEY)
  DIM AS int value, rtnval;
  besARGUMENTS("i")
    AT value
  besARGEND
  rtnval = _GETKEY(value);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_WAITKEY)
  _WAITKEY();
  besRETURNVALUE = NULL;
besEND  

besFUNCTION(BBC_KEYNAME)
  DIM AS int done, e;
  DIM AS SDL_Event event;
  DIM AS char tmpstr[80];
  DIM AS int value;
  besARGUMENTS("i")
    AT value
  besARGEND
  done = 0;
  e = 1;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (e)
    BEGIN_WHILE
      IF (value EQ 0) THEN
      	e = SDL_PollEvent(AT event);
      	SDL_Delay(20);
      ELSE
        e = SDL_WaitEvent(AT event);
      END_IF
      SELECT_CASE (event.type)
      BEGIN_SELECT
        CASE SDL_KEYDOWN:
          strcpy (tmpstr,"+");
          strcat (tmpstr,SDL_GetKeyName(event.key.keysym.sym));
          besRETURN_STRING(tmpstr);
          END_CASE
        CASE SDL_KEYUP:
          strcpy (tmpstr,"-");
          strcat (tmpstr,SDL_GetKeyName(event.key.keysym.sym));
          besRETURN_STRING(tmpstr);
          END_CASE
        CASE SDL_QUIT:
          done = 1;
          SDL_Quit();
          END_CASE
        CASE_ELSE
          IF (value EQ 0) THEN
            RETURN_FUNCTION(0);
          END_IF
          END_CASE
      END_SELECT
    WEND
  WEND
besEND

besFUNCTION(BBC_MOUSE)
  DIM AS int value, rtnval;
  besARGUMENTS("i")
    AT value
  besARGEND
  rtnval = _MOUSE(value);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(BBC_TIME)
  DIM AS Uint32 ticks;
  ticks = SDL_GetTicks();
  besRETURN_LONG(ticks);
besEND

/**********************
 Exported Function List
**********************/

START_FUNCTION_TABLE(SDL_SLFST)
// Ext. module
  EXPORT_MODULE_FUNCTION(versmodu)
  EXPORT_MODULE_FUNCTION(bootmodu)
  EXPORT_MODULE_FUNCTION(finimodu)
// BBC Graphics
  EXPORT_MODULE_FUNCTION(BBC_OPEN)
  EXPORT_MODULE_FUNCTION(BBC_CLOSE)
  EXPORT_MODULE_FUNCTION(BBC_MODE)
  EXPORT_MODULE_FUNCTION(BBC_MODESTR)
  EXPORT_MODULE_FUNCTION(BBC_NEWMODE)
  EXPORT_MODULE_FUNCTION(BBC_MODEFN)
  EXPORT_MODULE_FUNCTION(BBC_ORIGIN)
  EXPORT_MODULE_FUNCTION(BBC_GCOL)
  EXPORT_MODULE_FUNCTION(BBC_GCOLRGB)
  EXPORT_MODULE_FUNCTION(BBC_GCOLNUM)
  EXPORT_MODULE_FUNCTION(BBC_COLOURFN)
  EXPORT_MODULE_FUNCTION(BBC_MAPCOLOUR)
  EXPORT_MODULE_FUNCTION(BBC_SETCOLOUR)
  EXPORT_MODULE_FUNCTION(BBC_SETCOLNUM)
  EXPORT_MODULE_FUNCTION(BBC_DEFCOLOUR)
  EXPORT_MODULE_FUNCTION(BBC_COLOURTINT)
  EXPORT_MODULE_FUNCTION(BBC_TINT)
  EXPORT_MODULE_FUNCTION(BBC_TINTFN)
  EXPORT_MODULE_FUNCTION(BBC_PLOT)
  EXPORT_MODULE_FUNCTION(BBC_POINTFN)
  EXPORT_MODULE_FUNCTION(BBC_MOVE)
  EXPORT_MODULE_FUNCTION(BBC_MOVEBY)
  EXPORT_MODULE_FUNCTION(BBC_DRAWBY)
  EXPORT_MODULE_FUNCTION(BBC_DRAW)
  EXPORT_MODULE_FUNCTION(BBC_POINT)
  EXPORT_MODULE_FUNCTION(BBC_POINTBY)
  EXPORT_MODULE_FUNCTION(BBC_POINTTO)
  EXPORT_MODULE_FUNCTION(BBC_LINE)
  EXPORT_MODULE_FUNCTION(BBC_CIRCLE)
  EXPORT_MODULE_FUNCTION(BBC_ELLIPSE)
  EXPORT_MODULE_FUNCTION(BBC_DRAWRECT)
  EXPORT_MODULE_FUNCTION(BBC_MOVERECT)
  EXPORT_MODULE_FUNCTION(BBC_FILL)
  EXPORT_MODULE_FUNCTION(BBC_FILLBY)
  EXPORT_MODULE_FUNCTION(BBC_OFF)
  EXPORT_MODULE_FUNCTION(BBC_ON)
  EXPORT_MODULE_FUNCTION(BBC_SETCURSOR)
  EXPORT_MODULE_FUNCTION(BBC_VDU)
  EXPORT_MODULE_FUNCTION(BBC_VDUSTR)
  EXPORT_MODULE_FUNCTION(BBC_VDUFN)
  EXPORT_MODULE_FUNCTION(BBC_NEWLINE)
  EXPORT_MODULE_FUNCTION(BBC_POS)
  EXPORT_MODULE_FUNCTION(BBC_VPOS)
  EXPORT_MODULE_FUNCTION(BBC_TAB)
  EXPORT_MODULE_FUNCTION(BBC_RND)
  EXPORT_MODULE_FUNCTION(BBC_SHIFT)
  EXPORT_MODULE_FUNCTION(BBC_ROTATE)
  EXPORT_MODULE_FUNCTION(BBC_GETKEY)
  EXPORT_MODULE_FUNCTION(BBC_WAITKEY)
  EXPORT_MODULE_FUNCTION(BBC_KEYNAME)
  EXPORT_MODULE_FUNCTION(BBC_MOUSE)  
  EXPORT_MODULE_FUNCTION(BBC_TIME)
END_FUNCTION_TABLE

