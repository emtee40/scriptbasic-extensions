/*
** This file is the SDL graphic portion of the Brandy Basic V Interpreter.
** Copyright (C) 2000, 2001, 2002, 2003, 2004 David Daniels
**
** SDL additions by Colin Tuckley
** libbbc / C BASIC fork by John Spikowski
** BBC_SHIFT & BBC_ROTATE by Charles Pegge
** BBC_MOUSE direction by vovchik

gcc -shared -fPIC -o libbbc.so libbbc.c -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -DHAVE_OPENGL -lSDL -lm

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL.h>
#include "libbbc.h"
#include "cbasic.h"


SUB error(int errcode)
BEGIN_SUB
  PRINT ("ERROR = %i\n", errcode);
  _CLOSE();
END_SUB

SUB trace_edge(int32 x1, int32 y1, int32 x2, int32 y2)
BEGIN_SUB
  DIM AS int32 dx, dy, xf, yf, a, b, t, i;
  IF (x1 EQ x2 AND y1 EQ y2) THEN_DO EXIT_SUB;
  IF (x2 > x1) THEN
    dx = x2 - x1;
    xf = 1;
  ELSE
    dx = x1 - x2;
    xf = -1;
  END_IF
  IF (y2 > y1) THEN
    dy = y2 - y1;
    yf = 1;
  ELSE
    dy = y1 - y2;
    yf = -1;
  END_IF
  IF (dx > dy) THEN
    a = dy + dy;
    t = a - dx;
    b = t - dx;
    DEF_FOR (i = 0 TO i <= dx STEP INCR i)
    BEGIN_FOR
      IF (x1 < left[y1]) THEN_DO left[y1] = x1;
      IF (x1 > right[y1]) THEN_DO right[y1] = x1;
      x1 += xf;
      IF (t < 0) THEN
        t += a;
      ELSE
        t += b;
        y1 += yf;
      END_IF
    NEXT
  ELSE
    a = dx + dx;
    t = a - dy;
    b = t - dy;
    DEF_FOR (i = 0 TO i <= dy STEP INCR i)
    BEGIN_FOR
      IF (x1 < left[y1]) THEN_DO left[y1] = x1;
      IF (x1 > right[y1]) THEN_DO right[y1] = x1;
      y1 += yf;
      IF (t < 0) THEN
        t += a;
      ELSE
        t += b;
        x1 += xf;
      END_IF
    NEXT
  END_IF
END_SUB

SUB draw_h_line(SDL_Surface PTR sr, int32 sw, int32 sh, int32 x1, int32 y, int32 x2, Uint32 col)
BEGIN_SUB
  DIM AS int32 tt, i;
  IF (x1 > x2) THEN
    tt = x1;
    x1 = x2;
    x2 = tt;
  END_IF
  IF ( y >= 0 AND y < sh ) THEN
    IF (x1 < 0) THEN_DO x1 = 0;
    IF (x1 >= sw) THEN_DO x1 = sw-1;
    IF (x2 < 0) THEN_DO x2 = 0;
    IF (x2 >= sw) THEN_DO x2 = sw-1;
    DEF_FOR (i = x1 TO i <= x2 STEP INCR i)
    BEGIN_FOR
      PTR ((Uint32 PTR)sr->pixels + i + y * sw) = col;
    NEXT
  END_IF
END_SUB

SUB buff_convex_poly(SDL_Surface PTR sr, int32 sw, int32 sh, int32 n, int32  PTR x, int32 PTR y, Uint32 col)
BEGIN_SUB
  DIM AS int32 i, iy;
  DIM AS int32 low = MAX_YRES, high = 0;
  DEF_FOR (i = 0 TO i < n STEP INCR i)
  BEGIN_FOR
    IF (y[i] > high) THEN_DO high = y[i];
    IF (y[i] < low) THEN_DO low = y[i];
  NEXT
  DEF_FOR (iy = low TO iy <= high STEP INCR iy)
  BEGIN_FOR
    left[iy] = MAX_XRES + 1;
    right[iy] = - 1;
  NEXT
  trace_edge(x[n - 1], y[n - 1], x[0], y[0]);
  DEF_FOR (i = 0 TO i < n - 1 STEP INCR i)
  BEGIN_FOR
    trace_edge(x[i], y[i], x[i + 1], y[i + 1]);
  NEXT
  DEF_FOR (iy = low TO iy <= high STEP INCR iy)
  BEGIN_FOR
    draw_h_line(sr, sw, sh, left[iy], iy, right[iy], col);
  NEXT
END_SUB

SUB draw_line(SDL_Surface PTR sr, int32 sw, int32 sh, int32 x1, int32 y1, int32 x2, int32 y2, Uint32 col)
BEGIN_SUB
  DIM AS int d, x, y, ax, ay, sx, sy, dx, dy, tt;
  IF (x1 > x2) THEN
    tt = x1;
    x1 = x2;
    x2 = tt;
    tt = y1;
    y1 = y2;
    y2 = tt;
  END_IF
  dx = x2 - x1;
  ax = ABS(dx) << 1;
  sx = ((dx < 0) ? -1 : 1);
  dy = y2 - y1;
  ay = ABS(dy) << 1;
  sy = ((dy < 0) ? -1 : 1);
  x = x1;
  y = y1;
  IF (ax > ay) THEN
    d = ay - (ax >> 1);
    DEF_WHILE (x NE x2)
    BEGIN_WHILE
      IF ((x >= 0) AND (x < sw) AND (y >= 0) AND (y < sh)) THEN_DO PTR ((Uint32 PTR)sr->pixels + x + y * sw) = col;
      IF (d >= 0) THEN
        y += sy;
        d -= ax;
      END_IF
      x += sx;
      d += ay;
    WEND
  ELSE
    d = ax - (ay >> 1);
    DEF_WHILE (y NE y2)
    BEGIN_WHILE
      IF ((x >= 0) AND (x < sw) AND (y >= 0) AND (y < sh)) THEN_DO PTR ((Uint32 PTR)sr->pixels + x + y * sw) = col;
      IF (d >= 0) THEN
        x += sx;
        d -= ay;
      END_IF
      y += sy;
      d += ax;
    WEND
  END_IF
  IF ((x >= 0) AND (x < sw) AND (y >= 0) AND (y < sh)) THEN_DO PTR ((Uint32 PTR)sr->pixels + x + y * sw) = col;
END_SUB

SUB filled_triangle(SDL_Surface PTR sr, int32 sw, int32 sh, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, Uint32 col)
BEGIN_SUB
  DIM AS int x[3], y[3];
  x[0]=x1;
  x[1]=x2;
  x[2]=x3;
  y[0]=y1;
  y[1]=y2;
  y[2]=y3;
  buff_convex_poly(sr, sw, sh, 3, x, y, col);
END_SUB

SUB draw_ellipse(SDL_Surface PTR sr, int32 sw, int32 sh, int32 x0, int32 y0, int32 a, int32 b, Uint32 c)
BEGIN_SUB
  DIM AS int32 x, y, y1, aa, bb, d, g, h;
  DIM AS Uint32 PTR dest;
  aa = a * a;
  bb = b * b;
  h = (FAST_4_DIV(aa)) - b * aa + bb;
  g = (FAST_4_DIV(9 * aa)) - (FAST_3_MUL(b * aa)) + bb;
  x = 0;
  y = b;
  DEF_WHILE (g < 0)
  BEGIN_WHILE
    IF (((y0 - y) >= 0) AND ((y0 - y) < sh)) THEN
      dest = ((Uint32 PTR)sr->pixels + x0 + (y0 - y) * sw);
      IF (((x0 - x) >= 0) AND ((x0 - x) < sw)) THEN_DO PTR (dest - x) = c;
      IF (((x0 + x) >= 0) AND ((x0 + x) < sw)) THEN_DO PTR (dest + x) = c;
    END_IF
    IF (((y0 + y) >= 0) AND ((y0 + y) < sh)) THEN
      dest = ((Uint32 PTR)sr->pixels + x0 + (y0 + y) * sw);
      IF (((x0 - x) >= 0) AND ((x0 - x) < sw)) THEN_DO PTR (dest - x) = c;
      IF (((x0 + x) >= 0) AND ((x0 + x) < sw)) THEN_DO PTR (dest + x) = c;
    END_IF
    IF (h < 0) THEN
      d = ((FAST_2_MUL(x)) + 3) * bb;
      g += d;
    ELSE
      d = ((FAST_2_MUL(x)) + 3) * bb - FAST_2_MUL((y - 1) * aa);
      g += (d + (FAST_2_MUL(aa)));
      INCR y;
    END_IF
    h += d;
    INCR x;
  WEND
  y1 = y;
  h = (FAST_4_DIV(bb)) - a * bb + aa;
  x = a;
  y = 0;
  DEF_WHILE (y <= y1)
  BEGIN_WHILE
    IF (((y0 - y) >= 0) AND ((y0 - y) < sh)) THEN
      dest = ((Uint32 PTR)sr->pixels + x0 + (y0 - y) * sw);
      IF (((x0 - x) >= 0) AND ((x0 - x) < sw)) THEN_DO PTR (dest - x) = c;
      IF (((x0 + x) >= 0) AND ((x0 + x) < sw)) THEN_DO PTR (dest + x) = c;
    END_IF
    IF (((y0 + y) >= 0) AND ((y0 + y) < sh)) THEN
      dest = ((Uint32 PTR)sr->pixels + x0 + (y0 + y) * sw);
      IF (((x0 - x) >= 0) AND ((x0 - x) < sw)) THEN_DO PTR (dest - x) = c;
      IF (((x0 + x) >= 0) AND ((x0 + x) < sw)) THEN_DO PTR (dest + x) = c;
    END_IF
    IF (h < 0) THEN
      h += ((FAST_2_MUL(y)) + 3) * aa;
    ELSE
      h += (((FAST_2_MUL(y) + 3) * aa) - (FAST_2_MUL(x - 1) * bb));
      INCR x;
    END_IF
    INCR y;
  WEND
END_SUB

SUB filled_ellipse(SDL_Surface PTR sr, int32 sw, int32 sh, int32 x0, int32 y0, int32 a, int32 b, Uint32 c)
BEGIN_SUB
  DIM AS int32 x, y, y1, aa, bb, d, g, h;
  aa = a * a;
  bb = b * b;
  h = (FAST_4_DIV(aa)) - b * aa + bb;
  g = (FAST_4_DIV(9 * aa)) - (FAST_3_MUL(b * aa)) + bb;
  x = 0;
  y = b;
  DEF_WHILE (g < 0)
  BEGIN_WHILE
    draw_h_line(sr, sw, sh, x0 - x, y0 + y, x0 + x, c);
    draw_h_line(sr, sw, sh, x0 - x, y0 - y, x0 + x, c);
    IF (h < 0) THEN
      d = ((FAST_2_MUL(x)) + 3) * bb;
      g += d;
    ELSE
      d = ((FAST_2_MUL(x)) + 3) * bb - FAST_2_MUL((y - 1) * aa);
      g += (d + (FAST_2_MUL(aa)));
      DECR y;
    END_IF
    h += d;
    INCR x;
  WEND
  y1 = y;
  h = (FAST_4_DIV(bb)) - a * bb + aa;
  x = a;
  y = 0;
  DEF_WHILE (y <= y1)
  BEGIN_WHILE
    draw_h_line(sr, sw, sh, x0 - x, y0 + y, x0 + x, c);
    draw_h_line(sr, sw, sh, x0 - x, y0 - y, x0 + x, c);
    IF (h < 0) THEN
      h += ((FAST_2_MUL(y)) + 3) * aa;
    ELSE
      h += (((FAST_2_MUL(y) + 3) * aa) - (FAST_2_MUL(x - 1) * bb));
      DECR x;
    END_IF
    INCR y;
  WEND
END_SUB
SUB find_cursor()
BEGIN_SUB
END_SUB

SUB set_rgb()
BEGIN_SUB
  DIM AS int j;
  j = text_physforecol * 3;
  tf_colour = SDL_MapRGB(sdl_fontbuf->format, palette[j], palette[j + 1], palette[j + 2]);
  j = text_physbackcol * 3;
  tb_colour = SDL_MapRGB(sdl_fontbuf->format, palette[j], palette[j + 1], palette[j + 2]);
  j = graph_physforecol * 3;
  gf_colour = SDL_MapRGB(sdl_fontbuf->format, palette[j], palette[j + 1], palette[j + 2]);
  j = graph_physbackcol * 3;
  gb_colour = SDL_MapRGB(sdl_fontbuf->format, palette[j], palette[j + 1], palette[j + 2]);
END_SUB

SUB sdlchar(char ch)
BEGIN_SUB
  DIM AS int32 y, line;
  IF (cursorstate EQ ONSCREEN) THEN_DO cursorstate = SUSPENDED;
  place_rect.x = xtext * XPPC;
  place_rect.y = ytext * YPPC;
  SDL_FillRect(sdl_fontbuf, NULL, tb_colour);
  DEF_FOR (y = 0 TO y < YPPC STEP INCR y)
  BEGIN_FOR
    line = sysfont[ch-' '][y];
    IF (line NE 0) THEN
      IF (line & 0x80) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 0 + y * XPPC) = tf_colour;
      IF (line & 0x40) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 1 + y * XPPC) = tf_colour;
      IF (line & 0x20) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 2 + y * XPPC) = tf_colour;
      IF (line & 0x10) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 3 + y * XPPC) = tf_colour;
      IF (line & 0x08) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 4 + y * XPPC) = tf_colour;
      IF (line & 0x04) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 5 + y * XPPC) = tf_colour;
      IF (line & 0x02) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 6 + y * XPPC) = tf_colour;
      IF (line & 0x01) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 7 + y * XPPC) = tf_colour;
    END_IF
  NEXT
  SDL_BlitSurface(sdl_fontbuf, AT font_rect, screen0, AT place_rect);
  IF (echo) THEN_DO SDL_UpdateRect(screen0, xtext * XPPC, ytext * YPPC, XPPC, YPPC);
END_SUB

LOCAL SUB scroll_text(updown direction)
BEGIN_SUB
  DIM AS int n, xx, yy;
  IF (NOT textwin AND direction EQ SCROLL_UP) THEN
    scroll_rect.x = 0;
    scroll_rect.y = YPPC;
    scroll_rect.w = vscrwidth;
    scroll_rect.h = YPPC * textheight - 1;
    SDL_BlitSurface(screen0, AT scroll_rect, screen1, NULL);
    line_rect.x = 0;
    line_rect.y = YPPC * textheight - 1;
    line_rect.w = vscrwidth;
    line_rect.h = YPPC;
    SDL_FillRect(screen1, AT line_rect, tb_colour);
    SDL_BlitSurface(screen1, NULL, screen0, NULL);
    SDL_Flip(screen0);
  ELSE
    xx = xtext;
    yy = ytext;
    scroll_rect.x = XPPC * twinleft;
    scroll_rect.w = XPPC * (twinright - twinleft + 1);
    scroll_rect.h = YPPC * (twinbottom - twintop);
    line_rect.x = 0;
    IF (twintop NE twinbottom) THEN
      IF (direction EQ SCROLL_UP) THEN
        scroll_rect.y = YPPC * (twintop + 1);
        line_rect.y = 0;
      ELSE
        scroll_rect.y = YPPC * twintop;
        line_rect.y = YPPC;
      END_IF
      SDL_BlitSurface(screen0, AT scroll_rect, screen1, AT line_rect);
      scroll_rect.x = 0;
      scroll_rect.y = 0;
      scroll_rect.w = XPPC * (twinright - twinleft + 1);
      scroll_rect.h = YPPC * (twinbottom - twintop + 1);
      line_rect.x = twinleft * XPPC;
      line_rect.y = YPPC * twintop;
      SDL_BlitSurface(screen1, AT scroll_rect, screen0, AT line_rect);
    END_IF
    xtext = twinleft;
    echo_off();
    DEF_FOR (n = twinleft TO n <= twinright STEP INCR n)
    BEGIN_FOR
      sdlchar(' ');
    NEXT
    xtext = xx;
    ytext = yy;
    echo_on();
  END_IF
END_SUB

LOCAL SUB vdu_2317()
BEGIN_SUB
  DIM AS int32 temp;
  SELECT_CASE (vduqueue[1])
  BEGIN_SELECT
    CASE TINT_FORETEXT:
      text_foretint = (vduqueue[2] & TINTMASK) >> TINTSHIFT;
      IF (colourdepth EQ 256) THEN_DO text_physforecol = (text_forecol << COL256SHIFT) + text_foretint;
      END_CASE
    CASE TINT_BACKTEXT:
      text_backtint = (vduqueue[2] & TINTMASK) >> TINTSHIFT;
      IF (colourdepth EQ 256) THEN_DO text_physbackcol = (text_backcol << COL256SHIFT) + text_backtint;
      END_CASE
    CASE TINT_FOREGRAPH:
      graph_foretint = (vduqueue[2] & TINTMASK) >> TINTSHIFT;
      IF (colourdepth EQ 256) THEN_DO graph_physforecol = (graph_forecol << COL256SHIFT) + graph_foretint;
      END_CASE
    CASE TINT_BACKGRAPH:
      graph_backtint = (vduqueue[2] & TINTMASK) >> TINTSHIFT;
      IF (colourdepth EQ 256) THEN_DO graph_physbackcol = (graph_backcol << COL256SHIFT) + graph_backtint;
      END_CASE
    CASE EXCH_TEXTCOLS:
      temp = text_forecol;
      text_forecol = text_backcol;
      text_backcol = temp;
      temp = text_physforecol;
      text_physforecol = text_physbackcol;
      text_physbackcol = temp;
      temp = text_foretint;
      text_foretint = text_backtint;
      text_backtint = temp;
      END_CASE
    CASE_ELSE
      END_CASE
  END_SELECT
  set_rgb();
END_SUB

LOCAL SUB vdu_23command()
BEGIN_SUB
  DIM AS int codeval, n;
  SELECT_CASE (vduqueue[0])
  BEGIN_SELECT
    CASE 1:
      IF (graphmode EQ FULLSCREEN) THEN
        IF (vduqueue[1] EQ 0) THEN
          IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
          cursorstate = HIDDEN;
        END_IF
        IF (vduqueue[1] EQ 1 AND cursorstate NE NOCURSOR) THEN_DO cursorstate = ONSCREEN;
      END_IF
      IF (vduqueue[1] EQ 1) THEN
        cursorstate = ONSCREEN;
      ELSE
        cursorstate = HIDDEN;
      END_IF
      END_CASE
    CASE 8:
      END_CASE
    CASE 17:
      vdu_2317();
      END_CASE
    CASE_ELSE
      codeval = vduqueue[0] & 0x00FF;
      IF (codeval < 32 ) THEN_DO END_CASE
      DEF_FOR (n = 0 TO n < 8 STEP INCR n)
      BEGIN_FOR
        sysfont[codeval - 32][n] = vduqueue[n + 1];
      NEXT
  END_SELECT
END_SUB

SUB toggle_cursor()
BEGIN_SUB
  DIM AS int32 left, right, top, bottom, x, y;
  IF ((cursorstate NE SUSPENDED) AND (cursorstate NE ONSCREEN)) THEN_DO EXIT_SUB
  IF (cursorstate EQ ONSCREEN) THEN
    cursorstate = SUSPENDED;
  ELSE
    cursorstate = ONSCREEN;
  END_IF
  left = xoffset + xtext * xscale * XPPC;
  right = left + xscale * XPPC -1;
  IF (cursmode EQ UNDERLINE) THEN
    y = (yoffset + (ytext+1) * yscale * YPPC - yscale) * vscrwidth;
    DEF_FOR (x = left TO x <= right STEP  INCR x)
    BEGIN_FOR
      PTR ((Uint32 PTR)screen0->pixels + x + y) ^= xor_mask;
      IF (yscale NE 1) THEN_DO PTR ((Uint32 PTR)screen0->pixels + x + y + vscrwidth) ^= xor_mask;
    NEXT
  ELSE_IF (cursmode EQ BLOCK) THEN
    top = yoffset + ytext * yscale * YPPC;
    bottom = top + YPPC * yscale -1;
    DEF_FOR (y = top TO y <= bottom STEP INCR y)
    BEGIN_FOR
      DEF_FOR (x = left TO x <= right STEP INCR x)
      BEGIN_FOR
        PTR ((Uint32 PTR)screen0->pixels + x + y * vscrwidth) ^= xor_mask;
      NEXT
    NEXT
  END_IF
  IF (echo) THEN_DO SDL_UpdateRect(screen0, xoffset + xtext * xscale * XPPC, yoffset + ytext * yscale * YPPC, xscale * XPPC, yscale * YPPC);
END_SUB

LOCAL SUB toggle_tcursor()
BEGIN_SUB
  DIM AS int32 x, y, top, bottom, left, right;
  IF (cursorstate EQ ONSCREEN) THEN
    cursorstate = SUSPENDED;
  ELSE
    cursorstate = ONSCREEN;
  END_IF
  left = xtext * XPPC;
  right = left + XPPC -1;
  IF (cursmode EQ UNDERLINE) THEN
    y = ((ytext + 1) * YPPC - 1) * vscrwidth;
    DEF_FOR (x = left TO x <= right STEP  INCR x)
    BEGIN_FOR
      PTR ((Uint32 PTR)screen0->pixels + x + y) ^= xor_mask;
    NEXT
  ELSE_IF (cursmode EQ BLOCK) THEN
    top = ytext * YPPC;
    bottom = top + YPPC -1;
    DEF_FOR (y = top TO y <= bottom STEP INCR y)
    BEGIN_FOR
      DEF_FOR (x = left TO x <= right STEP INCR x)
      BEGIN_FOR
        PTR ((Uint32 PTR)screen0->pixels + x + y * vscrwidth) ^= xor_mask;
      NEXT
    NEXT
  END_IF
  IF (echo) THEN_DO SDL_UpdateRect(screen0, xtext * XPPC, ytext * YPPC, XPPC, YPPC);
END_SUB

LOCAL SUB blit_scaled(int32 left, int32 top, int32 right, int32 bottom)
BEGIN_SUB
  DIM AS int32 dleft, dtop, xx, yy, i, j, ii, jj;
  IF (left >= screenwidth OR right < 0 OR top >= screenheight OR bottom < 0) THEN_DO EXIT_SUB
  IF (left < 0) THEN_DO left = 0;
  IF (right >= screenwidth) THEN_DO right = screenwidth - 1;
  IF (top < 0) THEN_DO top = 0;
  IF (bottom >= screenheight) THEN_DO bottom = screenheight-1;
  dleft = left*xscale + xoffset;
  dtop  = top*yscale + yoffset;
  yy = dtop;
  DEF_FOR (j = top TO j <= bottom STEP INCR j)
  BEGIN_FOR
    DEF_FOR (jj = 1 TO jj <= yscale STEP INCR jj)
    BEGIN_FOR
      xx = dleft;
      DEF_FOR (i = left TO i <= right STEP INCR i)
      BEGIN_FOR
        DEF_FOR (ii = 1 TO ii <= xscale STEP  INCR ii)
        BEGIN_FOR
          PTR ((Uint32 PTR)screen0->pixels + xx + yy * vscrwidth) = PTR ((Uint32 PTR)modescreen->pixels + i + j * vscrwidth);
          INCR xx;
        NEXT
      NEXT
      INCR yy;
    NEXT
  NEXT
  scale_rect.x = dleft;
  scale_rect.y = dtop;
  scale_rect.w = (right + 1 - left) * xscale;
  scale_rect.h = (bottom + 1 - top) * yscale;
  SDL_UpdateRect(screen0, scale_rect.x, scale_rect.y, scale_rect.w, scale_rect.h);
END_SUB

LOCAL SUB init_palette()
BEGIN_SUB
  SELECT_CASE (colourdepth)
  BEGIN_SELECT
  CASE 2:
    palette[0] = palette[1] = palette[2] = 0;
    palette[3] = palette[4] = palette[5] = 255;
    END_CASE
  CASE 4:
    palette[0] = palette[1] = palette[2] = 0;   /* Black */
    palette[3] = 255; palette[4] = palette[5] = 0;  /* Red */
    palette[6] = palette[7] = 255; palette[8] = 0;  /* Yellow */
    palette[9] = palette[10] = palette[11] = 255; /* White */
    break;
  CASE 16:
    palette[0] = palette[1] = palette[2] = 0;       /* Black */
    palette[3] = 255; palette[4] = palette[5] = 0;      /* Red */
    palette[6] = 0; palette[7] = 255; palette[8] = 0; /* Green */
    palette[9] = palette[10] = 255; palette[11] = 0;  /* Yellow */
    palette[12] = palette[13] = 0; palette[14] = 255; /* Blue */
    palette[15] = 255; palette[16] = 0; palette[17] = 255;  /* Magenta */
    palette[18] = 0; palette[19] = palette[20] = 255; /* Cyan */
    palette[21] = palette[22] = palette[23] = 255;      /* White */
    palette[24] = palette[25] = palette[26] = 0;      /* Black */
    palette[27] = 160; palette[28] = palette[29] = 0; /* Dark red */
    palette[30] = 0; palette[31] = 160; palette[32] = 0;/* Dark green */
    palette[33] = palette[34] = 160; palette[35] = 0; /* Khaki */
    palette[36] = palette[37] = 0; palette[38] = 160; /* Navy blue */
    palette[39] = 160; palette[40] = 0; palette[41] = 160;  /* Purple */
    palette[42] = 0; palette[43] = palette[44] = 160; /* Cyan */
    palette[45] = palette[46] = palette[47] = 160;      /* Grey */
    break;
  CASE 256: {
    DIM AS int red, green, blue, tint, colour;
    colour = 0;
    DEF_FOR (blue = 0 TO blue <= COLOURSTEP * 3 STEP blue += COLOURSTEP)
    BEGIN_FOR
      DEF_FOR (green = 0 TO green <= COLOURSTEP * 3 STEP green += COLOURSTEP)
      BEGIN_FOR
        DEF_FOR (red = 0 TO  red <= COLOURSTEP * 3 STEP red += COLOURSTEP)
        BEGIN_FOR
          DEF_FOR (tint = 0 TO tint <= TINTSTEP * 3 STEP tint += TINTSTEP)
          BEGIN_FOR
            palette[colour] = red + tint;
            palette[colour + 1] = green + tint;
            palette[colour + 2] = blue + tint;
            colour += 3;
          NEXT
        NEXT
      NEXT
    NEXT
    END_CASE
  }
  CASE_ELSE
    error(ERR_UNSUPPORTED);
  END_SELECT
  IF (colourdepth EQ 256) THEN
    text_physforecol = (text_forecol<<COL256SHIFT) + text_foretint;
    text_physbackcol = (text_backcol<<COL256SHIFT) + text_backtint;
    graph_physforecol = (graph_forecol<<COL256SHIFT) + graph_foretint;
    graph_physbackcol = (graph_backcol<<COL256SHIFT) + graph_backtint;
  ELSE
    text_physforecol = text_forecol;
    text_physbackcol = text_backcol;
    graph_physforecol = graph_forecol;
    graph_physbackcol = graph_backcol;
  END_IF
  set_rgb();
END_SUB

LOCAL SUB change_palette(int32 colour, int32 red, int32 green, int32 blue)
BEGIN_SUB
  IF (graphmode NE FULLSCREEN) THEN_DO EXIT_SUB
  palette[colour * 3] = red;
  palette[colour * 3 + 1] = green;
  palette[colour * 3 + 2] = blue;
END_SUB

FUNCTION int32 _COLOURFN(int32 red, int32 green, int32 blue)
BEGIN_FUNCTION
  DIM AS int32 n, distance, test, best, dr, dg, db;
  IF (graphmode < TEXTMODE) THEN
    RETURN_FUNCTION(colourdepth - 1);
  ELSE_IF (graphmode EQ TEXTMODE) THEN
    switch_graphics();
  END_IF
  distance = 0x7fffffff;
  best = 0;
  DEF_FOR (n = 0 TO n < colourdepth AND distance NE 0 STEP INCR n)
  BEGIN_FOR
    dr = palette[n * 3] - red;
    dg = palette[n * 3 + 1] - green;
    db = palette[n * 3 + 2] - blue;
    test = 2 * dr * dr + 4 * dg * dg + db * db;
    IF (test < distance) THEN
      distance = test;
      best = n;
    END_IF
  NEXT
  RETURN_FUNCTION(best);
END_FUNCTION

LOCAL SUB set_text_colour(boolean background, int colnum)
BEGIN_SUB
  IF (background) THEN
    text_physbackcol = text_backcol = (colnum & (colourdepth - 1));
  ELSE
    text_physforecol = text_forecol = (colnum & (colourdepth - 1));
  END_IF
  set_rgb();
END_SUB

LOCAL SUB set_graphics_colour(boolean background, int colnum)
BEGIN_SUB
  IF (background) THEN
    graph_physbackcol = graph_backcol = (colnum & (colourdepth - 1));
  ELSE
    graph_physforecol = graph_forecol = (colnum & (colourdepth - 1));
  END_IF
  set_rgb();
END_SUB

LOCAL SUB switch_graphics()
BEGIN_SUB
  SDL_SetClipRect(screen0, NULL);
  SDL_SetClipRect(modescreen, NULL);
  SDL_FillRect(screen0, NULL, tb_colour);
  SDL_FillRect(screen1, NULL, tb_colour);
  SDL_FillRect(modescreen, NULL, tb_colour);
  init_palette();
  graphmode = FULLSCREEN;
  xtext = twinleft;
  ytext = twintop;
  IF (xoffset NE 0) THEN
    line_rect.x = xoffset - 1;
    line_rect.y = yoffset - 1;
    line_rect.w = vscrwidth;
    line_rect.h = vscrheight;
    SDL_SetClipRect(screen0, AT line_rect);
  END_IF
  vdu_cleartext();
  IF (cursorstate EQ NOCURSOR) THEN
    cursorstate = SUSPENDED;
    toggle_cursor();
  END_IF
END_SUB

LOCAL SUB switch_text()
BEGIN_SUB
  SDL_SetClipRect(screen0, NULL);
  SDL_SetClipRect(modescreen, NULL);
  SDL_FillRect(screen0, NULL, tb_colour);
  SDL_FillRect(screen1, NULL, tb_colour);
  SDL_FillRect(modescreen, NULL, tb_colour);
END_SUB

LOCAL SUB scroll(updown direction)
BEGIN_SUB
  DIM AS int left, right, top, bottom, dest, topwin;
  topwin = ybufoffset + twintop * YPPC;
  IF (direction EQ SCROLL_UP) THEN
    dest = ybufoffset + twintop * YPPC;
    left = xbufoffset + twinleft * XPPC;
    right = xbufoffset + twinright * XPPC + XPPC - 1;
    top = dest + YPPC;
    bottom = ybufoffset + twinbottom * YPPC + YPPC - 1;
    scroll_rect.x = xbufoffset + twinleft * XPPC;
    scroll_rect.y = ybufoffset + YPPC * (twintop + 1);
    scroll_rect.w = XPPC * (twinright - twinleft +1);
    scroll_rect.h = YPPC * (twinbottom - twintop);
    SDL_BlitSurface(modescreen, AT scroll_rect, screen1, NULL);
    line_rect.x = 0;
    line_rect.y = YPPC * (twinbottom - twintop);
    line_rect.w = XPPC * (twinright - twinleft + 1);
    line_rect.h = YPPC;
    SDL_FillRect(screen1, AT line_rect, tb_colour);
  ELSE
    dest = ybufoffset + (twintop + 1) * YPPC;
    left = xbufoffset + twinleft * XPPC;
    right = xbufoffset + (twinright + 1) * XPPC - 1;
    top = ybufoffset + twintop * YPPC;
    bottom = ybufoffset + twinbottom * YPPC - 1;
    scroll_rect.x = left;
    scroll_rect.y = top;
    scroll_rect.w = XPPC * (twinright - twinleft + 1);
    scroll_rect.h = YPPC * (twinbottom - twintop);
    line_rect.x = 0;
    line_rect.y = YPPC;
    SDL_BlitSurface(modescreen, AT scroll_rect, screen1, AT line_rect);
    line_rect.x = 0;
    line_rect.y = 0;
    line_rect.w = XPPC * (twinright - twinleft + 1);
    line_rect.h = YPPC;
    SDL_FillRect(screen1, &line_rect, tb_colour);
  END_IF
  line_rect.x = 0;
  line_rect.y = 0;
  line_rect.w = XPPC * (twinright - twinleft + 1);
  line_rect.h = YPPC * (twinbottom - twintop + 1);
  scroll_rect.x = left;
  scroll_rect.y = dest;
  SDL_BlitSurface(screen1, AT line_rect, modescreen, AT scroll_rect);
  IF (scaled) THEN
    blit_scaled(left, topwin, right, twinbottom * YPPC + YPPC - 1);
  ELSE
    SDL_BlitSurface(screen1, AT line_rect, screen0, AT scroll_rect);
    SDL_Flip(screen0);
  END_IF
END_SUB

LOCAL SUB echo_ttext()
BEGIN_SUB
  IF (xtext NE 0) THEN_DO SDL_UpdateRect(screen0, 0, ytext * YPPC, xtext * XPPC, YPPC);
END_SUB

LOCAL SUB echo_text()
BEGIN_SUB
  DIM AS int sx, ex, sy, ey;
  IF (xtext EQ 0) THEN_DO EXIT_SUB
  IF (scaled) THEN
    blit_scaled(0, ytext * YPPC, xtext * XPPC - 1, ytext * YPPC + YPPC - 1);
  ELSE
    sx = xoffset;
    sy = yoffset + ytext * YPPC;
    ex = xoffset + xtext * XPPC - 1;
    ey = sy + YPPC - 1;
    line_rect.x = xoffset;
    line_rect.y = yoffset + ytext * YPPC;
    line_rect.w = xtext * XPPC;
    line_rect.h = YPPC;
    scroll_rect.x = xoffset;
    scroll_rect.y = yoffset + ytext * YPPC;
    SDL_BlitSurface(modescreen, AT line_rect, screen0, AT scroll_rect);
    SDL_UpdateRect(screen0, xoffset, yoffset + ytext * YPPC, xtext * XPPC, YPPC);
  END_IF
END_SUB

LOCAL SUB write_char(int32 ch)
BEGIN_SUB
  DIM AS int32 y, topx, topy, line;
  IF (cursorstate EQ ONSCREEN) THEN_DO cursorstate = SUSPENDED;
  topx = xbufoffset + xtext * XPPC;
  topy = ybufoffset + ytext * YPPC;
  place_rect.x = topx;
  place_rect.y = topy;
  SDL_FillRect(sdl_fontbuf, NULL, tb_colour);
  DEF_FOR (y = 0 TO y < YPPC STEP INCR y)
  BEGIN_FOR
    line = sysfont[ch-' '][y];
    IF (line NE 0) THEN
      IF (line & 0x80) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 0 + y * XPPC) = tf_colour;
      IF (line & 0x40) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 1 + y * XPPC) = tf_colour;
      IF (line & 0x20) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 2 + y * XPPC) = tf_colour;
      IF (line & 0x10) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 3 + y * XPPC) = tf_colour;
      IF (line & 0x08) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 4 + y * XPPC) = tf_colour;
      IF (line & 0x04) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 5 + y * XPPC) = tf_colour;
      IF (line & 0x02) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 6 + y * XPPC) = tf_colour;
      IF (line & 0x01) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 7 + y * XPPC) = tf_colour;
    END_IF
  NEXT
  SDL_BlitSurface(sdl_fontbuf, &font_rect, modescreen, &place_rect);
  IF (echo) THEN
    IF (NOT scaled) THEN
      SDL_BlitSurface(sdl_fontbuf, AT font_rect, screen0,  AT place_rect);
      SDL_UpdateRect(screen0, place_rect.x, place_rect.y, XPPC, YPPC);
    ELSE
      blit_scaled(topx, topy, topx + XPPC - 1, topy + YPPC - 1);
    END_IF
  END_IF
  INCR xtext;
  IF (xtext > twinright) THEN
    IF (NOT echo) THEN_DO echo_text();
    xtext = twinleft;
    INCR ytext;
    IF (ytext > twinbottom) THEN
      scroll(SCROLL_UP);
      DECR ytext;
    END_IF
  END_IF
END_SUB

LOCAL SUB plot_char(int32 ch)
BEGIN_SUB
  DIM AS int32 y, topx, topy, line;
  topx = GXTOPX(xlast);
  topy = GYTOPY(ylast);
  place_rect.x = topx;
  place_rect.y = topy;
  SDL_FillRect(sdl_fontbuf, NULL, gb_colour);
  DEF_FOR (y = 0 TO y < YPPC STEP INCR y)
  BEGIN_FOR
    line = sysfont[ch-' '][y];
    IF (line NE 0) THEN
      IF (line & 0x80) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 0 + y * XPPC) = gf_colour;
      IF (line & 0x40) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 1 + y * XPPC) = gf_colour;
      IF (line & 0x20) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 2 + y * XPPC) = gf_colour;
      IF (line & 0x10) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 3 + y * XPPC) = gf_colour;
      IF (line & 0x08) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 4 + y * XPPC) = gf_colour;
      IF (line & 0x04) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 5 + y * XPPC) = gf_colour;
      IF (line & 0x02) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 6 + y * XPPC) = gf_colour;
      IF (line & 0x01) THEN_DO PTR ((Uint32 PTR)sdl_fontbuf->pixels + 7 + y * XPPC) = gf_colour;
    END_IF
  NEXT
  SDL_BlitSurface(sdl_fontbuf, AT font_rect, modescreen, AT place_rect);
  IF (NOT scaled) THEN
    SDL_BlitSurface(sdl_fontbuf, AT font_rect, screen0, AT place_rect);
    SDL_UpdateRect(screen0, place_rect.x, place_rect.y, XPPC, YPPC);
  ELSE
    blit_scaled(topx, topy, topx + XPPC - 1, topy + YPPC - 1);
  END_IF
  cursorstate = SUSPENDED;
  xlast += XPPC * xgupp;
  IF (xlast > gwinright) THEN
    xlast = gwinleft;
    ylast -= YPPC * ygupp;
    IF (ylast < gwinbottom) THEN_DO ylast = gwintop;
  END_IF
END_SUB

SUB echo_on()
BEGIN_SUB
  echo = TRUE;
  IF (graphmode EQ FULLSCREEN) THEN
    echo_text();
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    echo_ttext();
  END_IF
END_SUB

SUB echo_off()
BEGIN_SUB
  echo = FALSE;
  IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
  END_IF
END_SUB

LOCAL SUB move_cursor(int32 column, int32 row)
BEGIN_SUB
  IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    xtext = column;
    ytext = row;
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    toggle_tcursor();
    xtext = column;
    ytext = row;
  END_IF
END_SUB

SUB set_cursor(boolean underline)
BEGIN_SUB
  IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    cursmode = underline ? UNDERLINE : BLOCK;
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_tcursor();
    cursmode = underline ? UNDERLINE : BLOCK;
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_tcursor();
  END_IF
END_SUB

LOCAL SUB vdu_setpalette()
BEGIN_SUB
  DIM AS int32 logcol, mode;
  logcol = vduqueue[0] & colourmask;
  mode = vduqueue[1];
  IF (mode < 16 AND colourdepth <= 16) THEN
    logtophys[logcol] = mode;
  ELSE_IF (mode EQ 16) THEN
    change_palette(logcol, vduqueue[2], vduqueue[3], vduqueue[4]);
  ELSE
    error(ERR_UNSUPPORTED);
  END_IF
END_SUB

LOCAL SUB move_down()
BEGIN_SUB
  INCR ytext;
  IF (ytext > twinbottom) THEN
    DECR ytext;
    scroll(SCROLL_UP);
  END_IF
END_SUB

LOCAL SUB move_up()
BEGIN_SUB
  DECR ytext;
  IF (ytext < twintop) THEN
    INCR ytext;
    scroll(SCROLL_DOWN);
  END_IF
END_SUB

LOCAL SUB move_curback()
BEGIN_SUB
  IF (vdu5mode) THEN
    xlast -= XPPC * xgupp;
    IF (xlast < gwinleft) THEN
      xlast = gwinright - XPPC * xgupp + 1;
      ylast += YPPC * ygupp;
      IF (ylast > gwintop) THEN
        ylast = gwinbottom + YPPC * ygupp - 1;
      END_IF
    END_IF
  ELSE_IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    DECR xtext;
    IF (xtext < twinleft) THEN
      xtext = twinright;
      move_up();
    END_IF
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    toggle_tcursor();
    DECR xtext;
    IF (xtext < twinleft) THEN
      xtext = twinright;
      DECR ytext;
      IF (ytext < twintop) THEN
        INCR ytext;
        scroll_text(SCROLL_DOWN);
      END_IF
    END_IF
    toggle_tcursor();
  END_IF
END_SUB

LOCAL SUB move_curforward()
BEGIN_SUB
  IF (vdu5mode) THEN
    xlast += XPPC * xgupp;
    IF (xlast > gwinright) THEN
      xlast = gwinleft;
      ylast -= YPPC * ygupp;
      IF (ylast < gwinbottom) THEN_DO ylast = gwintop;
    END_IF
  ELSE_IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    INCR xtext;
    IF (xtext > twinright) THEN
      xtext = twinleft;
      move_down();
    END_IF
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    INCR xtext;
    IF (xtext > twinright) THEN
      INCR ytext;
      IF (ytext > twinbottom) THEN
        DECR ytext;
        scroll_text(SCROLL_UP);
      END_IF
    END_IF
  END_IF
END_SUB

LOCAL SUB move_curdown()
BEGIN_SUB
  IF (vdu5mode) THEN
    ylast -= YPPC * ygupp;
    IF (ylast < gwinbottom) THEN_DO ylast = gwintop;
  ELSE_IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    move_down();
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    INCR ytext;
    IF (ytext > twinbottom) THEN
      DECR ytext;
      scroll_text(SCROLL_UP);
    END_IF
  END_IF
END_SUB

LOCAL SUB move_curup()
BEGIN_SUB
  IF (vdu5mode) THEN
    ylast += YPPC * ygupp;
    IF (ylast > gwintop) THEN_DO ylast = gwinbottom + YPPC * ygupp - 1;
  ELSE_IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    move_up();
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    DECR ytext;
    IF (ytext < twintop) THEN
      INCR ytext;
      scroll_text(SCROLL_DOWN);
    END_IF
  END_IF
END_SUB

LOCAL SUB vdu_cleartext()
BEGIN_SUB
  DIM AS int32 left, right, top, bottom;
  IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    IF (scaled) THEN
      left = twinleft * XPPC;
      right = twinright * XPPC + XPPC - 1;
      top = twintop * YPPC;
      bottom = twinbottom * YPPC + YPPC - 1;
      SDL_FillRect(modescreen, NULL, tb_colour);
      blit_scaled(left, top, right, bottom);
      xtext = twinleft;
      ytext = twintop;
      IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
    ELSE
      IF (textwin) THEN
        left = xbufoffset + twinleft * XPPC;
        right = xbufoffset + twinright * XPPC + XPPC - 1;
        top = ybufoffset + twintop * YPPC;
        bottom = ybufoffset + twinbottom * YPPC + YPPC - 1;
        line_rect.x = left;
        line_rect.y = top;
        line_rect.w = right - left + 1;
        line_rect.h = bottom - top + 1;
        SDL_FillRect(modescreen, AT line_rect, tb_colour);
        SDL_FillRect(screen0, AT line_rect, tb_colour);
      ELSE
        SDL_FillRect(modescreen, NULL, tb_colour);
        SDL_FillRect(screen0, NULL, tb_colour);
      END_IF
      xtext = twinleft;
      ytext = twintop;
      IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
    END_IF
  ELSE_IF (textwin) THEN
    DIM AS int32 column, row;
    echo_off();
    DEF_FOR (row = twintop TO row <= twinbottom STEP INCR row)
    BEGIN_FOR
      xtext = twinleft;
      ytext = row;
      DEF_FOR (column = twinleft TO column <= twinright STEP INCR column)
      BEGIN_FOR
       sdlchar(' ');
      NEXT
    NEXT
    echo_on();
    xtext = twinleft;
    ytext = twintop;
  ELSE
    SDL_FillRect(screen0, NULL, tb_colour);
    xtext = twinleft;
    ytext = twintop;
  END_IF
  SDL_Flip(screen0);
END_SUB

LOCAL SUB vdu_return()
BEGIN_SUB
  IF (vdu5mode) THEN
    xlast = gwinleft;
  ELSE_IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    xtext = twinleft;
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    move_cursor(twinleft, ytext);
  END_IF
END_SUB

LOCAL SUB vdu_cleargraph()
BEGIN_SUB
  IF (graphmode EQ TEXTONLY) THEN_DO EXIT_SUB
  IF (graphmode EQ TEXTMODE) THEN_DO switch_graphics();
  IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
  SDL_FillRect(modescreen, NULL, gb_colour);
  IF (NOT scaled) THEN
    SDL_FillRect(screen0, NULL, gb_colour);
  ELSE
    blit_scaled(GXTOPX(gwinleft), GYTOPY(gwintop), GXTOPX(gwinright), GYTOPY(gwinbottom));
  END_IF
  IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  SDL_Flip(screen0);
END_SUB

LOCAL SUB vdu_textcol()
BEGIN_SUB
  DIM AS int32 colnumber;
  colnumber = vduqueue[0];
  IF (colnumber < 128) THEN
    IF (graphmode EQ FULLSCREEN) THEN
      IF (colourdepth EQ 256) THEN
        text_forecol = colnumber & COL256MASK;
        text_physforecol = (text_forecol << COL256SHIFT) + text_foretint;
      ELSE
        text_physforecol = text_forecol = colnumber & colourmask;
      END_IF
    ELSE
      text_physforecol = text_forecol = colnumber & colourmask;
    END_IF
  ELSE
    IF (graphmode EQ FULLSCREEN) THEN
      IF (colourdepth EQ 256) THEN
        text_backcol = colnumber & COL256MASK;
        text_physbackcol = (text_backcol << COL256SHIFT) + text_backtint;
      ELSE
        text_physbackcol = text_backcol = colnumber & colourmask;
      END_IF
    ELSE
      text_physbackcol = text_backcol = (colnumber - 128) & colourmask;
    END_IF
  END_IF
  set_rgb();
END_SUB

LOCAL SUB reset_colours()
BEGIN_SUB
  SELECT_CASE (colourdepth)
  BEGIN_SELECT
    CASE 2:
      logtophys[0] = VDU_BLACK;
      logtophys[1] = VDU_WHITE;
      text_forecol = graph_forecol = 1;
      END_CASE
    CASE 4:
      logtophys[0] = VDU_BLACK;
      logtophys[1] = VDU_RED;
      logtophys[2] = VDU_YELLOW;
      logtophys[3] = VDU_WHITE;
      text_forecol = graph_forecol = 3;
      END_CASE
    CASE 16:
      logtophys[0] = VDU_BLACK;
      logtophys[1] = VDU_RED;
      logtophys[2] = VDU_GREEN;
      logtophys[3] = VDU_YELLOW;
      logtophys[4] = VDU_BLUE;
      logtophys[5] = VDU_MAGENTA;
      logtophys[6] = VDU_CYAN;
      logtophys[7]  = VDU_WHITE;
      logtophys[8] = FLASH_BLAWHITE;
      logtophys[9] = FLASH_REDCYAN;
      logtophys[10] = FLASH_GREENMAG;
      logtophys[11] = FLASH_YELBLUE;
      logtophys[12] = FLASH_BLUEYEL;
      logtophys[13] = FLASH_MAGREEN;
      logtophys[14] = FLASH_CYANRED;
      logtophys[15]  = FLASH_WHITEBLA;
      text_forecol = graph_forecol = 7;
      END_CASE
    CASE 256:
      text_forecol = graph_forecol = 63;
      graph_foretint = text_foretint = MAXTINT;
      graph_backtint = text_backtint = 0;
      END_CASE
    CASE_ELSE
      error(ERR_UNSUPPORTED);
  END_SELECT
  IF (colourdepth EQ 256) THEN
    colourmask = COL256MASK;
  ELSE
    colourmask = colourdepth - 1;
  END_IF
  text_backcol = graph_backcol = 0;
  init_palette();
END_SUB

LOCAL SUB vdu_graphcol()
BEGIN_SUB
  DIM AS int32 colnumber;
  IF (graphmode EQ NOGRAPHICS) THEN_DO error(ERR_NOGRAPHICS);
  IF (vduqueue[0] NE OVERWRITE_POINT) THEN_DO error(ERR_UNSUPPORTED);
  colnumber = vduqueue[1];
  IF (colnumber < 128) THEN
    graph_fore_action = vduqueue[0];
    IF (colourdepth EQ 256) THEN
      graph_forecol = colnumber & COL256MASK;
      graph_physforecol = (graph_forecol<<COL256SHIFT) + graph_foretint;
    ELSE
      graph_physforecol = graph_forecol = colnumber & colourmask;
    END_IF
  ELSE
    graph_back_action = vduqueue[0];
    IF (colourdepth EQ 256) THEN
      graph_backcol = colnumber & COL256MASK;
      graph_physbackcol = (graph_backcol<<COL256SHIFT) + graph_backtint;
    ELSE
      graph_physbackcol = graph_backcol = colnumber & colourmask;
    END_IF
  END_IF
  set_rgb();
END_SUB

LOCAL SUB vdu_graphwind()
BEGIN_SUB
  DIM AS int32 left, right, top, bottom;
  IF (graphmode NE FULLSCREEN) THEN_DO EXIT_SUB
  left = vduqueue[0] + vduqueue[1] * 256;
  IF (left > 0x7FFF) THEN_DO left = -(0x10000 - left);
  bottom = vduqueue[2] + vduqueue[3] * 256;
  IF (bottom > 0x7FFF) THEN_DO bottom = -(0x10000 - bottom);
  right = vduqueue[4] + vduqueue[5] * 256;
  IF (right > 0x7FFF) THEN_DO right = -(0x10000 - right);
  top = vduqueue[6] + vduqueue[7] * 256;
  IF (top > 0x7FFF) THEN_DO top = -(0x10000 - top);
  left += xorigin;
  right += xorigin;
  top += yorigin;
  bottom += yorigin;
  IF (left > right) THEN
    int32 temp = left;
    left = right;
    right = temp;
  END_IF
  IF (bottom > top) THEN
    int32 temp = bottom;
    bottom = top;
    top = temp;
  END_IF
  IF (right < 0 OR top < 0 OR left >= xgraphunits OR bottom >= ygraphunits) THEN_DO EXIT_SUB
  gwinleft = left;
  gwinright = right;
  gwintop = top;
  gwinbottom = bottom;
  line_rect.x = GXTOPX(left);
  line_rect.y = GYTOPY(top);
  line_rect.w = right - left + 1;
  line_rect.h = bottom - top + 1;
  SDL_SetClipRect(modescreen, AT line_rect);
  clipping = TRUE;
END_SUB

LOCAL SUB flood_fill(int32 x, int y, int colour)
BEGIN_SUB
  DIM AS int32 sp, fillx[FILLSTACK], filly[FILLSTACK];
  DIM AS int32 left, right, top, bottom, lleft, lright, pwinleft, pwinright, pwintop, pwinbottom;
  DIM AS SDL_Rect plot_rect;
  DIM AS boolean above, below;
  pwinleft = GXTOPX(gwinleft);
  pwinright = GXTOPX(gwinright);
  pwintop = GYTOPY(gwintop);
  pwinbottom = GYTOPY(gwinbottom);
  IF (x < pwinleft OR x > pwinright OR y < pwintop OR y > pwinbottom OR PTR ((Uint32 PTR)modescreen->pixels + x + y * vscrwidth) NE gb_colour) THEN_DO EXIT_SUB
  left = right = x;
  top = bottom = y;
  sp = 0;
  fillx[sp] = x;
  filly[sp] = y;
  INCR sp;
  DO
    DECR sp;
    y = filly[sp];
    lleft = fillx[sp];
    lright = lleft + 1;
    IF (y < top) THEN_DO top = y;
    IF (y > bottom) THEN_DO bottom = y;
    above = below = FALSE;
    DEF_WHILE (lleft >= pwinleft AND PTR ((Uint32 PTR)modescreen->pixels + lleft + y * vscrwidth) EQ gb_colour)
    BEGIN_WHILE
      IF (y > pwintop) THEN
        IF (PTR ((Uint32 PTR)modescreen->pixels + lleft + (y - 1) * vscrwidth) NE gb_colour) THEN
          above = FALSE;
        ELSE_IF (NOT above) THEN
          above = TRUE;
          IF (sp EQ FILLSTACK) THEN_DO EXIT_SUB
          fillx[sp] = lleft;
          filly[sp] = y - 1;
          INCR sp;
        END_IF
      END_IF
      IF (y < pwinbottom) THEN
        IF (PTR ((Uint32 PTR)modescreen->pixels + lleft + (y + 1) * vscrwidth) NE gb_colour) THEN
          below = FALSE;
        ELSE_IF (NOT below) THEN
          below = TRUE;
          IF (sp EQ FILLSTACK) THEN_DO EXIT_SUB
          fillx[sp] = lleft;
          filly[sp] = y + 1;
          INCR sp;
        END_IF
      END_IF
      DECR lleft;
    WEND
    INCR lleft;
    above = below = FALSE;
    DEF_WHILE (lright <= pwinright AND PTR ((Uint32 PTR)modescreen->pixels + lright + y * vscrwidth) EQ gb_colour)
    BEGIN_WHILE
      IF (y > pwintop) THEN
        IF (PTR ((Uint32 PTR)modescreen->pixels + lright + (y - 1) * vscrwidth) NE gb_colour) THEN
          above = FALSE;
        ELSE_IF (NOT above) THEN
          above = TRUE;
          IF (sp EQ FILLSTACK) THEN_DO EXIT_SUB
          fillx[sp] = lright;
          filly[sp] = y - 1;
          INCR sp;
        END_IF
      END_IF
      IF (y < pwinbottom) THEN
        IF (PTR ((Uint32 PTR)modescreen->pixels + lright + (y + 1) * vscrwidth) NE gb_colour) THEN
          below = FALSE;
        ELSE_IF (NOT below) THEN
          below = TRUE;
          IF (sp EQ FILLSTACK) THEN_DO EXIT_SUB
          fillx[sp] = lright;
          filly[sp] = y + 1;
          INCR sp;
        END_IF
      END_IF
      INCR lright;
    WEND
    DECR lright;
    draw_line(modescreen, vscrwidth, vscrheight, lleft, y, lright, y, colour);
    IF (lleft < left) THEN_DO left = lleft;
    IF (lright > right) THEN_DO right = lright;
  WHILE (sp != 0);
  IF (NOT scaled) THEN
    plot_rect.x = left;
    plot_rect.y = top;
    plot_rect.w = right - left + 1;
    plot_rect.h = bottom - top + 1;
    SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
  ELSE
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    blit_scaled(left, top, right, bottom);
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  END_IF
END_SUB

SUB _PLOT(int32 code, int32 x, int32 y)
BEGIN_SUB
  DIM AS int32 xlast3, ylast3, sx, sy, ex, ey;
  DIM AS Uint32 colour = 0;
  DIM AS SDL_Rect plot_rect, temp_rect;
  IF (graphmode EQ TEXTONLY) THEN_DO EXIT_SUB
  IF (graphmode EQ TEXTMODE) THEN_DO switch_graphics();
  xlast3 = xlast2;
  ylast3 = ylast2;
  xlast2 = xlast;
  ylast2 = ylast;
  IF ((code & ABSCOORD_MASK) NE 0) THEN
    xlast = x + xorigin;
    ylast = y + yorigin;
  ELSE
    xlast += x;
    ylast += y;
  END_IF
  IF ((code & PLOT_COLMASK) EQ PLOT_MOVEONLY) THEN_DO EXIT_SUB
  sx = GXTOPX(xlast2);
  sy = GYTOPY(ylast2);
  ex = GXTOPX(xlast);
  ey = GYTOPY(ylast);
  IF ((code & GRAPHOP_MASK) NE SHIFT_RECTANGLE) THEN
    SELECT_CASE (code & PLOT_COLMASK)
    BEGIN_SELECT
      CASE PLOT_FOREGROUND:
        colour = gf_colour;
        END_CASE
      CASE PLOT_INVERSE:
        error(ERR_UNSUPPORTED);
        END_CASE
      CASE PLOT_BACKGROUND:
        colour = gb_colour;
    END_SELECT
  END_IF
  SELECT_CASE (code & GRAPHOP_MASK)
  BEGIN_SELECT
    CASE DRAW_SOLIDLINE: {
      DIM AS int32 top, left;
      left = sx;
      top = sy;
      IF (ex < sx) THEN_DO left = ex;
      IF (ey < sy) THEN_DO top = ey;
      draw_line(modescreen, vscrwidth, vscrheight, sx, sy, ex, ey, colour);
      IF (NOT scaled) THEN
        plot_rect.x = left;
        plot_rect.y = top;
        plot_rect.w = sx + ex - 2 * left + 1;
        plot_rect.h = sy + ey - 2 * top + 1;
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(left, top, sx + ex - left, sy + ey - top);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE PLOT_POINT:
      IF (NOT scaled) THEN
        plot_rect.x = ex;
        plot_rect.y = ey;
        plot_rect.w = 1;
        plot_rect.h = 1;
        PTR ((Uint32 PTR)screen0->pixels + ex + ey * vscrwidth) = colour;
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        PTR ((Uint32 PTR)modescreen->pixels + ex + ey * vscrwidth) = colour;
        blit_scaled(ex, ey, ex, ey);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    CASE FILL_TRIANGLE: {
      DIM AS int32 left, right, top, bottom;
      filled_triangle(modescreen, vscrwidth, vscrheight, GXTOPX(xlast3), GYTOPY(ylast3), sx, sy, ex, ey, colour);
      left = right = xlast3;
      top = bottom = ylast3;
      IF (xlast2 < left) THEN_DO left = xlast2;
      IF (xlast < left) THEN_DO left = xlast;
      IF (xlast2 > right) THEN_DO right = xlast2;
      IF (xlast > right) THEN_DO right = xlast;
      IF (ylast2 > top) THEN_DO top = ylast2;
      IF (ylast > top) THEN_DO top = ylast;
      IF (ylast2 < bottom) THEN_DO bottom = ylast2;
      IF (ylast < bottom) THEN_DO bottom = ylast;
      IF (NOT scaled) THEN
        plot_rect.x = GXTOPX(left);
        plot_rect.y = GYTOPY(top);
        plot_rect.w = GXTOPX(right) - GXTOPX(left) + 1;
        plot_rect.h = GYTOPY(bottom) - GYTOPY(top) + 1;
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(GXTOPX(left), GYTOPY(top), GXTOPX(right), GYTOPY(bottom));
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE FILL_RECTANGLE: {
      DIM AS int32 left, right, top, bottom;
      left = sx;
      top = sy;
      IF (ex < sx) THEN_DO left = ex;
      IF (ey < sy) THEN_DO top = ey;
      right = sx + ex - left;
      bottom = sy + ey - top;
      plot_rect.x = left;
      plot_rect.y = top;
      plot_rect.w = right - left + 1;
      plot_rect.h = bottom - top + 1;
      SDL_FillRect(modescreen, AT plot_rect, colour);
      IF (NOT scaled) THEN
        SDL_FillRect(screen0, AT plot_rect, colour);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(left, top, right, bottom);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE FILL_PARALLELOGRAM: {
      DIM AS int32 vx, vy, left, right, top, bottom;
      filled_triangle(modescreen, vscrwidth, vscrheight, GXTOPX(xlast3), GYTOPY(ylast3), sx, sy, ex, ey, colour);
      vx = xlast3-xlast2+xlast;
      vy = ylast3-ylast2+ylast;
      filled_triangle(modescreen, vscrwidth, vscrheight, ex, ey, GXTOPX(vx), GYTOPY(vy), GXTOPX(xlast3), GYTOPY(ylast3), colour);
      left = right = xlast3;
      top = bottom = ylast3;
      IF (xlast2 < left) THEN_DO left = xlast2;
      IF (xlast < left) THEN_DO left = xlast;
      IF (vx < left) THEN_DO left = vx;
      IF (xlast2 > right) THEN_DO right = xlast2;
      IF (xlast > right) THEN_DO right = xlast;
      IF (vx > right) THEN_DO right = vx;
      IF (ylast2 > top) THEN_DO top = ylast2;
      IF (ylast > top) THEN_DO top = ylast;
      IF (vy > top) THEN_DO top = vy;
      IF (ylast2 < bottom) THEN_DO bottom = ylast2;
      IF (ylast < bottom) THEN_DO bottom = ylast;
      IF (vy < bottom) THEN_DO bottom = vy;
      IF (NOT scaled) THEN
        plot_rect.x = GXTOPX(left);
        plot_rect.y = GYTOPY(top);
        plot_rect.w = GXTOPX(right) - GXTOPX(left) + 1;
        plot_rect.h = GYTOPY(bottom) - GYTOPY(top) + 1;
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(GXTOPX(left), GYTOPY(top), GXTOPX(right), GYTOPY(bottom));
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE FLOOD_BACKGROUND:
      flood_fill(ex, ey, colour);
      END_CASE
    CASE PLOT_CIRCLE:
    CASE FILL_CIRCLE: {
      DIM AS int32 xradius, yradius;
      xradius = abs(xlast2 - xlast) / xgupp;
      yradius = abs(xlast2 - xlast) / ygupp;
      IF ((code & GRAPHOP_MASK) EQ PLOT_CIRCLE) THEN
        draw_ellipse(modescreen, vscrwidth, vscrheight, sx, sy, xradius, yradius, colour);
      ELSE
        filled_ellipse(modescreen, vscrwidth, vscrheight, sx, sy, xradius, yradius, colour);
      END_IF
      ex = sx - xradius;
      ey = sy - yradius;
      IF (NOT scaled) THEN
        plot_rect.x = ex;
        plot_rect.y = ey;
        plot_rect.w = 2 * xradius + 1;
        plot_rect.h = 2 * yradius + 1;
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(ex, ey, ex + 2 * xradius, ey + 2 * yradius);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE SHIFT_RECTANGLE: {
      DIM AS int32 destleft, destop, left, right, top, bottom;
      IF (xlast3 < xlast2) THEN
        left = GXTOPX(xlast3);
        right = GXTOPX(xlast2);
      ELSE
        left = GXTOPX(xlast2);
        right = GXTOPX(xlast3);
      END_IF
      IF (ylast3 > ylast2) THEN
        top = GYTOPY(ylast3);
        bottom = GYTOPY(ylast2);
      ELSE
        top = GYTOPY(ylast2);
        bottom = GYTOPY(ylast3);
      END_IF
      destleft = GXTOPX(xlast);
      destop = GYTOPY(ylast) - (bottom - top);
      plot_rect.x = destleft;
      plot_rect.y = destop;
      temp_rect.x = left;
      temp_rect.y = top;
      temp_rect.w = plot_rect.w = right - left + 1;
      temp_rect.h = plot_rect.h = bottom - top + 1;
      SDL_BlitSurface(modescreen, AT temp_rect, screen1, AT plot_rect);
      SDL_BlitSurface(screen1, AT plot_rect, modescreen, AT plot_rect);
      IF (NOT scaled) THEN
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(destleft, destop, destleft + (right - left), destop + (bottom - top));
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      IF (code EQ MOVE_RECTANGLE) THEN
        DIM AS int32 destright, destbot;
        destright = destleft + right - left;
        destbot = destop + bottom - top;
        IF (((destleft >= left AND destleft <= right) OR (destright >= left AND destright <= right)) AND ((destop >= top AND destop <= bottom) OR (destbot >= top AND destbot <= bottom))) THEN
          DIM AS int32 xdiff, ydiff;
          xdiff = left - destleft;
          ydiff = top - destop;
          IF (ydiff > 0) THEN
            IF (xdiff > 0) THEN
              plot_rect.x = destright + 1;
              plot_rect.y = top;
              plot_rect.w = right - (destright + 1) + 1;
              plot_rect.h = destbot - top + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            ELSE_IF (xdiff < 0) THEN
              plot_rect.x = left;
              plot_rect.y = top;
              plot_rect.w = (destleft - 1) - left + 1;
              plot_rect.h = destbot - top + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            END_IF
            plot_rect.x = left;
            plot_rect.y = destbot + 1;
            plot_rect.w = right - left + 1;
            plot_rect.h = bottom - (destbot + 1) + 1;
            SDL_FillRect(modescreen, AT plot_rect, gb_colour);
          ELSE_IF (ydiff EQ 0) THEN
            IF (xdiff > 0) THEN
              plot_rect.x = destright + 1;
              plot_rect.y = top;
              plot_rect.w = right - (destright + 1) + 1;
              plot_rect.h = bottom - top + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            ELSE_IF (xdiff < 0) THEN
              plot_rect.x = left;
              plot_rect.y = top;
              plot_rect.w = (destleft - 1) - left + 1;
              plot_rect.h = bottom - top + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            END_IF
          ELSE
            IF (xdiff > 0) THEN
              plot_rect.x = destright + 1;
              plot_rect.y = destop;
              plot_rect.w = right - (destright + 1) + 1;
              plot_rect.h = bottom - destop + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            ELSE_IF (xdiff < 0) THEN
              plot_rect.x = left;
              plot_rect.y = destop;
              plot_rect.w = (destleft - 1) - left + 1;
              plot_rect.h = bottom - destop + 1;
              SDL_FillRect(modescreen, AT plot_rect, gb_colour);
            END_IF
            plot_rect.x = left;
            plot_rect.y = top;
            plot_rect.w = right - left + 1;
            plot_rect.h = (destop - 1) - top + 1;
            SDL_FillRect(modescreen, AT plot_rect, gb_colour);
          END_IF
        ELSE
          plot_rect.x = left;
          plot_rect.y = top;
          plot_rect.w = right - left + 1;
          plot_rect.h = bottom - top + 1;
          SDL_FillRect(modescreen, AT plot_rect, gb_colour);
        END_IF
        IF (NOT scaled) THEN
          plot_rect.x = left;
          plot_rect.y = top;
          plot_rect.w = right - left + 1;
          plot_rect.h = bottom - top + 1;
          SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
        ELSE
          IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
          blit_scaled(left, top, right, bottom);
          IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
        END_IF
      END_IF
      END_CASE
    }
    CASE PLOT_ELLIPSE:
    CASE FILL_ELLIPSE: {
      DIM AS int32 semimajor, semiminor;
      semimajor = abs(xlast2 - xlast3) / xgupp;
      semiminor = abs(ylast - ylast3) / ygupp;
      sx = GXTOPX(xlast3);
      sy = GYTOPY(ylast3);
      IF ((code & GRAPHOP_MASK) EQ PLOT_ELLIPSE) THEN
        draw_ellipse(modescreen, vscrwidth, vscrheight, sx, sy, semimajor, semiminor, colour);
      ELSE
        filled_ellipse(modescreen, vscrwidth, vscrheight, sx, sy, semimajor, semiminor, colour);
      END_IF
      ex = sx - semimajor;
      ey = sy - semiminor;
      IF (NOT scaled) THEN
        plot_rect.x = ex;
        plot_rect.y = ey;
        plot_rect.w = 2 * semimajor;
        plot_rect.h = 2 * semiminor;
        SDL_BlitSurface(modescreen, AT plot_rect, screen0, AT plot_rect);
      ELSE
        IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
        blit_scaled(ex, ey, ex + 2 * semimajor, ey + 2 * semiminor);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      END_IF
      END_CASE
    }
    CASE_ELSE
      error(ERR_UNSUPPORTED);
  END_SELECT
  IF (NOT scaled) THEN_DO SDL_UpdateRect(screen0, plot_rect.x, plot_rect.y, plot_rect.w, plot_rect.h);
END_SUB

LOCAL SUB vdu_plot()
BEGIN_SUB
  DIM AS int32 x, y;
  x = vduqueue[1] + vduqueue[2] * 256;
  IF (x > 0x7FFF) THEN_DO x = -(0x10000 - x);
  y = vduqueue[3] + vduqueue[3] * 256;
  IF (y > 0x7FFF) THEN_DO y = -(0x10000 - y);
  _PLOT(vduqueue[0], x, y);
END_SUB

LOCAL SUB vdu_restwind()
BEGIN_SUB
  IF (clipping) THEN
    IF (scaled OR xoffset EQ 0) THEN
      SDL_SetClipRect(modescreen, NULL);
    ELSE
      line_rect.x = xoffset - 1;
      line_rect.y = yoffset - 1;
      line_rect.w = vscrwidth;
      line_rect.h = vscrheight;
      SDL_SetClipRect(screen0, AT line_rect);
    END_IF
    clipping = FALSE;
  END_IF
  xorigin = yorigin = 0;
  xlast = ylast = xlast2 = ylast2 = 0;
  gwinleft = 0;
  gwinright = xgraphunits - 1;
  gwintop = ygraphunits - 1;
  gwinbottom = 0;
  IF (graphmode EQ FULLSCREEN) THEN
    IF (cursorstate EQ ONSCREEN) THEN_DO toggle_cursor();
    xtext = ytext = 0;
    IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
  ELSE
    xtext = ytext = 0;
    move_cursor(0, 0);
  END_IF
  textwin = FALSE;
  twinleft = 0;
  twinright = textwidth - 1;
  twintop = 0;
  twinbottom = textheight - 1;
END_SUB

LOCAL SUB vdu_textwind()
BEGIN_SUB
  DIM AS int32 left, right, top, bottom;
  left = vduqueue[0];
  bottom = vduqueue[1];
  right = vduqueue[2];
  top = vduqueue[3];
  IF (left > right) THEN
    int32 temp = left;
    left = right;
    right = temp;
  END_IF
  IF (bottom < top) THEN
    int32 temp = bottom;
    bottom = top;
    top = temp;
  END_IF
  IF (left >= textwidth OR top >= textheight) THEN_DO EXIT_SUB
  twinleft = left;
  twinright = right;
  twintop = top;
  twinbottom = bottom;
  textwin = left > 0 OR right < textwidth - 1 OR top > 0 OR bottom < textheight - 1;
  move_cursor(twinleft, twintop);
END_SUB

LOCAL SUB vdu_origin()
BEGIN_SUB
  DIM AS int32 x, y;
  x = vduqueue[0] + vduqueue[1] * 256;
  y = vduqueue[2] + vduqueue[3] * 256;
  xorigin = x <= 32767 ? x : -(0x10000 - x);
  yorigin = y <= 32767 ? y : -(0x10000 - y);
END_SUB

LOCAL SUB vdu_hometext()
BEGIN_SUB
  IF (vdu5mode) THEN
    xlast = gwinleft;
    ylast = gwintop;
  ELSE
    move_cursor(twinleft, twintop);
  END_IF
END_SUB

LOCAL SUB vdu_movetext()
BEGIN_SUB
  DIM AS int32 column, row;
  IF (vdu5mode) THEN
    xlast = gwinleft + vduqueue[0] * XPPC * xgupp;
    ylast = gwintop - vduqueue[1] * YPPC * ygupp + 1;
  ELSE
    column = vduqueue[0] + twinleft;
    row = vduqueue[1] + twintop;
    IF (column > twinright OR row > twinbottom) THEN_DO EXIT_SUB
    move_cursor(column, row);
  END_IF
END_SUB

LOCAL SUB setup_mode(int32 mode)
BEGIN_SUB
  DIM AS int32 modecopy;
  modecopy = mode;
  mode = mode & MODEMASK;
  IF (mode > HIGHMODE) THEN_DO mode = modecopy = 0;
  IF (modetable[mode].xres > vscrwidth OR modetable[mode].yres > vscrheight) THEN_DO error(ERR_BADMODE);
  screenmode = modecopy;
  screenwidth = modetable[mode].xres;
  screenheight = modetable[mode].yres;
  xgraphunits = modetable[mode].xgraphunits;
  ygraphunits = modetable[mode].ygraphunits;
  colourdepth = modetable[mode].coldepth;
  textwidth = modetable[mode].xtext;
  textheight = modetable[mode].ytext;
  xscale = modetable[mode].xscale;
  yscale = modetable[mode].yscale;
  scaled = yscale NE 1 OR xscale NE 1;
  enable_vdu = TRUE;
  echo = TRUE;
  vdu5mode = FALSE;
  cursmode = UNDERLINE;
  cursorstate = NOCURSOR;
  clipping = FALSE;
  xoffset = (vscrwidth - screenwidth * xscale) / 2;
  yoffset = (vscrheight - screenheight * yscale) / 2;
  IF (scaled) THEN
    xbufoffset = ybufoffset = 0;
  ELSE
    xbufoffset = xoffset;
    ybufoffset = yoffset;
  END_IF
  IF (modetable[mode].graphics) THEN
    xgupp = xgraphunits / screenwidth;
    ygupp = ygraphunits / screenheight;
    xorigin = yorigin = 0;
    xlast = ylast = xlast2 = ylast2 = 0;
    gwinleft = 0;
    gwinright = xgraphunits-1;
    gwintop = ygraphunits-1;
    gwinbottom = 0;
  END_IF
  textwin = FALSE;
  twinleft = 0;
  twinright = textwidth - 1;
  twintop = 0;
  twinbottom = textheight - 1;
  xtext = ytext = 0;
  IF (graphmode EQ FULLSCREEN AND NOT modetable[mode].graphics) THEN
    switch_text();
    graphmode = TEXTONLY;
  END_IF
  IF (graphmode NE NOGRAPHICS AND graphmode NE FULLSCREEN) THEN
    IF (modetable[mode].graphics) THEN
      graphmode = TEXTMODE;
    ELSE
      graphmode = TEXTONLY;
    END_IF
  END_IF
  reset_colours();
  IF (graphmode EQ FULLSCREEN) THEN
    init_palette();
    IF (cursorstate EQ NOCURSOR) THEN_DO cursorstate = ONSCREEN;
    SDL_FillRect(screen0, NULL, tb_colour);
    SDL_FillRect(modescreen, NULL, tb_colour);
    IF (xoffset EQ 0) THEN
      SDL_SetClipRect(screen0, NULL);
    ELSE
      line_rect.x = xoffset;
      line_rect.y = yoffset;
      line_rect.w = vscrwidth;
      line_rect.h = vscrheight;
      SDL_SetClipRect(screen0, AT line_rect);
    END_IF
  END_IF
END_SUB

SUB _MODE(int32 mode)
BEGIN_SUB
  setup_mode(mode);
  SDL_FillRect(screen0, NULL, tb_colour);
  SDL_FillRect(modescreen, NULL, tb_colour);
  xtext = twinleft;
  ytext = twintop;
  SDL_Flip(screen0);
END_SUB

SUB _VDU(int32 charvalue)
BEGIN_SUB
  charvalue = charvalue & BYTEMASK;
  IF (vduneeded EQ 0) THEN
    IF (charvalue >= ' ') THEN
      IF (vdu5mode) THEN
        plot_char(charvalue);
      ELSE_IF (graphmode EQ FULLSCREEN) THEN
        write_char(charvalue);
        IF (cursorstate EQ SUSPENDED) THEN_DO toggle_cursor();
      ELSE
        sdlchar(charvalue);
        INCR xtext;
        IF (xtext > twinright) THEN
          xtext = twinleft;
          INCR ytext;
          IF (ytext > twinbottom) THEN
            DECR ytext;
            IF (textwin)THEN_DO scroll_text(SCROLL_UP);
          END_IF
        END_IF
        toggle_tcursor();
      END_IF
      EXIT_SUB
    ELSE
      IF (graphmode EQ FULLSCREEN) THEN
        IF (NOT echo) THEN_DO echo_text();
      ELSE
        IF (NOT echo) THEN_DO echo_ttext();
      END_IF
      vducmd = charvalue;
      vduneeded = vdubytes[charvalue];
      vdunext = 0;
    END_IF
  ELSE
    vduqueue[vdunext] = charvalue;
    vdunext++;
  END_IF
  IF (vdunext < vduneeded) THEN_DO EXIT_SUB;
  vduneeded = 0;
  SELECT_CASE (vducmd)
  BEGIN_SELECT
    CASE VDU_NULL:
      END_CASE
    CASE VDU_PRINT:
    CASE VDU_ENAPRINT:
    CASE VDU_DISPRINT:
      END_CASE
    CASE VDU_TEXTCURS:
      vdu5mode = FALSE;
      IF (cursorstate EQ HIDDEN) THEN
        cursorstate = SUSPENDED;
        toggle_cursor();
      END_IF
      END_CASE
    CASE VDU_GRAPHICURS:
      IF (graphmode EQ TEXTMODE) THEN_DO switch_graphics();
      IF (graphmode == FULLSCREEN) THEN
        vdu5mode = TRUE;
        toggle_cursor();
        cursorstate = HIDDEN;
      END_IF
      END_CASE
    CASE VDU_ENABLE:
      enable_vdu = TRUE;
      END_CASE
    CASE VDU_BEEP:
      putchar('\7');
      IF (echo) fflush(stdout);
      END_CASE
    CASE VDU_CURBACK:
      move_curback();
      END_CASE
    CASE VDU_CURFORWARD:
      move_curforward();
      END_CASE
    CASE VDU_CURDOWN:
      move_curdown();
      END_CASE
    CASE VDU_CURUP:
      move_curup();
      END_CASE
    CASE VDU_CLEARTEXT:
      IF (vdu5mode) THEN
        vdu_cleargraph();
        vdu_hometext();
      ELSE
        vdu_cleartext();
      END_IF
      END_CASE
    CASE VDU_RETURN:
      vdu_return();
      END_CASE
    CASE VDU_ENAPAGE:
    CASE VDU_DISPAGE:
      END_CASE
    CASE VDU_CLEARGRAPH:
      vdu_cleargraph();
      END_CASE
    CASE VDU_TEXTCOL:
      vdu_textcol();
      END_CASE
    CASE VDU_GRAPHCOL:
      vdu_graphcol();
      END_CASE
    CASE VDU_LOGCOL:
      vdu_setpalette();
      END_CASE
    CASE VDU_RESTCOL:
      reset_colours();
      END_CASE
    CASE VDU_DISABLE:
      END_CASE
    CASE VDU_SCRMODE:
      _MODE(vduqueue[0]);
      END_CASE
    CASE VDU_COMMAND:
      vdu_23command();
      END_CASE
    CASE VDU_DEFGRAPH:
      vdu_graphwind();
      END_CASE
    CASE VDU_PLOT:
      vdu_plot();
      END_CASE
    CASE VDU_RESTWIND:
      vdu_restwind();
      END_CASE
    CASE VDU_ESCAPE:
      END_CASE
    CASE VDU_DEFTEXT:
      vdu_textwind();
      END_CASE
    CASE VDU_ORIGIN:
      vdu_origin();
      END_CASE
    CASE VDU_HOMETEXT:
      vdu_hometext();
      END_CASE
    CASE VDU_MOVETEXT:
      vdu_movetext();
  END_SELECT
END_SUB

SUB _VDUSTR(char string[])
BEGIN_SUB
  DIM AS int32 n, length;
  length = strlen(string);
  echo_off();
  DEF_FOR (n = 0 TO n < length - 1 STEP INCR n)
  BEGIN_FOR
     _VDU(string[n]);
  NEXT
  echo_on();
  _VDU(string[length - 1]);
END_SUB

SUB _PRINTF(char *format, ...)
BEGIN_SUB
  DIM AS int32 length;
  DIM AS va_list parms;
  DIM AS char text [MAXSTRING];
  va_start(parms, format);
  length = vsprintf(text, format, parms);
  va_end(parms);
  _VDUSTR(text);
END_SUB

FUNCTION int32 _VDUFN(int variable)
BEGIN_FUNCTION
  SELECT_CASE (variable)
  BEGIN_SELECT
    CASE 0: /* ModeFlags */
      RETURN_FUNCTION(graphmode >= TEXTMODE ? 0 : 1);
    CASE 1: /* ScrRCol */
      RETURN_FUNCTION(textwidth - 1);
    CASE 2: /* ScrBRow */
      RETURN_FUNCTION(textheight - 1);
    CASE 3: /* NColour */
      RETURN_FUNCTION(colourdepth - 1);
    CASE 11: /* XWindLimit */
      RETURN_FUNCTION(screenwidth - 1);
    CASE 12: /* YWindLimit */
      RETURN_FUNCTION(screenheight - 1);
    CASE 128: /* GWLCol */
      RETURN_FUNCTION(gwinleft / xgupp);
    CASE 129: /* GWBRow */
      RETURN_FUNCTION(gwinbottom / ygupp);
    CASE 130: /* GWRCol */
      RETURN_FUNCTION(gwinright / xgupp);
    CASE 131: /* GWTRow */
      RETURN_FUNCTION(gwintop / ygupp);
    CASE 132: /* TWLCol */
      RETURN_FUNCTION(twinleft);
    CASE 133: /* TWBRow */
      RETURN_FUNCTION(twinbottom);
    CASE 134: /* TWRCol */
      RETURN_FUNCTION(twinright);
    CASE 135: /* TWTRow */
      RETURN_FUNCTION(twintop);
    CASE 136: /* OrgX */
      RETURN_FUNCTION(xorigin);
    CASE 137: /* OrgY */
      RETURN_FUNCTION(yorigin);
    CASE 153: /* GFCOL */
      RETURN_FUNCTION(graph_forecol);
    CASE 154: /* GBCOL */
      RETURN_FUNCTION(graph_backcol);
    CASE 155: /* TForeCol */
      RETURN_FUNCTION(text_forecol);
    CASE 156: /* TBackCol */
      RETURN_FUNCTION(text_backcol);
    CASE 157: /* GFTint */
      RETURN_FUNCTION(graph_foretint);
    CASE 158: /* GBTint */
      RETURN_FUNCTION(graph_backtint);
    CASE 159: /* TFTint */
      RETURN_FUNCTION(text_foretint);
    CASE 160: /* TBTint */
      RETURN_FUNCTION(text_backtint);
    CASE 161: /* MaxMode */
      RETURN_FUNCTION(HIGHMODE);
    CASE_ELSE
      RETURN_FUNCTION(0);
  END_SELECT
END_FUNCTION

FUNCTION int32 _POS()
BEGIN_FUNCTION
  RETURN_FUNCTION(xtext - twinleft);
END_FUNCTION

FUNCTION int32 _VPOS()
BEGIN_FUNCTION
  RETURN_FUNCTION(ytext - twintop);
END_FUNCTION

SUB _NEWMODE(int32 xres, int32 yres, int32 bpp, int32 rate)
BEGIN_SUB
  DIM AS int32 coldepth, n;
  IF (xres EQ 0 OR yres EQ 0 OR rate EQ 0 OR bpp EQ 0) THEN_DO error(ERR_BADMODE);
  SELECT_CASE (bpp)
  BEGIN_SELECT
    CASE 1:
       coldepth = 2;
       END_CASE
    CASE 2:
       coldepth = 4;
       END_CASE
    CASE 4:
       coldepth = 16;
       END_CASE
    CASE_ELSE
      coldepth = 256;
  END_SELECT
  DEF_FOR (n = 0 TO n <= HIGHMODE STEP INCR n)
  BEGIN_FOR
    IF (modetable[n].xres EQ xres AND modetable[n].yres EQ yres AND modetable[n].coldepth EQ coldepth) THEN_DO EXIT_SUB
  NEXT
  IF (n > HIGHMODE) THEN_DO error(ERR_BADMODE);
  _MODE(n);

END_SUB

SUB _MODESTR(int32 xres, int32 yres, int32 colours, int32 greys, int32 xeig, int32 yeig, int32 rate)
BEGIN_SUB
  DIM AS int32 coldepth, n;
  IF (xres EQ 0 OR yres EQ 0 OR rate EQ 0 OR (colours EQ 0 AND greys EQ 0)) THEN_DO error(ERR_BADMODE);
  coldepth = colours NE 0 ? colours : greys;
  DEF_FOR (n = 0 TO n <= HIGHMODE STEP INCR n)
  BEGIN_FOR
    IF (modetable[n].xres EQ xres AND modetable[n].yres EQ yres AND modetable[n].coldepth EQ coldepth) THEN_DO EXIT_SUB
  NEXT
  IF (n > HIGHMODE) THEN_DO error(ERR_BADMODE);
  _MODE(n);
  IF (colours EQ 0) THEN
    DIM AS int32 step, intensity;
    step = 255 / (greys - 1);
    intensity = 0;
    DEF_FOR (n = 0 TO n < greys STEP INCR n)
    BEGIN_FOR
      change_palette(n, intensity, intensity, intensity);
      intensity += step;
    NEXT
  END_IF
END_SUB

FUNCTION int32 _MODEFN()
BEGIN_FUNCTION
  RETURN_FUNCTION(screenmode);
END_FUNCTION

FUNCTION int32 _POINTFN(int32 x, int32 y)
BEGIN_FUNCTION
  DIM AS int32 colour;
  IF (graphmode EQ FULLSCREEN) THEN
    colour = PTR ((Uint32 PTR)modescreen->pixels + GXTOPX(x+xorigin) + GYTOPY(y + yorigin) * vscrwidth);
    IF (colourdepth EQ 256) THEN_DO colour = colour>>COL256SHIFT;
    RETURN_FUNCTION(colour);
  ELSE
    RETURN_FUNCTION(0);
  END_IF
END_FUNCTION

FUNCTION int32 _TINTFN(int32 x, int32 y)
BEGIN_FUNCTION
  IF (graphmode NE FULLSCREEN OR colourdepth NE 256) THEN_DO RETURN_FUNCTION(0);
  RETURN_FUNCTION(PTR ((Uint32 PTR)modescreen->pixels + GXTOPX(x + xorigin) + GYTOPY(y + yorigin) * vscrwidth) << TINTSHIFT);
END_FUNCTION

SUB _POINTTO(int32 x, int32 y)
BEGIN_SUB
  error(ERR_UNSUPPORTED);
END_SUB

SUB _WAIT()
BEGIN_SUB
  error(ERR_UNSUPPORTED);
END_SUB

SUB _TAB(int32 x, int32 y)
BEGIN_SUB
  _VDU(VDU_MOVETEXT);
  _VDU(x);
  _VDU(y);
END_SUB

SUB _NEWLINE()
BEGIN_SUB
  _VDU(CR);
  _VDU(LF);
END_SUB

SUB _OFF()
BEGIN_SUB
  DIM AS int32 n;
  _VDU(VDU_COMMAND);
  _VDU(1);
  _VDU(0);
  DEF_FOR (n = 1 TO n <= 7 STEP INCR n)
  BEGIN_FOR
   _VDU(0);
  NEXT
END_SUB

SUB _ON()
BEGIN_SUB
  DIM AS int32 n;
  _VDU(VDU_COMMAND);
  _VDU(1);
  _VDU(1);
  DEF_FOR (n = 1 TO n <= 7 STEP INCR n)
  BEGIN_FOR
    _VDU(0);
  NEXT
END_SUB

SUB _TINT(int32 action, int32 tint)
BEGIN_SUB
  DIM AS int32 n;
  _VDU(VDU_COMMAND);
  _VDU(17);
  _VDU(action);
  IF (tint <= MAXTINT) THEN_DO tint = tint << TINTSHIFT;
  _VDU(tint);
  DEF_FOR (n = 1 TO n <= 7 STEP INCR n)
  BEGIN_FOR
    _VDU(0);
  NEXT
END_SUB

SUB _GCOL(int32 action, int32 colour, int32 tint)
BEGIN_SUB
  _VDU(VDU_GRAPHCOL);
  _VDU(action);
  _VDU(colour);
  _TINT(colour < 128 ? TINT_FOREGRAPH : TINT_BACKGRAPH, tint);
END_SUB

SUB _GCOLNUM(int32 action, int32 background, int32 colnum)
BEGIN_SUB
  IF (background) THEN
    graph_back_action = action;
  ELSE
    graph_fore_action = action;
  END_IF
  set_graphics_colour(background, colnum);
END_SUB

SUB _GCOLRGB(int32 action, int32 background, int32 red, int32 green, int32 blue) {
  int32 colnum = _COLOURFN(red, green, blue);
  _GCOLNUM(action, background, colnum);
END_SUB

SUB _COLOURTINT(int32 colour, int32 tint)
BEGIN_SUB
  _VDU(VDU_TEXTCOL);
  _VDU(colour);
  _TINT(colour < 128 ? TINT_FORETEXT : TINT_BACKTEXT, tint);
END_SUB

SUB _MAPCOLOUR(int32 colour, int32 physcolour)
BEGIN_SUB
  _VDU(VDU_LOGCOL);
  _VDU(colour);
  _VDU(physcolour);
  _VDU(0);
  _VDU(0);
  _VDU(0);
END_SUB

SUB _SETCOLOUR(int32 background, int32 red, int32 green, int32 blue)
BEGIN_SUB
  int32 colnum = _COLOURFN(red, green, blue);
  set_text_colour(background, colnum);
END_SUB

SUB _SETCOLNUM(int32 background, int32 colnum)
BEGIN_SUB
  set_text_colour(background, colnum);
END_SUB

SUB _DEFCOLOUR(int32 colour, int32 red, int32 green, int32 blue)
BEGIN_SUB
  _VDU(VDU_LOGCOL);
  _VDU(colour);
  _VDU(16);
  _VDU(red);
  _VDU(green);
  _VDU(blue);
END_SUB

SUB _MOVE(int32 x, int32 y)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x, y);
END_SUB

SUB _MOVEBY(int32 x, int32 y)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_RELATIVE, x, y);
END_SUB

SUB _DRAW(int32 x, int32 y)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + DRAW_ABSOLUTE, x, y);
END_SUB

SUB _DRAWBY(int32 x, int32 y)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + DRAW_RELATIVE, x, y);
END_SUB

SUB _LINE(int32 x1, int32 y1, int32 x2, int32 y2)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x1, y1);
  _PLOT(DRAW_SOLIDLINE + DRAW_ABSOLUTE, x2, y2);
END_SUB

SUB _POINT(int32 x, int32 y)
BEGIN_SUB
  _PLOT(PLOT_POINT + DRAW_ABSOLUTE, x, y);
END_SUB

SUB _POINTBY(int32 x, int32 y)
BEGIN_SUB
  _PLOT(PLOT_POINT + DRAW_RELATIVE, x, y);
END_SUB

SUB _ELLIPSE(int32 x, int32 y, int32 majorlen, int32 minorlen, float64 angle, boolean isfilled)
BEGIN_SUB
  IF (angle NE 0.0) THEN_DO error(ERR_UNSUPPORTED);
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x, y);
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x + majorlen, y);
  IF (isfilled) THEN
    _PLOT(FILL_ELLIPSE + DRAW_ABSOLUTE, x, y + minorlen);
  ELSE
    _PLOT(PLOT_ELLIPSE + DRAW_ABSOLUTE, x, y + minorlen);
  END_IF
END_SUB

SUB _CIRCLE(int32 x, int32 y, int32 radius, boolean isfilled)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x, y);
  IF (isfilled) THEN
    _PLOT(FILL_CIRCLE + DRAW_ABSOLUTE, x - radius, y);
  ELSE
    _PLOT(PLOT_CIRCLE + DRAW_ABSOLUTE, x - radius, y);
  END_IF
END_SUB

SUB _DRAWRECT(int32 x1, int32 y1, int32 width, int32 height, boolean isfilled)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x1, y1);
  IF (isfilled) THEN
    _PLOT(FILL_RECTANGLE + DRAW_RELATIVE, width, height);
  ELSE
    _PLOT(DRAW_SOLIDLINE + DRAW_RELATIVE, width, 0);
    _PLOT(DRAW_SOLIDLINE + DRAW_RELATIVE, 0, height);
    _PLOT(DRAW_SOLIDLINE + DRAW_RELATIVE, -width, 0);
    _PLOT(DRAW_SOLIDLINE + DRAW_RELATIVE, 0, -height);
  END_IF
END_SUB

SUB _MOVERECT(int32 x1, int32 y1, int32 width, int32 height, int32 x2, int32 y2, boolean ismove)
BEGIN_SUB
  _PLOT(DRAW_SOLIDLINE + MOVE_ABSOLUTE, x1, y1);
  _PLOT(DRAW_SOLIDLINE + MOVE_RELATIVE, width, height);
  IF (ismove) THEN
    _PLOT(MOVE_RECTANGLE, x2, y2);
  ELSE
    _PLOT(COPY_RECTANGLE, x2, y2);
  END_IF
END_SUB

SUB _FILL(int32 x, int32 y)
BEGIN_SUB
  _PLOT(FLOOD_BACKGROUND + DRAW_ABSOLUTE, x, y);
END_SUB

SUB _FILLBY(int32 x, int32 y)
BEGIN_SUB
  _PLOT(FLOOD_BACKGROUND + DRAW_RELATIVE, x, y);
END_SUB

SUB _ORIGIN(int32 x, int32 y)
BEGIN_SUB
  _VDU(VDU_ORIGIN);
  _VDU(x & BYTEMASK);
  _VDU((x >> BYTESHIFT) & BYTEMASK);
  _VDU(y & BYTEMASK);
  _VDU((y >> BYTESHIFT) & BYTEMASK);
END_SUB

FUNCTION boolean _OPEN()
BEGIN_FUNCTION
  DIM AS static SDL_Surface PTR fontbuf;
  DIM AS int flags = SDL_DOUBLEBUF | SDL_HWSURFACE;
  IF (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) THEN
    PRINT_FILE (stderr, "Unable to init SDL: %s\n", SDL_GetError());
    RETURN_FUNCTION(FALSE);
  END_IF
  srand((unsigned)time(NULL));
  lastrandom = rand();
  randomoverflow = 0;
  screen0 = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, flags);
  IF (NOT screen0) THEN
    PRINT_FILE (stderr, "Failed to open screen: %s\n", SDL_GetError());
    RETURN_FUNCTION(FALSE);
  END_IF
  screen1 = SDL_DisplayFormat(screen0);
  modescreen = SDL_DisplayFormat(screen0);
  fontbuf = SDL_CreateRGBSurface(SDL_SWSURFACE,   XPPC,   YPPC, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  sdl_fontbuf = SDL_ConvertSurface(fontbuf, screen0->format, 0);
  SDL_FreeSurface(fontbuf);
  vdunext = 0;
  vduneeded = 0;
  enable_print = FALSE;
  graphmode = TEXTMODE;
  vscrwidth = SCREEN_WIDTH;
  vscrheight = SCREEN_HEIGHT;
  xgupp = ygupp = 1;
  SDL_EnableUNICODE(SDL_ENABLE);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  setup_mode(31);
  switch_graphics();
  xor_mask = SDL_MapRGB(sdl_fontbuf->format, 0xff, 0xff, 0xff);
  font_rect.x = font_rect.y = 0;
  font_rect.w = XPPC;
  font_rect.h = YPPC;
  place_rect.x = place_rect.y = 0;
  place_rect.w = XPPC;
  place_rect.h = YPPC;
  scale_rect.x = scale_rect.y = 0;
  scale_rect.w = 1;
  scale_rect.h = 1;
  RETURN_FUNCTION(TRUE);
END_FUNCTION

SUB _CLOSE()
BEGIN_SUB
  SDL_EnableUNICODE(SDL_DISABLE);
  SDL_Quit();
END_SUB

SUB _WAITKEY()
BEGIN_SUB
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
END_SUB

FUNCTION int _GETKEY(int choice)
BEGIN_FUNCTION
  DIM AS int done, e;
  DIM AS SDL_Event event;
  done = 0;
  e = 1;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (e)
    BEGIN_WHILE
      IF (choice EQ 0 OR choice EQ 2 OR choice EQ 4) THEN
      	e = SDL_PollEvent(AT event);
      	SDL_Delay(20);
      ELSE
        e = SDL_WaitEvent(AT event);
      END_IF
      SELECT_CASE (event.type)
      BEGIN_SELECT
        CASE SDL_KEYDOWN:
          IF (choice EQ 0 OR choice EQ 1) THEN
            RETURN_FUNCTION(event.key.keysym.unicode);
          ELSE_IF (choice EQ 2 OR choice EQ 3) THEN
            RETURN_FUNCTION(event.key.keysym.sym);
          ELSE_IF (choice EQ 4 OR choice EQ 5) THEN
            RETURN_FUNCTION(event.key.keysym.scancode);
          END_IF
          END_CASE
        CASE SDL_QUIT:
          done = 1;
          SDL_Quit();
          END_CASE
        CASE_ELSE
          IF (choice EQ 0 OR choice EQ 2 OR choice EQ 4) THEN
            RETURN_FUNCTION(0);
          END_IF 
          END_CASE
      END_SELECT
    WEND
  WEND
  RETURN_FUNCTION(0);
END_FUNCTION

FUNCTION int _MOUSE(int choice)
BEGIN_FUNCTION
  DIM AS int done, e;
  DIM AS SDL_Event event;
  done = 0;
  e = 1;
  DEF_WHILE (NOT done)
  BEGIN_WHILE
    DEF_WHILE (e)
    BEGIN_WHILE
    IF (choice EQ 2) THEN
    	e = SDL_PollEvent(AT event);
    	SDL_Delay(20);
    ELSE
      e = SDL_WaitEvent(AT event);
    END_IF  
      SELECT_CASE (choice)
      BEGIN_SELECT
        CASE 0:
          IF (event.type EQ SDL_MOUSEMOTION) THEN
            RETURN_FUNCTION(event.motion.x);
          END_IF
          END_CASE
        CASE 1:
          IF (event.type EQ SDL_MOUSEMOTION) THEN
            RETURN_FUNCTION(event.motion.y);
          END_IF
          END_CASE
        CASE 2:
        CASE 3:	
          IF (event.button.button EQ SDL_BUTTON_LEFT) THEN
            RETURN_FUNCTION(1);
          ELSE_IF (event.button.button EQ SDL_BUTTON_MIDDLE) THEN
            RETURN_FUNCTION(2);
          ELSE_IF (event.button.button EQ SDL_BUTTON_RIGHT) THEN
            RETURN_FUNCTION(3);
          END_IF
          END_CASE
        CASE_ELSE
          IF (choice EQ 2) THEN
            RETURN_FUNCTION(0);
          END_IF  
      END_SELECT
    WEND
    done = 1;
  WEND  
  RETURN_FUNCTION(-1);
END_FUNCTION
