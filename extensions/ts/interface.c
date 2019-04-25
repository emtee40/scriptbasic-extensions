// Tiny Scheme - Script BASIC extension module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "../../basext.h"
#include "cbasic.h"

#define USE_INTERFACE 1

#include "scheme.h"
#include "scheme-private.h"


/****************************
 Extension Module Functions
****************************/

besVERSION_NEGOTIATE
  RETURN_FUNCTION((int)INTERFACE_VERSION);
besEND

besSUB_START
  DIM AS long PTR p;
  besMODULEPOINTER = besALLOC(sizeof(long));
  IF (besMODULEPOINTER EQ NULL) THEN_DO RETURN_FUNCTION(0);
  p = (long PTR)besMODULEPOINTER;
  RETURN_FUNCTION(0);
besEND

besSUB_FINISH
  DIM AS long PTR p;
  p = (long PTR)besMODULEPOINTER;
  IF (p EQ NULL) THEN_DO RETURN_FUNCTION(0);
  RETURN_FUNCTION(0);
besEND


/***********************
 Tiny Scheme Functions
***********************/

besFUNCTION(TS_init_new)
  DIM AS scheme PTR sc;
  sc = scheme_init_new();
  besRETURN_LONG(sc);
besEND

besFUNCTION(TS_deinit)
  DIM AS scheme PTR sc;
  besARGUMENTS("i")
    AT sc
  besARGEND
  scheme_deinit(sc);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(TS_load_string)
  DIM AS scheme PTR sc;
  DIM AS char PTR cmdstr;
  DIM AS char tsbuf[16384];
  DIM AS int bufsz = 0;
  besARGUMENTS("iz[i]")
    AT sc, AT cmdstr, AT bufsz
  besARGEND
  IF (bufsz EQ 0) THEN
    bufsz = 16383;
  ELSE
    IF (bufsz > 16383) THEN_DO bufsz = 16383;
  END_IF
  scheme_set_output_port_string (sc, (char PTR) tsbuf, (char PTR) tsbuf + bufsz);
  scheme_load_string(sc, cmdstr);
  besRETURN_STRING(RTRIM(tsbuf));
besEND