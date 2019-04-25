/*
** This file is the SDL graphic portion of the Brandy Basic V Interpreter.
** Copyright (C) 2000, 2001, 2002, 2003, 2004 David Daniels
**
** SDL additions by Colin Tuckley
** libbbc / C BASIC fork by John Spikowski
*/

#define FALSE 0
#define TRUE 1
#define NIL 0
#define MAXINTVAL 2147483647
#define SMALLCONST 256
#define MAXFLOATVAL 1.7976931348623157E+308
#define TINYFLOATVAL 2.2250738585072014E-308
#define MAXEXPONENT 308
#define CR 0xD
#define LF 0xA
#define TAB '\t'
#define NUL '\0'
#define ESC 0x1B
#define VBAR 0x7C
#define BYTEMASK 0xFF
#define BYTESHIFT 8
#define CAST(x,y) ((y)(x))
#define TOINT(x) ((int32)(x))
#define TOFLOAT(x) ((float64)(x))
#define TOSTRING(x) ((char *)(x))
#define TOINTADDR(x) ((int32 *)(x))
#define ERR_UNSUPPORTED	1
#define ERR_NOGRAPHICS	3
#define ERR_BADMODE	104
#define MODEMASK 0x7F		/* Mode numbers are in the range 0..127 */
#define HIGHMODE 46		/* Standard RISC OS 3.1 modes are in the range 0..46 */
#define USERMODE 127		/* Pretend mode used before specific mode given */
#define COL256MASK 0x3F		/* Mask to extract colour number in 256 colour modes */
#define COL256SHIFT 2		/* Shift to move colour number to make room for tint */
#define TEXTCOLMASK 0x0F	/* Mask to limit logical colour number when not in full screen mode */
#define DEL 0x7F		/* ASCII code for the 'delete' character */
#define TINT_FORETEXT 0
#define TINT_BACKTEXT 1
#define TINT_FOREGRAPH 2
#define TINT_BACKGRAPH 3
#define EXCH_TEXTCOLS 5
#define MAXTINT 3		/* Highest value for TINT */
#define TINTMASK 0xC0		/* Mask to extract TINT value */
#define TINTSHIFT 6		/* Shift to move TINT value to least significant bits of byte */
#define C256_REDBIT 0x02	/* Mask for most sig bit of red component in 256 colour colour number */
#define C256_GREENBIT 0x08
#define C256_BLUEBIT 0x20
#define ABSCOORD_MASK	4	/* Mask to check relative/absolute coordinate bit */
#define PLOT_COLMASK	3	/* Mask to extract colour type to use */
#define PLOT_MOVEONLY	0	/* Move graphics cursor only */
#define PLOT_FOREGROUND	1	/* Use graphics foreground colour */
#define PLOT_INVERSE	2	/* Use logical inverse colour */
#define PLOT_BACKGROUND	3	/* Use graphics background colour */
#define MOVE_RELATIVE	0	/* Move cursor relative to last graphics position */
#define DRAW_RELATIVE	1	/* Draw line relative to last graphics position */
#define MOVE_ABSOLUTE	4	/* Move cursor to actual coordinate given */
#define DRAW_ABSOLUTE	5	/* Draw line to actual coordinate given */
#define DRAW_SOLIDLINE	0	/* Draw a solid line including both end points */
#define PLOT_POINT	0x40	/* Plot a single point */
#define FILL_TRIANGLE	0x50	/* Plot a filled triangle */
#define FILL_RECTANGLE	0x60	/* Plot a filled rectangle */
#define FILL_PARALLELOGRAM 0x70	/* Plot a filled parallelogram */
#define FLOOD_BACKGROUND 0x80	/* Flood fill as far as background colour */
#define PLOT_CIRCLE	0x90	/* Plot a circle outline */
#define FILL_CIRCLE	0x98	/* Plot a filled circle */
#define SHIFT_RECTANGLE	0xB8	/* Move or copy rectangle */
#define MOVE_RECTANGLE	0xBD	/* Move rectangle absolute */
#define COPY_RECTANGLE	0xBE	/* Copy rectangle absolute */
#define PLOT_ELLIPSE	0xC0	/* Plot an ellipse outline */
#define FILL_ELLIPSE	0xC8	/* Plot a filled ellipse */
#define GRAPHOP_MASK	0xF8	/* Mask to extract graphics operation */
#define GRAPHHOW_MASK	0x07	/* Mask to extract details of operation */
#define OVERWRITE_POINT 0	/* Overwrite point on screen */
#define OR_POINT	1	/* OR with point */
#define AND_POINT	2	/* AND with point */
#define EOR_POINT	3	/* Exclusive OR with point */
#define INVERT_POINT	4	/* Invert colour on screen */
#define LEAVE_POINT	5	/* Leave point untouched */
#define ANDNOT_POINT	6	/* AND point with NOT graphics foreground colour */
#define ORNOT_POINT	7	/* OR point with NOT graphics foreground colour */
#define VDU_NULL	0	/* Do nothing */
#define VDU_PRINT	1	/* Send next character to printer */
#define VDU_ENAPRINT	2	/* Enable printer */
#define VDU_DISPRINT	3	/* Disable printer */
#define VDU_TEXTCURS	4	/* Write text at text cursor position */
#define VDU_GRAPHICURS	5	/* Write text at graphics position */
#define VDU_ENABLE	6	/* Enable VDU driver */
#define VDU_BEEP	7	/* Generate 'bell' sound */
#define VDU_CURBACK	8	/* Move cursor back one position */
#define VDU_CURFORWARD	9	/* Move cursor forwards one position */
#define VDU_CURDOWN	10	/* Move cursor down one line */
#define VDU_CURUP	11	/* Move cursor up one line */
#define VDU_CLEARTEXT	12	/* Clear text window */
#define VDU_RETURN	13	/* Move cursor to start of line */
#define VDU_ENAPAGE	14	/* Enable 'page' mode */
#define VDU_DISPAGE	15	/* Disable 'page' mode */
#define VDU_CLEARGRAPH	16	/* Clear graphics window */
#define VDU_TEXTCOL	17	/* Define text colour to use */
#define VDU_GRAPHCOL	18	/* Define graphics colour and plot action */
#define VDU_LOGCOL	19	/* Define logical colour */
#define VDU_RESTCOL	20	/* Restore logical colours to default values */
#define VDU_DISABLE	21	/* Disable VDU driver */
#define VDU_SCRMODE	22	/* Select screen mode */
#define VDU_COMMAND	23	/* Multitudenous VDU commands */
#define VDU_DEFGRAPH	24	/* Define graphics window */
#define VDU_PLOT	25	/* PLOT command */
#define VDU_RESTWIND	26	/* Restore default windows */
#define VDU_ESCAPE	27	/* Does nothing */
#define VDU_DEFTEXT	28	/* Define text window */
#define VDU_ORIGIN	29	/* Define graphics origin */
#define VDU_HOMETEXT	30	/* Send text cursor to home position */
#define VDU_MOVETEXT	31	/* Move text cursor */
#define VDU_BLACK	0
#define VDU_RED		1
#define VDU_GREEN	2
#define VDU_YELLOW	3
#define VDU_BLUE	4
#define VDU_MAGENTA	5
#define VDU_CYAN	6
#define VDU_WHITE	7
#define FLASH_BLAWHITE	8
#define FLASH_REDCYAN	9
#define FLASH_GREENMAG	10
#define FLASH_YELBLUE	11
#define FLASH_BLUEYEL	12
#define FLASH_MAGREEN	13
#define FLASH_CYANRED	14
#define FLASH_WHITEBLA	15
#define MAXBYTES	9	/* VDU commands need at most nine bytes of data */
#define MAXSTRING 65536
#define TOFLOAT(x) ((float64)(x))
#define TOINT(x) ((int32)(x))
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define MAX_YRES 1280
#define MAX_XRES 16384
#define FAST_2_MUL(x) ((x)<<1)
#define FAST_3_MUL(x) (((x)<<1)+x)
#define FAST_4_DIV(x) ((x)>>2)
#define GXTOPX(x) ((x) / xgupp + xbufoffset)
#define GYTOPY(y) ((ygraphunits - 1 -(y)) / ygupp + ybufoffset)
#define XPPC 8		/* Size of character in pixels in X direction */
#define YPPC 8		/* Size of character in pixels in Y direction */
#define COLOURSTEP 68		/* RGB colour value increment used in 256 colour modes */
#define TINTSTEP 17		/* RGB colour value increment used for tints */
#define FILLSTACK 500

typedef unsigned char byte;
typedef unsigned char boolean;
typedef int int32;			/* Type for 32-bit integer variables in Basic */
typedef unsigned int uint32;		/* 32-bit unsigned integer */
typedef double float64;			/* Type for 64-bit floating point variables in Basic */
typedef enum {NOGRAPHICS, TEXTONLY, TEXTMODE, FULLSCREEN} graphics;
typedef enum {BLOCK, UNDERLINE} curstype;
typedef enum {NOCURSOR, HIDDEN, SUSPENDED, ONSCREEN} curstate;
typedef enum {SCROLL_UP, SCROLL_DOWN} updown;
typedef struct {
  int32 xres;			/* RISC OS screen width in pixels */
  int32 yres;			/* RISC OS screen height in pixels */
  int32 coldepth;		/* Number of colours */
  int32 xgraphunits;		/* Screen width in RISC OS graphics units */
  int32 ygraphunits;		/* Screen height in RISC OS graphics units */
  int32 xtext;			/* Screen width in characters */
  int32 ytext;			/* Screen height in characters */
  int32 xscale;			/* X direction scale factor for points */
  int32 yscale;			/* Y direction scale factor for points */
  boolean graphics;		/* TRUE if this mode supports graphics */
} modetails;

static char bbcbuf[256];
static SDL_Surface *screen0, *screen1, *sdl_fontbuf;
static SDL_Surface *modescreen;	/* Buffer used when screen mode is scaled to fit real screen */
static SDL_Rect font_rect, place_rect, scroll_rect, line_rect, scale_rect;
Uint32 tf_colour,       /* text foreground SDL rgb triple */
tb_colour,       /* text background SDL rgb triple */
gf_colour,       /* graphics foreground SDL rgb triple */
gb_colour;       /* graphics background SDL rgb triple */
Uint32 xor_mask;
static Uint8 palette[768];		/* palette for screen */
static int32
  vscrwidth,			/* Width of virtual screen in pixels */
  vscrheight,			/* Height of virtual screen in pixels */
  screenwidth,			/* RISC OS width of current screen mode in pixels */
  screenheight,			/* RISC OS height of current screen mode in pixels */
  xgraphunits,			/* Screen width in RISC OS graphics units */
  ygraphunits,			/* Screen height in RISC OS graphics units */
  gwinleft,			/* Left coordinate of graphics window in RISC OS graphics units */
  gwinright,			/* Right coordinate of graphics window in RISC OS graphics units */
  gwintop,			/* Top coordinate of graphics window in RISC OS graphics units */
  gwinbottom,			/* Bottom coordinate of graphics window in RISC OS graphics units */
  xgupp,			/* RISC OS graphic units per pixel in X direction */
  ygupp,			/* RISC OS graphic units per pixel in Y direction */
  graph_fore_action,		/* Foreground graphics PLOT action (ignored) */
  graph_back_action,		/* Background graphics PLOT action (ignored) */
  graph_forecol,		/* Current graphics foreground logical colour number */
  graph_backcol,		/* Current graphics background logical colour number */
  graph_physforecol,		/* Current graphics foreground physical colour number */
  graph_physbackcol,		/* Current graphics background physical colour number */
  graph_foretint,		/* Tint value added to foreground graphics colour in 256 colour modes */
  graph_backtint,		/* Tint value added to background graphics colour in 256 colour modes */
  xlast,			/* Graphics X coordinate of last point visited */
  ylast,			/* Graphics Y coordinate of last point visited */
  xlast2,			/* Graphics X coordinate of last-but-one point visited */
  ylast2,			/* Graphics Y coordinate of last-but-one point visited */
  xorigin,			/* X coordinate of graphics origin */
  yorigin,			/* Y coordinate of graphics origin */
  xscale,			/* X direction scale factor */
  yscale,			/* Y direction scale factor */
  xoffset,			/* X offset to centre screen mode in 800 by 600 screen in pixels */
  yoffset,			/* Y offset to centre screen mode in 800 by 600 screen in pixels */
  xbufoffset,			/* X offset in screen buffer used for plotting graphics */
  ybufoffset;			/* Y offset in screen buffer used for plotting graphics */
static int32 left[MAX_YRES], right[MAX_YRES];
static boolean
  scaled,			/* TRUE if screen mode is scaled to fit real screen */
  vdu5mode,			/* TRUE if text output goes to graphics cursor */
  clipping;			/* TRUE if clipping region is not full screen of a RISC OS mode */
static graphics graphmode;	/* Says whether graphics are possible or not */
static byte sysfont [224][8] = {
/*   */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* ! */  {0x18u, 0x18u, 0x18u, 0x18u, 0x18u, 0u, 0x18u, 0u},
/* " */  {0x6cu, 0x6cu, 0x6cu, 0u, 0u, 0u, 0u, 0u},
/* # */  {0x6cu, 0x6cu, 0xfeu, 0x6cu, 0xfeu, 0x6cu, 0x6cu, 0u},
/* $ */  {0x18u, 0x3eu, 0x78u, 0x3cu, 0x1eu, 0x7cu, 0x18u, 0u},
/* % */  {0x62u, 0x66u, 0x0cu, 0x18u, 0x30u, 0x66u, 0x46u, 0u},
/* & */  {0x70u, 0xd8u, 0xd8u, 0x70u, 0xdau, 0xccu, 0x76u, 0u},
/* ' */  {0x0cu, 0x0cu, 0x18u, 0u, 0u, 0u, 0u, 0u},
/* ( */  {0x0cu, 0x18u, 0x30u, 0x30u, 0x30u, 0x18u, 0x0cu, 0u},
/* ) */  {0x30u, 0x18u, 0x0cu, 0x0cu, 0x0cu, 0x18u, 0x30u, 0u},
/* * */  {0x44u, 0x6cu, 0x38u, 0xfeu, 0x38u, 0x6cu, 0x44u, 0u},
/* + */  {0u, 0x18u, 0x18u, 0x7eu, 0x18u, 0x18u, 0u, 0u},
/* , */  {0u, 0u, 0u, 0u, 0u, 0x18u, 0x18u, 0x30u},
/* - */  {0u, 0u, 0u, 0xfeu, 0u, 0u, 0u, 0u},
/* . */  {0u, 0u, 0u, 0u, 0u, 0x18u, 0x18u, 0u},
/* / */  {0u, 0x6u, 0x0cu, 0x18u, 0x30u, 0x60u, 0u, 0u},
/* 0 */  {0x7cu, 0xc6u, 0xceu, 0xd6u, 0xe6u, 0xc6u, 0x7cu, 0u},
/* 1 */  {0x18u, 0x38u, 0x18u, 0x18u, 0x18u, 0x18u, 0x7eu, 0u},
/* 2 */  {0x7cu, 0xc6u, 0x0cu, 0x18u, 0x30u, 0x60u, 0xfeu, 0u},
/* 3 */  {0x7cu, 0xc6u, 0x6u, 0x1cu, 0x6u, 0xc6u, 0x7cu, 0u},
/* 4 */  {0x1cu, 0x3cu, 0x6cu, 0xccu, 0xfeu, 0x0cu, 0x0cu, 0u},
/* 5 */  {0xfeu, 0xc0u, 0xfcu, 0x6u, 0x6u, 0xc6u, 0x7cu, 0u},
/* 6 */  {0x3cu, 0x60u, 0xc0u, 0xfcu, 0xc6u, 0xc6u, 0x7cu, 0u},
/* 7 */  {0xfeu, 0x6u, 0x0cu, 0x18u, 0x30u, 0x30u, 0x30u, 0u},
/* 8 */  {0x7cu, 0xc6u, 0xc6u, 0x7cu, 0xc6u, 0xc6u, 0x07cu, 0u},
/* 9 */  {0x7cu, 0xc6u, 0xc6u, 0x7eu, 0x6u, 0x0cu, 0x78u, 0u},
/* : */  {0u, 0u, 0x18u, 0x18u, 0u, 0x18u, 0x18u, 0u},
/* ; */  {0u, 0u, 0x18u, 0x18u, 0u, 0x18u, 0x18u, 0x30u},
/* < */  {0x6u, 0x1cu, 0x70u, 0xc0u, 0x70u, 0x1cu, 0x6u, 0u},
/* = */  {0u, 0u, 0xfeu, 0u, 0xfeu, 0u, 0u, 0u},
/* > */  {0xc0u, 0x70u, 0x1cu, 0x6u, 0x1cu, 0x70u, 0xc0u, 0u},
/* ? */  {0x7cu, 0xc6u, 0xc6u, 0x0cu, 0x18u, 0u, 0x18u, 0u},
/* @ */  {0x7cu, 0xc6u, 0xdeu, 0xd6u, 0xdcu, 0xc0u, 0x7cu, 0u},
/* A */  {0x7cu, 0xc6u, 0xc6u, 0xfeu, 0xc6u, 0xc6u, 0xc6u, 0u},
/* B */  {0xfcu, 0xc6u, 0xc6u, 0xfcu, 0xc6u, 0xc6u, 0xfcu, 0u},
/* C */  {0x7cu, 0xc6u, 0xc0u, 0xc0u, 0xc0u, 0xc6u, 0x7cu, 0u},
/* D */  {0xf8u, 0xccu, 0xc6u, 0xc6u, 0xc6u, 0xccu, 0xf8u, 0u},
/* E */  {0xfeu, 0xc0u, 0xc0u, 0xfcu, 0xc0u, 0xc0u, 0xfeu, 0u},
/* F */  {0xfeu, 0xc0u, 0xc0u, 0xfcu, 0xc0u, 0xc0u, 0xc0u, 0u},
/* G */  {0x7cu, 0xc6u, 0xc0u, 0xceu, 0xc6u, 0xc6u, 0x7cu, 0u},
/* H */  {0xc6u, 0xc6u, 0xc6u, 0xfeu, 0xc6u, 0xc6u, 0xc6u, 0u},
/* I */  {0x7eu, 0x18u, 0x18u, 0x18u, 0x18u, 0x18u, 0x7eu, 0u},
/* J */  {0x3eu, 0x0cu, 0x0cu, 0x0cu, 0x0cu, 0xccu, 0x78u, 0u},
/* K */  {0xc6u, 0xccu, 0xd8u, 0xf0u, 0xd8u, 0xccu, 0xc6u, 0u},
/* L */  {0xc0u, 0xc0u, 0xc0u, 0xc0u, 0xc0u, 0xc0u, 0xfeu, 0u},
/* M */  {0xc6u, 0xeeu, 0xfeu, 0xd6u, 0xd6u, 0xc6u, 0xc6u, 0u},
/* N */  {0xc6u, 0xe6u, 0xf6u, 0xdeu, 0xceu, 0xc6u, 0xc6u, 0u},
/* O */  {0x7cu, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0x7cu, 0u},
/* P */  {0xfcu, 0xc6u, 0xc6u, 0xfcu, 0xc0u, 0xc0u, 0xc0u, 0u},
/* Q */  {0x7cu, 0xc6u, 0xc6u, 0xc6u, 0xcau, 0xccu, 0x76u, 0u},
/* R */  {0xfcu, 0xc6u, 0xc6u, 0xfcu, 0xccu, 0xc6u, 0xc6u, 0u},
/* S */  {0x7cu, 0xc6u, 0xc0u, 0x7cu, 0x06u, 0xc6u, 0x7cu, 0u},
/* T */  {0xfeu, 0x18u, 0x18u, 0x18u, 0x18u, 0x18u, 0x18u, 0u},
/* U */  {0xc6u, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0x7cu, 0u},
/* V */  {0xc6u, 0xc6u, 0x6cu, 0x6cu, 0x38u, 0x38u, 0x10u, 0u},
/* W */  {0xc6u, 0xc6u, 0xd6u, 0xd6u, 0xfeu, 0xeeu, 0xc6u, 0u},
/* X */  {0xc6u, 0x6cu, 0x38u, 0x10u, 0x38u, 0x6cu, 0xc6u, 0u},
/* Y */  {0xc6u, 0xc6u, 0x6cu, 0x38u, 0x18u, 0x18u, 0x18u, 0u},
/* Z */  {0xfeu, 0x0cu, 0x18u, 0x30u, 0x60u, 0xc0u, 0xfeu, 0u},
/* [ */  {0x7cu, 0x60u, 0x60u, 0x60u, 0x60u, 0x60u, 0x7cu, 0u},
/* \ */  {0u, 0x60u, 0x30u, 0x18u, 0x0cu, 0x6u, 0u, 0u},
/* ] */  {0x3eu, 0x6u, 0x6u, 0x6u, 0x6u, 0x6u, 0x3eu, 0u},
/* ^ */  {0x10u, 0x38u, 0x6cu, 0xc6u, 0x82u, 0u, 0u, 0u},
/* _ */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0xffu},
/* ` */  {0x3cu, 0x66u, 0x60u, 0xfcu, 0x60u, 0x60u, 0xfeu, 0u},
/* a */  {0u, 0u, 0x7cu, 0x6u, 0x7eu, 0xc6u, 0x7eu, 0u},
/* b */  {0xc0u, 0xc0u, 0xfcu, 0xc6u, 0xc6u, 0xc6u, 0xfcu, 0u},
/* c */  {0u, 0u, 0x7cu, 0xc6u, 0xc0u, 0xc6u, 0x7cu, 0u},
/* d */  {0x6u, 0x6u, 0x7eu, 0xc6u, 0xc6u, 0xc6u, 0x7eu, 0u},
/* e */  {0u, 0u, 0x7cu, 0xc6u, 0xfeu, 0xc0u, 0x7cu, 0u},
/* f */  {0x3eu, 0x60u, 0x60u, 0xfcu, 0x60u, 0x60u, 0x60u, 0u},
/* g */  {0u, 0u, 0x7eu, 0xc6u, 0xc6u, 0x7eu, 0x6u, 0x7cu},
/* h */  {0xc0u, 0xc0u, 0xfcu, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0u},
/* i */  {0x18u, 0u, 0x78u, 0x18u, 0x18u, 0x18u, 0x7eu, 0u},
/* j */  {0x18u, 0u, 0x38u, 0x18u, 0x18u, 0x18u, 0x18u, 0x70u},
/* k */  {0xc0u, 0xc0u, 0xc6u, 0xccu, 0xf8u, 0xccu, 0xc6u, 0u},
/* l */  {0x78u, 0x18u, 0x18u, 0x18u, 0x18u, 0x18u, 0x7eu, 0u},
/* m */  {0u, 0u, 0xecu, 0xfeu, 0xd6u, 0xd6u, 0xc6u, 0u},
/* n */  {0u, 0u, 0xfcu, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0u},
/* o */  {0u, 0u, 0x7cu, 0xc6u, 0xc6u, 0xc6u, 0x7cu, 0u},
/* p */  {0u, 0u, 0xfcu, 0xc6u, 0xc6u, 0xfcu, 0xc0u, 0xc0u},
/* q */  {0u, 0u, 0x7eu, 0xc6u, 0xc6u, 0x7eu, 0x6u, 0x7u},
/* r */  {0u, 0u, 0xdcu, 0xf6u, 0xc0u, 0xc0u, 0xc0u, 0u},
/* s */  {0u, 0u, 0x7eu, 0xc0u, 0x7cu, 0x6u, 0xfcu, 0u},
/* t */  {0x30u, 0x30u, 0xfcu, 0x30u, 0x30u, 0x30u, 0x1eu, 0u},
/* u */  {0u, 0u, 0xc6u, 0xc6u, 0xc6u, 0xc6u, 0x7eu, 0u},
/* v */  {0u, 0u, 0xc6u, 0xc6u, 0x6cu, 0x38u, 0x10u, 0u},
/* w */  {0u, 0u, 0xc6u, 0xd6u, 0xd6u, 0xfeu, 0xc6u, 0u},
/* x */  {0u, 0u, 0xc6u, 0x6cu, 0x38u, 0x6cu, 0xc6u, 0u},
/* y */  {0u, 0u, 0xc6u, 0xc6u, 0xc6u, 0x7eu, 0x6u, 0x7cu},
/* z */  {0u, 0u, 0xfeu, 0x0cu, 0x38u, 0x60u, 0xfeu, 0u},
/* { */  {0x0cu, 0x18u, 0x18u, 0x70u, 0x18u, 0x18u, 0x0cu, 0u},
/* | */  {0x18u, 0x18u, 0x18u, 0u, 0x18u, 0x18u, 0x18u, 0u},
/* } */  {0x30u, 0x18u, 0x18u, 0xeu, 0x18u, 0x18u, 0x30u, 0u},
/* ~ */  {0x31u, 0x6bu, 0x46u, 0u, 0u, 0u, 0u, 0u},
/* DEL */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* 0x80 */
/* \80 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \81 */  {0x1C, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0u},
/* \82 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \83 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \84 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \85 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \86 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \87 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \88 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \89 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8A */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8B */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8C */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8D */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8E */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \8F */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* 90 */
/* \90 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \91 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \92 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \93 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \94 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \95 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \96 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \97 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \98 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \99 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9A */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9B */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9C */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9D */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9E */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \9F */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* a0 */
/* \A0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \A9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AD */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \AF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* b0 */
/* \B0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \B9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BD */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \BF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* c0 */
/* \C0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \C9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CD */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \CF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* d0 */
/* \D0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \D9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DD */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \DF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* e0 */
/* \E0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \E9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \EA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \EB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \EC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \ED */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \EE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \EF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
	/* f0 */
/* \F0 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F1 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F2 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F3 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F4 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F5 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F6 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F7 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F8 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \F9 */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \FA */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \FB */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \FC */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/*   */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \FE */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
/* \FF */  {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}
};
static int32 vdubytes [] = {	/* Number of bytes of data needed for each VDU command */
  0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 2, 5, 0, 0, 1, 9,
  8, 5, 0, 0, 4, 4, 0, 2
};
static modetails modetable [] = {
/*  0 */  { 640, 256,   2, 1280,  1024,  80, 32, 1, 2,  TRUE},
/*  1 */  { 320, 256,   4, 1280,  1024,  40, 32, 2, 2,  TRUE},
/*  2 */  { 160, 256,  16, 1280,  1024,  20, 32, 4, 2,  TRUE},
/*  3 */  {   0,   0,   2,    0,     0,  80, 25, 1, 2, FALSE},
/*  4 */  { 320, 256,   2, 1280,  1024,  40, 32, 2, 2,  TRUE},
/*  5 */  { 160, 256,   4, 1280,  1024,  20, 32, 4, 2,  TRUE},
/*  6 */  {   0,   0,   2,    0,     0,  40, 25, 2, 2, FALSE},
/*  7 */  {   0,   0,  16,    0,     0,  40, 25, 2, 2, FALSE},
/*  8 */  { 640, 256,   4, 1280,  1024,  80, 32, 1, 2,  TRUE},
/*  9 */  { 320, 256,  16, 1280,  1024,  40, 32, 2, 2,  TRUE},
/* 10 */  { 160, 256, 256, 1280,  1024,  20, 32, 4, 2,  TRUE},
/* 11 */  { 640, 250,   4, 1280,  1000,  80, 25, 1, 2,  TRUE},
/* 12 */  { 640, 256,  16, 1280,  1024,  80, 32, 1, 2,  TRUE},
/* 13 */  { 320, 256, 256, 1280,  1024,  40, 32, 2, 2,  TRUE},
/* 14 */  { 640, 250,  16, 1280,  1000,  80, 32, 1, 2,  TRUE},
/* 15 */  { 640, 256, 256, 1280,  1024,  80, 32, 1, 2,  TRUE},
/* 16 */  {1056, 256,  16, 2112,  1024, 132, 32, 1, 2,  TRUE},
/* 17 */  {1056, 250,  16, 2112,  1000, 132, 32, 1, 2,  TRUE},
/* 18 */  { 640, 512,   2, 1280,  1024,  80, 64, 1, 1,  TRUE},
/* 19 */  { 640, 512,   4, 1280,  1024,  80, 64, 1, 1,  TRUE},
/* 20 */  { 640, 512,  16, 1280,  1024,  80, 64, 1, 1,  TRUE},
/* 21 */  { 640, 512, 256, 1280,  1024,  80, 64, 1, 1,  TRUE},
/* 22 */  { 768, 288,  16,  768,   576,  96, 36, 1, 2,  TRUE},
/* 23 */  {1152, 896,   2, 2304,  1792, 144, 56, 1, 1,  TRUE},
/* 24 */  {1056, 256, 256, 2112,  1024, 132, 32, 1, 2,  TRUE},
/* 25 */  { 640, 480,   2, 1280,   960,  80, 60, 1, 1,  TRUE},
/* 26 */  { 640, 480,   4, 1280,   960,  80, 60, 1, 1,  TRUE},
/* 27 */  { 640, 480,  16, 1280,   960,  80, 60, 1, 1,  TRUE},
/* 28 */  { 640, 480, 256, 1280,   960,  80, 60, 1, 1,  TRUE},
/* 29 */  { 800, 600,   2, 1280,  1200, 100, 75, 1, 1,  TRUE},
/* 30 */  { 800, 600,   4, 1600,  1200, 100, 75, 1, 1,  TRUE},
/* 31 */  { 800, 600,  16, 1600,  1200, 100, 75, 1, 1,  TRUE},
/* 32 */  { 800, 600, 256, 1600,  1200, 100, 75, 1, 1,  TRUE},
/* 33 */  { 768, 288,   2, 1536,  1152,  96, 36, 1, 2,  TRUE},
/* 34 */  { 768, 288,   4, 1536,  1152,  96, 36, 1, 2,  TRUE},
/* 35 */  { 768, 288,  16, 1536,  1152,  96, 36, 1, 2,  TRUE},
/* 36 */  { 768, 288, 256, 1536,  1152,  96, 36, 1, 2,  TRUE},
/* 37 */  { 896, 352,   2, 1792,  1408, 112, 44, 1, 1,  TRUE},
/* 38 */  { 896, 352,   4, 1792,  1408, 112, 44, 1, 1,  TRUE},
/* 39 */  { 896, 352,  16, 1792,  1408, 112, 44, 1, 1,  TRUE},
/* 40 */  { 896, 352, 256, 1792,  1408, 112, 44, 1, 1,  TRUE},
/* 41 */  { 640, 352,   2, 1280,  1408,  80, 44, 1, 1,  TRUE},
/* 42 */  { 640, 352,   4, 1280,  1408,  80, 44, 1, 1,  TRUE},
/* 43 */  { 640, 352,  16, 1280,  1408,  80, 44, 1, 1,  TRUE},
/* 44 */  { 640, 200,   2, 1280,   800,  80, 25, 1, 2,  TRUE},
/* 45 */  { 640, 200,   4, 1280,   800,  80, 25, 1, 2,  TRUE},
/* 46 */  { 640, 200,  16, 1280,   800,  80, 25, 1, 2,  TRUE}
};
static int32
  vducmd,			/* Current VDU command */
  vdunext,			/* Index of next entry in VDU queue */
  vduneeded,			/* Number of bytes needed for current VDU command */
  screenmode,			/* Current screen mode */
  colourdepth,			/* Number of colours allowed in current screen mode */
  colourmask,			/* Mask to isolate logical colour number */
  text_forecol,			/* Current text foreground logical colour number */
  text_backcol,			/* Current text background logical colour number */
  text_physforecol,		/* Current text foreground physical colour number */
  text_physbackcol,		/* Current text background physical colour number */
  text_foretint,		/* Tint value added to foreground text colour in 256 colour modes */
  text_backtint,		/* Tint value added to background text colour in 256 colour modes */
  textwidth,			/* Width of current screen mode in characters */
  textheight,			/* Height of current screen mode in characters */
  twinleft,			/* Column number of left-hand side of text window */
  twinright,			/* Column number of right-hand side of text window */
  twintop,			/* Row number of top of text window */
  twinbottom,			/* Row number of bottom of text window */
  xtext,			/* Text cursor X coordinate (real on-screen location) */
  ytext;			/* Text cursor Y coordinate (real on-screen location) */
static boolean
  enable_vdu,			/* TRUE if VDU driver is enable */
  enable_print,			/* TRUE if sending characters to the printer stream is enabled */
  echo,				/* TRUE if character should be echoed on screen immediately */
  textwin;			/* TRUE if a text window has been defined */
static curstype cursmode;	/* Type of cursor being displayed in graphics mode */
static curstate cursorstate;	/* Whether cursor is shown */
static byte vduqueue[MAXBYTES];	/* Queue to hold data for VDU commands */
static int32 logtophys[16];
static int32 lastrandom;    
static int32 randomoverflow;
static float64 floatvalue;

extern void _TAB(int32, int32);
extern void echo_on(void);
extern void echo_off(void);
extern void set_cursor(boolean);
extern void _VDU(int32);
extern void _VDUSTR(char *);
extern void _PRINTF(char *, ...);
extern void _NEWLINE(void);
extern int32 _VDUFN(int32);
extern int32 _POS(void);
extern int32 _VPOS(void);
extern void _MODE(int32);
extern void _MODESTR(int32, int32, int32, int32, int32, int32, int32);
extern void _NEWMODE(int32, int32, int32, int32);
extern void _OFF(void);
extern void _ON(void);
extern int32 _MODEFN(void);
extern int32 _COLOURFN(int32, int32, int32);
extern void _COLOURTINT(int32, int32);
extern void _MAPCOLOUR(int32, int32);
extern void _SETCOLOUR(int32, int32, int32, int32);
extern void _SETCOLNUM(int32, int32);
extern void _DEFCOLOUR(int32, int32, int32, int32);
extern void _GCOL(int32, int32, int32);
extern void _GCOLRGB(int32, int32, int32, int32, int32);
extern void _GCOLNUM(int32, int32, int32);
extern void _TINT(int32, int32);
extern int32 _TINTFN(int32, int32);
extern void _PLOT(int32, int32, int32);
extern int32 _POINTFN(int32, int32);
extern void _MOVE(int32, int32);
extern void _MOVEBY(int32, int32);
extern void _DRAW(int32, int32);
extern void _DRAWBY(int32, int32);
extern void _POINT(int32, int32);
extern void _POINTBY(int32, int32);
extern void _POINTTO(int32, int32);
extern void _LINE(int32, int32, int32, int32);
extern void _CIRCLE(int32, int32, int32, boolean);
extern void _ELLIPSE(int32, int32, int32, int32, float64, boolean);
extern void _DRAWRECT(int32, int32, int32, int32, boolean);
extern void _moverect(int32, int32, int32, int32, int32, int32, boolean);
extern void _FILL(int32, int32);
extern void _FILLBY(int32, int32);
extern void _ORIGIN(int32, int32);
extern void _WAIT(void);
extern void find_cursor(void);
extern boolean _OPEN(void);
extern void _CLOSE(void);

extern void draw_line(SDL_Surface *, int32, int32, int32, int32, int32, int32, Uint32);
extern void filled_triangle(SDL_Surface *, int32, int32, int32, int32, int32, int32, int32, int32, Uint32);
extern void draw_ellipse(SDL_Surface *, int32, int32, int32, int32, int32, int32, Uint32);
extern void filled_ellipse(SDL_Surface *, int32, int32, int32, int32, int32, int32, Uint32);
static void toggle_cursor(void);
static void switch_graphics(void);
static void vdu_cleartext(void);

extern void _WAITKEY(void);
extern int32 _GETKEY(int);
extern int32 _SHIFT(int32, int32, int32);
extern int32 _ROTATE(int32, int32);
extern int _MOUSE(int);
