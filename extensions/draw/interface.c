// libSDL_draw extension module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "SDL_draw.h"
#include "cbasic.h"
#include "../../basext.h"

#define TOFLOAT(x) ((float64)(x))
#define TOINT(x) ((int32)(x))

typedef unsigned char byte;
typedef unsigned char boolean;
typedef int int32;
typedef unsigned int uint32;
typedef double float64;

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

/******************
 SDL_draw Functions
******************/

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

besFUNCTION(SDL_Draw_Window)
  DIM AS int width, height;
  DIM AS Uint32 videoflags;
  DIM AS const char *title;
  DIM AS SDL_Rect screct;
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
//videoflags = SDL_SWSURFACE | SDL_ANYFORMAT;
  videoflags = SDL_HWSURFACE | SDL_ANYFORMAT;
  besARGUMENTS("ii[z]")
    AT width, AT height, AT title
  besARGEND
  IF (title EQ NULL) THEN_DO title = "";
  screen = SDL_SetVideoMode(width, height, 32, videoflags);
  screct.x = 1;
  screct.y = 1;
  screct.w = 798;  
  screct.h = 598;  
  SDL_SetClipRect(screen, AT screct);
  SDL_WM_SetCaption(title, 0);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Close)
  SDL_EnableUNICODE(SDL_DISABLE);
  SDL_Quit();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Update)
  SDL_UpdateRect(screen, 0, 0, 0, 0);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_CLS)
  DIM AS Uint32 color;
  besARGUMENTS("i")
    AT color
  besARGEND
  SDL_FillRect(screen, AT screen->clip_rect, color);
  SDL_UpdateRect(screen, 0, 0, 0, 0);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_RGB)
  DIM AS const SDL_PixelFormat PTR const format;
  DIM AS const Uint8 r, g, b;
  DIM AS Uint32 color;
  besARGUMENTS("iii")
    AT r, AT g, AT b
  besARGEND
  color = SDL_MapRGB(screen->format, r, g, b);
  besRETURN_LONG(color);
besEND

besFUNCTION(SDL_Draw_Time)
  DIM AS Uint32 ticks;
  ticks = SDL_GetTicks();
  besRETURN_LONG(ticks);
besEND

besFUNCTION(SDL_Draw_Pixel)
  DIM AS Sint16 x, y;
  DIM AS Uint32 color;
  besARGUMENTS("iii")
    AT x, AT y, AT color
  besARGEND
  Draw_Pixel(screen, x, y, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Line)
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint32 color;
  besARGUMENTS("iiiii")
    AT x1, AT y1, AT x2, AT y2, AT color
  besARGEND
  Draw_Line(screen, x1, y1, x2, y2, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Circle)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 r;
  DIM AS Uint32 color;
  besARGUMENTS("iiii")
    AT x0, AT y0, AT r, AT color
  besARGEND
  Draw_Circle(screen, x0, y0, r, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_FillCircle)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 r;
  DIM AS Uint32 color;
  besARGUMENTS("iiii")
    AT x0, AT y0, AT r, AT color
  besARGEND
  Draw_FillCircle(screen, x0, y0, r, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_HLine)
  DIM AS Sint16 x0, y0, x1;
  DIM AS Uint32 color;
  besARGUMENTS("iiii")
    AT x0, AT y0, AT x1, AT color
  besARGEND
  Draw_HLine(screen, x0, y0, x1, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_VLine)
  DIM AS Sint16 x0, y0, y1;
  DIM AS Uint32 color;
  besARGUMENTS("iiii")
    AT x0, AT y0, AT y1, AT color
  besARGEND
  Draw_VLine(screen, x0, y0, y1, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Rect)
  DIM AS Sint16 x, y;
  DIM AS Uint16 w, h;
  DIM AS Uint32 color;
  besARGUMENTS("iiiii")
    AT x, AT y, AT w, AT h, AT color
  besARGEND
  Draw_Rect(screen, x, y, w, h, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_FillRect)
  DIM AS Sint16 x, y;
  DIM AS Uint16 w, h;
  DIM AS Uint32 color;
  besARGUMENTS("iiiii")
    AT x, AT y, AT w, AT h, AT color
  besARGEND
  Draw_FillRect(screen, x, y, w, h, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Ellipse)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 Xradius, Yradius;
  DIM AS Uint32 color;
  besARGUMENTS("iiiii")
    AT x0, AT y0, AT Xradius, AT Yradius, AT color
  besARGEND
  Draw_Ellipse(screen, x0, y0, Xradius, Yradius, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_FillEllipse)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 Xradius, Yradius;
  DIM AS Uint32 color;
  besARGUMENTS("iiiii")
    AT x0, AT y0, AT Xradius, AT Yradius, AT color
  besARGEND
  Draw_FillEllipse(screen, x0, y0, Xradius, Yradius, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_Round)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 w, h, corner;
  DIM AS Uint32 color;
  besARGUMENTS("iiiiii")
    AT x0, AT y0, AT w, AT h, AT corner, AT color
  besARGEND
  Draw_Round(screen, x0, y0, w, h, corner, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_FillRound)
  DIM AS Sint16 x0, y0;
  DIM AS Uint16 w, h, corner;
  DIM AS Uint32 color;
  besARGUMENTS("iiiiii")
    AT x0, AT y0, AT w, AT h, AT corner, AT color
  besARGEND
  Draw_FillRound(screen, x0, y0, w, h, corner, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(SDL_Draw_RND)
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

besFUNCTION(SDL_Draw_Shift)
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

besFUNCTION(SDL_Draw_Rotate)
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

besFUNCTION(SDL_Draw_KeyName)
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

START_FUNCTION_TABLE(SDL_SLFST)
// Ext. module
  EXPORT_MODULE_FUNCTION(versmodu)
  EXPORT_MODULE_FUNCTION(bootmodu)
  EXPORT_MODULE_FUNCTION(finimodu)
// SDL_Draw
  EXPORT_MODULE_FUNCTION(SDL_Draw_Window)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Close)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Update)
  EXPORT_MODULE_FUNCTION(SDL_Draw_CLS)
  EXPORT_MODULE_FUNCTION(SDL_Draw_RGB)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Time)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Pixel)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Line)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Circle)
  EXPORT_MODULE_FUNCTION(SDL_Draw_FillCircle)
  EXPORT_MODULE_FUNCTION(SDL_Draw_HLine)
  EXPORT_MODULE_FUNCTION(SDL_Draw_VLine)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Rect)
  EXPORT_MODULE_FUNCTION(SDL_Draw_FillRect)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Ellipse)
  EXPORT_MODULE_FUNCTION(SDL_Draw_FillEllipse)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Round)
  EXPORT_MODULE_FUNCTION(SDL_Draw_FillRound)
  EXPORT_MODULE_FUNCTION(SDL_Draw_RND)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Shift)
  EXPORT_MODULE_FUNCTION(SDL_Draw_Rotate)
  EXPORT_MODULE_FUNCTION(SDL_Draw_KeyName)
END_FUNCTION_TABLE
