// ScriptBasic SDL_gxf Extension Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "SDL_framerate.h"
#include "SDL_gfxPrimitives.h"
#include "../../basext.h"
#include "cbasic.h"

typedef int int32;
typedef unsigned int uint32;
typedef double float64;

#define BORDER  10
#define WIDTH 640
#define HEIGHT  480

DIM AS static SDL_Surface PTR screen;
DIM AS static FPSmanager fpsm;

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

/*************
 SDL Functions
*************/

besFUNCTION(gfx_Window)
  DIM AS int width, height;
  DIM AS Uint32 videoflags;
  DIM AS Uint8  video_bpp;
  DIM AS const char *title;
  DIM AS const SDL_VideoInfo *info;
  besARGUMENTS("ii[z]")
    AT width, AT height, AT title
  besARGEND
  IF (title EQ NULL) THEN_DO title = "";
  IF (SDL_Init(SDL_INIT_VIDEO) < 0 ) THEN
    PRINT_FILE (stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
    exit(1);
  END_IF
  atexit(SDL_Quit);
  info = SDL_GetVideoInfo();
  IF (info->vfmt->BitsPerPixel > 8) THEN
    video_bpp = info->vfmt->BitsPerPixel;
  ELSE
    video_bpp = 16;
  END_IF
//  videoflags = SDL_SWSURFACE | SDL_SRCALPHA | SDL_RESIZABLE | SDL_DOUBLEBUF;
  videoflags = SDL_HWSURFACE | SDL_SRCALPHA ;
  IF ((screen = SDL_SetVideoMode(width,height,video_bpp,videoflags)) EQ NULL ) THEN
    PRINT_FILE (stderr, "Couldn't set %ix%i %i bpp video mode: %s\n",width,height,video_bpp,SDL_GetError());
    exit(2);
  END_IF
//SDL_SetAlpha(screen, SDL_SRCALPHA, 0);
  SDL_SetAlpha(screen, 0, screen->format->alpha);
  SDL_WM_SetCaption(title, 0);
  SDL_EnableUNICODE(SDL_ENABLE);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  besRETURN_LONG(screen);
besEND

besFUNCTION(gfx_Close)
  SDL_EnableUNICODE(SDL_DISABLE);
  SDL_Quit();
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_Update)
  DIM AS SDL_Surface *this_surface;
  besARGUMENTS("[i]")
    AT this_surface
  besARGEND
  IF (this_surface EQ 0 OR this_surface EQ NULL) THEN_DO this_surface = screen;
  SDL_Flip(this_surface);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_ClearScreen)
  DIM AS SDL_Surface *this_surface;
  DIM AS int rtnval;
  DIM AS Uint32 color;
  besARGUMENTS("ii")
    AT this_surface, AT color
  besARGEND
  IF (this_surface EQ 0) THEN_DO this_surface = screen;
  rtnval = SDL_FillRect(this_surface, 0, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_SDL_SetClipRect)
  DIM AS int x1, y1, x2, y2;
  DIM AS SDL_Rect clip;
  DIM AS  Uint32 color;
  DIM AS SDL_Surface *this_surface;
  besARGUMENTS("iiiii")
    AT this_surface, AT x1, AT y1, AT x2, AT y2
  besARGEND
  IF (this_surface EQ 0) THEN_DO this_surface = screen;
  color = SDL_MapRGBA(this_surface->format, 0,0,0,0);
  clip.x = x1;
  clip.y = y1;
  clip.w = x2-(x1-1);
  clip.h = y2-(y1-1);
  SDL_SetClipRect(this_surface, AT clip);
  SDL_FillRect(this_surface, NULL, color);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_Time)
  DIM AS Uint32 ticks;
  ticks = SDL_GetTicks();
  besRETURN_LONG(ticks);
besEND

besFUNCTION(gfx_Shift)
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

besFUNCTION(gfx_Rotate)
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

besFUNCTION(gfx_GetKey)
  DIM AS int value, done, e;
  DIM AS SDL_Event event;
  besARGUMENTS("i")
    AT value
  besARGEND
  done = 0;
  e = 1;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (e)
    BEGIN_WHILE
      IF (value EQ 0 OR value EQ 2 OR value EQ 4) THEN
        e = SDL_PollEvent(AT event);
        SDL_Delay(20);
      ELSE
        e = SDL_WaitEvent(AT event);
      END_IF
      SELECT_CASE (event.type)
      BEGIN_SELECT
        CASE SDL_KEYDOWN:
          IF (value EQ 0 OR value EQ 1) THEN
            besRETURN_LONG(event.key.keysym.unicode);
          ELSE_IF (value EQ 2 OR value EQ 3) THEN
            besRETURN_LONG(event.key.keysym.sym);
          ELSE_IF (value EQ 4 OR value EQ 5) THEN
            besRETURN_LONG(event.key.keysym.scancode);
          END_IF
          END_CASE
        CASE SDL_QUIT:
          done = 1;
          SDL_Quit();
          besRETURN_LONG(0);
          END_CASE
        CASE_ELSE
          IF (value EQ 0 OR value EQ 2 OR value EQ 4) THEN
            besRETURN_LONG(0);
          END_IF
          END_CASE
      END_SELECT
    WEND
  WEND
  besRETURN_LONG(0);
besEND

besFUNCTION(gfx_WaitKey)
  DIM AS int done;
  DIM AS SDL_Event event;
  done = 0;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (SDL_WaitEvent(AT event))
    BEGIN_WHILE
      SELECT_CASE (event.type)
      BEGIN_SELECT
        CASE SDL_KEYDOWN:
          done = 1;
          EXIT_SUB
          END_CASE
        CASE SDL_QUIT:
          done = 1;
          SDL_Quit();
          END_CASE
        CASE_ELSE
          END_CASE
      END_SELECT
    WEND
  WEND
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_KeyName)
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
          RETURN_FUNCTION(0);
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

besFUNCTION(gfx_Mouse)
  DIM AS int value, done, e;
  DIM AS SDL_Event event;
  besARGUMENTS("i")
    AT value
  besARGEND
  done = 0;
  e = 1;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (e)
    BEGIN_WHILE
    IF (value EQ 2) THEN
      e = SDL_PollEvent(AT event);
      SDL_Delay(20);
    ELSE
      e = SDL_WaitEvent(AT event);
    END_IF
      SELECT_CASE (value)
      BEGIN_SELECT
        CASE 0:
          IF (event.type EQ SDL_MOUSEMOTION) THEN
            besRETURN_LONG(event.motion.x);
          END_IF
          END_CASE
        CASE 1:
          IF (event.type EQ SDL_MOUSEMOTION) THEN
            besRETURN_LONG(event.motion.y);
          END_IF
          END_CASE
        CASE 2:
        CASE 3:
          IF (event.button.button EQ SDL_BUTTON_LEFT) THEN
            besRETURN_LONG(1);
          ELSE_IF (event.button.button EQ SDL_BUTTON_MIDDLE) THEN
            besRETURN_LONG(2);
          ELSE_IF (event.button.button EQ SDL_BUTTON_RIGHT) THEN
            besRETURN_LONG(3);
          END_IF
          END_CASE
        CASE_ELSE
          IF (value EQ 2) THEN
            besRETURN_LONG(0);
          END_IF
      END_SELECT
    WEND
    done = 1;
  WEND
  besRETURN_LONG(0);
besEND

besFUNCTION(gfx_CreateSurface)
  DIM AS Uint32 flags, rmask, gmask, bmask, amask;
  DIM AS int width, height, depth;
  DIM AS SDL_Surface PTR this_surface;
  besARGUMENTS("ii[i]")
    AT width, AT height, AT depth
  besARGEND
  IF (depth EQ NULL) THEN_DO depth = 32;
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
  this_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, depth, rmask, gmask, bmask, amask);
  besRETURN_LONG(this_surface);
besEND

besFUNCTION(gfx_FreeSurface)
  DIM AS SDL_Surface PTR this_surface;
  besARGUMENTS("[i]")
    AT this_surface
  besARGEND
  IF (this_surface EQ 0 OR this_surface EQ NULL) THEN_DO this_surface = screen;
  SDL_FreeSurface(this_surface);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_BlitSurface)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS SDL_Surface PTR src;
  DIM AS SDL_Rect srcrect;
  DIM AS SDL_Surface PTR dst;
  DIM AS SDL_Rect dstrect;
  DIM AS int i, srflag, rtnval;
  IF (besARGNR < 4) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= besARGNR STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      src = LONGVALUE(Argument);
    END_IF
    IF (i EQ 2) THEN
      IF (TYPE(Argument) EQ VTYPE_LONG) THEN_DO srflag = 1;
      IF (TYPE(Argument) EQ VTYPE_ARRAY) THEN
        tmpArg = ARRAYVALUE(Argument, 0);
        besDEREFERENCE(tmpArg);
        srcrect.x = (Sint16)LONGVALUE(tmpArg);
        tmpArg = ARRAYVALUE(Argument, 1);
        besDEREFERENCE(tmpArg);
        srcrect.y = (Sint16)LONGVALUE(tmpArg);
        tmpArg = ARRAYVALUE(Argument, 2);
        besDEREFERENCE(tmpArg);
        srcrect.w = (Uint16)LONGVALUE(tmpArg);
        tmpArg = ARRAYVALUE(Argument, 3);
        besDEREFERENCE(tmpArg);
        srcrect.h = (Uint16)LONGVALUE(tmpArg);
      END_IF
    END_IF
    IF (i EQ 3) THEN
      dst = LONGVALUE(Argument);
      IF (dst EQ 0 OR dst EQ NULL) THEN_DO dst = screen;
    END_IF
    IF (i EQ 4) THEN
      IF (TYPE(Argument) EQ VTYPE_ARRAY) THEN
        tmpArg = ARRAYVALUE(Argument, 0);
        besDEREFERENCE(tmpArg);
        dstrect.x = (Sint16)LONGVALUE(tmpArg);
        tmpArg = ARRAYVALUE(Argument, 1);
        besDEREFERENCE(tmpArg);
        dstrect.y = (Sint16)LONGVALUE(tmpArg);
        dstrect.w = NULL;
        dstrect.h = NULL;
      END_IF
    END_IF
  NEXT
  IF (srflag EQ 1) THEN
    rtnval = SDL_BlitSurface(src, NULL, dst, AT dstrect);
  ELSE
    rtnval = SDL_BlitSurface(src, AT srcrect, dst, AT dstrect);
  END_IF
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_LoadBMP)
  DIM AS char PTR file_name;
  DIM AS SDL_Surface PTR image;
  besARGUMENTS("z")
    AT file_name
  besARGEND
  image = SDL_LoadBMP(file_name);
  besRETURN_LONG(image);
besEND

besFUNCTION(gfx_GetPixel)
  DIM AS SDL_Surface PTR this_surface;
  DIM AS int x, y;
  DIM AS Uint32 PTR pixels;
  besARGUMENTS("iii")
    AT this_surface, AT x, AT y
  besARGEND
  pixels = (Uint32 *)this_surface->pixels;
  besRETURN_LONG(pixels[(y * this_surface->w) + x]);
besEND

besFUNCTION(gfx_GetPixelRGBA)
  DIM AS SDL_Surface PTR this_surface;
  DIM AS Uint32 PTR pixels;
  DIM AS int x, y;
  DIM AS Uint8 r, g, b, a;
  besARGUMENTS("iii")
    AT this_surface, AT x, AT y
  besARGEND
  pixels = (Uint32 *)this_surface->pixels;
  SDL_GetRGBA(pixels[(y * this_surface->w) + x], this_surface->format, &r, &g, &b, &a);
  printf("r = %i, g = %i, b = %i, a = %i\n", r, g, b, a);  
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_Mandelbrot)
  DIM AS double cx, cy, zx, zy, tp;
  DIM AS int iter;
  besARGUMENTS("rri")
    AT cx, AT cy, AT iter
  besARGEND
  DEF_WHILE (zx * zx + zy * zy < 4 AND iter > 0)
  BEGIN_WHILE
    tp = zx * zx - zy * zy + cx;
    zy = 2 * zx * zy + cy;
    zx = tp;
    iter = iter - 1;
  WEND
  besRETURN_LONG(iter);
besEND

/*****************
 SDL_gfx Functions
*****************/

/* Pixel */

besFUNCTION(gfx_pixelColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 4) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 4 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = pixelColor(this_surface, x, y, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_pixelRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = pixelRGBA(this_surface, x, y, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Horizontal line */

besFUNCTION(gfx_hlineColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, x2, y;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = hlineColor(this_surface, x1, x2, y, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_hlineRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, x2, y;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = hlineRGBA(this_surface, x1, x2, y, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Vertical line */

besFUNCTION(gfx_vlineColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y1, y2;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = vlineColor(this_surface, x, y1, y2, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_vlineRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y1, y2;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = vlineRGBA(this_surface, x, y1, y2, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Rectangle */

besFUNCTION(gfx_rectangleColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = rectangleColor(this_surface, x1, y1, x2, y2, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_rectangleRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = rectangleRGBA(this_surface, x1, y1, x2, y2, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Rounded-Corner Rectangle */

besFUNCTION(gfx_roundedRectangleColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, rad;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = roundedRectangleColor(this_surface, x1, y1, x2, y2, rad, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_roundedRectangleRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, rad;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = roundedRectangleRGBA(this_surface, x1, y1, x2, y2, rad, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Filled rectangle (Box) */

besFUNCTION(gfx_boxColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = boxColor(this_surface, x1, y1, x2, y2, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_boxRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = boxRGBA(this_surface, x1, y1, x2, y2, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Rounded-Corner Filled rectangle (Box) */

besFUNCTION(gfx_roundedBoxColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, rad;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = roundedBoxColor(this_surface, x1, y1, x2, y2, rad, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_roundedBoxRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, rad;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = roundedBoxRGBA(this_surface, x1, y1, x2, y2, rad, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Line */

besFUNCTION(gfx_lineColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = lineColor(this_surface, x1, y1, x2, y2, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_lineRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = lineRGBA(this_surface, x1, y1, x2, y2, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* AA Line */

besFUNCTION(gfx_aalineColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = aalineColor(this_surface, x1, y1, x2, y2, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_aalineRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = aalineRGBA(this_surface, x1, y1, x2, y2, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Thick Line */

besFUNCTION(gfx_thickLineColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 width;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO width  = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = thickLineColor(this_surface, x1, y1, x2, y2, width, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_thickLineRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2;
  DIM AS Uint8 w, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO w  = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = thickLineRGBA(this_surface, x1, y1, x2, y2, w, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Circle */

besFUNCTION(gfx_circleColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = circleColor(this_surface, x, y, rad, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_circleRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = circleRGBA(this_surface, x, y, rad, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Arc */

besFUNCTION(gfx_arcColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = arcColor(this_surface, x, y, rad, start, end, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_arcRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = arcRGBA(this_surface, x, y, rad, start, end, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* AA Circle */

besFUNCTION(gfx_aacircleColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = aacircleColor(this_surface, x, y, rad, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_aacircleRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = aacircleRGBA(this_surface, x, y, rad, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Filled Circle */

besFUNCTION(gfx_filledCircleColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = filledCircleColor(this_surface, x, y, rad, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_filledCircleRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = filledCircleRGBA(this_surface, x, y, rad, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Ellipse */

besFUNCTION(gfx_ellipseColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = ellipseColor(this_surface, x, y, rx, ry, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_ellipseRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = ellipseRGBA(this_surface, x, y, rx, ry, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* AA Ellipse */

besFUNCTION(gfx_aaellipseColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = aaellipseColor(this_surface, x, y, rx, ry, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_aaellipseRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = aaellipseRGBA(this_surface, x, y, rx, ry, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

 /* Filled Ellipse */

besFUNCTION(gfx_filledEllipseColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = filledEllipseColor(this_surface, x, y, rx, ry, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_filledEllipseRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rx, ry;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO ry = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = filledEllipseRGBA(this_surface, x, y, rx, ry, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Pie */

besFUNCTION(gfx_pieColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = pieColor(this_surface, x, y, rad, start, end, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_pieRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = pieRGBA(this_surface, x, y, rad, start, end, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Filled Pie */

besFUNCTION(gfx_filledPieColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = filledPieColor(this_surface, x, y, rad, start, end, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_filledPieRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y, rad, start, end;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 10) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 10 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO rad = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO start = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO end = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = filledPieRGBA(this_surface, x, y, rad, start, end, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Trigon */

besFUNCTION(gfx_trigonColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = trigonColor(this_surface, x1, y1, x2, y2, x3, y3, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_trigonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 11) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 11 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 11) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = trigonRGBA(this_surface, x1, y1, x2, y2, x3, y3, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* AA-Trigon */

besFUNCTION(gfx_aatrigonColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = aatrigonColor(this_surface, x1, y1, x2, y2, x3, y3, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_aatrigonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 11) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 11 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 11) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = aatrigonRGBA(this_surface, x1, y1, x2, y2, x3, y3, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Filled Trigon */

besFUNCTION(gfx_filledTrigonColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = filledTrigonColor(this_surface, x1, y1, x2, y2, x3, y3, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_filledTrigonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x1, y1, x2, y2, x3, y3;
  DIM AS Uint8 width, r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 11) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 11 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y1 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO x2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO y2 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO x3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO y3 = (Sint16)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 10) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 11) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = filledTrigonRGBA(this_surface, x1, y1, x2, y2, x3, y3, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Polygon */

besFUNCTION(gfx_polygonColor)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint32 color;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = polygonColor(this_surface, vx, vy, n, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_polygonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint8 r, g, b, a;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = polygonRGBA(this_surface, AT vx, AT vy, n, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

  /* AA-Polygon */

besFUNCTION(gfx_aapolygonColor)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint32 color;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = aapolygonColor(this_surface, AT vx, AT vy, n, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_aapolygonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint8 r, g, b, a;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = aapolygonRGBA(this_surface, AT vx, AT vy, n, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Filled Polygon */

besFUNCTION(gfx_filledPolygonColor)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint32 color;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = filledPolygonColor(this_surface, AT vx, AT vy, n, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_filledPolygonRGBA)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint8 r, g, b, a;
  DIM AS int n, i, j, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = filledPolygonRGBA(this_surface, AT vx, AT vy, n, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_texturedPolygon)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint32 color;
  DIM AS SDL_Surface *texture;
  DIM AS char *s;
  DIM AS int n, i, j, texture_dx, texture_dy, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 7) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 7 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO s = STRINGVALUE(Argument);
    IF (i EQ 6) THEN_DO texture_dx = (int)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO texture_dy = (int)LONGVALUE(Argument);
  NEXT
  texture = SDL_LoadBMP(s);
  rtnval = texturedPolygon(this_surface, AT vx, AT vy, n, texture, texture_dx, texture_dy);
  besRETURN_LONG(rtnval);
besEND

/* Bezier */

besFUNCTION(gfx_bezierColor)
  DIM AS VARIABLE Argument;
  DIM AS const Sint16 PTR vx, PTR vy;
  DIM AS Uint32 color;
  DIM AS int n, i, s, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO vx = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO vy = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO s = (int)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = bezierColor(this_surface, vx, vy, n, s, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_bezierRGBA)
  DIM AS VARIABLE Argument;
  DIM AS VARIABLE tmpArg;
  DIM AS Sint16 vx[64], vy[64];
  DIM AS Uint8 r, g, b, a;
  DIM AS int n, i, j, s, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 9) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 9 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vx[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 3) THEN
      DEF_FOR (j = 0 TO j <= ARRAYHIGH(Argument) STEP INCR j)
      BEGIN_FOR
        tmpArg = ARRAYVALUE(Argument, j);
        besDEREFERENCE(tmpArg);
        vy[j] = (Sint16)LONGVALUE(tmpArg);
      NEXT
    END_IF
    IF (i EQ 4) THEN_DO n = (int)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO s = (int)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 9) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = bezierRGBA(this_surface, AT vx, AT vy, n, s, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

/* Characters/Strings */

besFUNCTION(gfx_SetFont)
  DIM AS VARIABLE Argument;
  DIM AS char *fontdata;
  DIM AS Uint32 cw, ch;
  DIM AS int i;
  IF (besARGNR < 3) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 3 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO fontdata = STRINGVALUE(Argument);
    IF (i EQ 2) THEN_DO cw = (Uint32)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO ch = (Uint32)LONGVALUE(Argument);
  NEXT
  gfxPrimitivesSetFont(fontdata, cw, ch);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_FontRotation)
  DIM AS VARIABLE Argument;
  DIM AS Uint32 rotation;
  IF (besARGNR < 1) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  Argument = besARGUMENT(1);
  besDEREFERENCE(Argument);
  rotation = (Uint32)LONGVALUE(Argument);
  gfxPrimitivesSetFontRotation(rotation);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(gfx_characterColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS char *c;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO c = STRINGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = characterColor(this_surface, x, y, c, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_characterRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS char *c;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO c = STRINGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = characterRGBA(this_surface, x, y, c, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_stringColor)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS char *s;
  DIM AS Uint32 color;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO s = STRINGVALUE(Argument);
    IF (i EQ 5) THEN_DO color = (Uint32)LONGVALUE(Argument);
  NEXT
  rtnval = stringColor(this_surface, x, y, s, color);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_stringRGBA)
  DIM AS VARIABLE Argument;
  DIM AS Sint16 x, y;
  DIM AS char *s;
  DIM AS Uint8 r, g, b, a;
  DIM AS int i, rtnval;
  DIM AS long this_surface;
  IF (besARGNR < 8) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 8 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN
      this_surface = LONGVALUE(Argument);
      IF (this_surface EQ 0) THEN_DO this_surface = screen;
    END_IF
    IF (i EQ 2) THEN_DO x = (Sint16)LONGVALUE(Argument);
    IF (i EQ 3) THEN_DO y = (Sint16)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO s = (char)PTR STRINGVALUE(Argument);
    IF (i EQ 5) THEN_DO r = (Uint8)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO g = (Uint8)LONGVALUE(Argument);
    IF (i EQ 7) THEN_DO b = (Uint8)LONGVALUE(Argument);
    IF (i EQ 8) THEN_DO a = (Uint8)LONGVALUE(Argument);
  NEXT
  rtnval = stringRGBA(this_surface, x, y, s, r, g, b, a);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_SDL_initFramerate)
  DIM AS int rate;
  SDL_initFramerate(AT fpsm);
  rate = SDL_getFramerate(AT fpsm);
  besRETURN_LONG(rate);
besEND

besFUNCTION(gfx_SDL_getFramerate)
  DIM AS int rate;
  rate = SDL_getFramerate(AT fpsm);
  besRETURN_LONG(rate);
besEND

besFUNCTION(gfx_SDL_setFramerate)
  DIM AS VARIABLE Argument;
  DIM AS Uint32 rate;
  DIM AS int rtnval;
  IF (besARGNR < 1) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  Argument = besARGUMENT(1);
  besDEREFERENCE(Argument);
  rate = (Uint32)LONGVALUE(Argument);
  rtnval = SDL_setFramerate(AT fpsm, rate);
  besRETURN_LONG(rtnval);
besEND

besFUNCTION(gfx_SDL_framerateDelay)
  DIM AS Uint32 time_passed;
  SDL_initFramerate(AT fpsm);
  time_passed = SDL_framerateDelay(AT fpsm);
  besRETURN_LONG(time_passed);
besEND
