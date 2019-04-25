/*  V7 JavaScript Engine Extension Module */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "v7.h"
#include "../../basext.h"
#include "../../scriba.h"
#include "cbasic.h"

struct prop_iter_ctx ctx;

union dv {
  double d;
  v7_val_t v;
} ;

static double v2d(v7_val_t v) {
  union dv dv;
  dv.v = v;
  return dv.d;
}

static v7_val_t d2v(double d) {
  union dv dv;
  dv.d = d;
  return dv.v;
}

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


/*****************
 Utility Funtions
*****************/

besFUNCTION(SB_shifts)
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

besFUNCTION(SB_rotates)
  DIM AS int co, v, x, y, d, p, ar;
  besARGUMENTS("ii[i]")
    AT v, AT p, AT ar
  besARGEND
  IF (ar EQ NULL) THEN_DO ar = 0;
  x = 0xffffffff & v;
  d = p;
  IF (d < 0) THEN_DO d=-d;
  DEF_FOR (co = 1 TO co <= d STEP INCR co)
  BEGIN_FOR
    IF (p >= 0) THEN
      y = x << 1; // bit shift left
      IF (x & 0x80000000) THEN_DO y |= 1;
    ELSE
      y = x >> 1; // bit shift right
      IF (x & 1) THEN_DO y |= 0x80000000;
    END_IF
    x = y;
  NEXT
  besRETURN_LONG(y);
besEND

besCOMMAND(js_iif)
  DIM AS NODE nItem;
  DIM AS VARIABLE Op1;
  DIM long ConditionValue;
  USE_CALLER_MORTALS;
  nItem = besPARAMETERLIST;
  IF (NOT nItem) THEN
    RESULT = NULL;
    RETURN;
  END_IF
  Op1 = besEVALUATEEXPRESSION(CAR(nItem));
  ASSERTOKE;
  IF (Op1 EQ NULL) THEN
     ConditionValue = 0;
  ELSE
    Op1 = besCONVERT2LONG(Op1);
    ConditionValue = LONGVALUE(Op1);
  END_IF
  IF (NOT ConditionValue) THEN_DO nItem = CDR(nItem);
  IF (NOT nItem) THEN
    RESULT = NULL;
    RETURN;
  END_IF
  nItem = CDR(nItem);
  RESULT = besEVALUATEEXPRESSION(CAR(nItem));
  ASSERTOKE;
  RETURN;
besEND_COMMAND


/*****
 Core
*****/


besFUNCTION(js_create)
  struct v7 *v7 = v7_create();
  besRETURN_LONG(v7);
besEND

// TODO: v7_create_opt()

besFUNCTION(js_destroy)
  DIM AS unsigned long v7;
  besARGUMENTS("i")
    AT v7
  besARGEND
  v7_destroy(v7);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_get_global)
  DIM AS unsigned long v7;
  DIM AS v7_val_t result;
  besARGUMENTS("i")
    AT v7
  besARGEND
  result = v7_get_global(v7);
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_get_this)
  DIM AS unsigned long v7;
  DIM AS v7_val_t result;
  besARGUMENTS("i")
    AT v7
  besARGEND
  result = v7_get_this(v7);
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_get_arguments)
  DIM AS unsigned long v7;
  DIM AS v7_val_t result;
  besARGUMENTS("i")
    AT v7
  besARGEND
  result = v7_get_arguments(v7);
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_arg)
  DIM AS unsigned long v7, arrayidx;
  DIM AS v7_val_t result;
  besARGUMENTS("ii")
    AT v7, AT arrayidx
  besARGEND
  result = v7_arg(v7, arrayidx);
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_argc)
  DIM AS unsigned long v7, argcnt;
  besARGUMENTS("i")
    AT v7
  besARGEND
  argcnt = v7_argc(v7);
  besRETURN_LONG(argcnt);
besEND

besFUNCTION(js_own)
  DIM AS unsigned long v7;
  DIM AS double argvar;
  DIM AS v7_val_t cvar;
  besARGUMENTS("ir")
    AT v7, AT argvar
  besARGEND
  cvar = d2v(argvar);
  v7_own(v7, &cvar);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_disown)
  DIM AS unsigned long v7;
  DIM AS double argvar;
  DIM AS v7_val_t cvar;
  DIM AS int found;
  besARGUMENTS("ir")
    AT v7, AT argvar
  besARGEND
  cvar = d2v(argvar);
  found = v7_disown(v7, &cvar);
  besRETURN_LONG(found);
besEND

besFUNCTION(js_set_gc_enabled)
  DIM AS unsigned long v7;
  DIM AS int enabled;
  besARGUMENTS("ii")
    AT v7, AT enabled
  besARGEND
  v7_set_gc_enabled(v7, enabled);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_interrupt)
  DIM AS unsigned long v7;
    besARGUMENTS("i")
    AT v7
  besARGEND
  v7_interrupt(v7);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_get_parser_error)
  DIM AS unsigned long v7;
  DIM AS const char *errstr;
  besARGUMENTS("i")
    AT v7
  besARGEND
  errstr = v7_get_parser_error(v7);
  besRETURN_STRING(errstr);
besEND


/***********
 Primitives
***********/


besFUNCTION(js_mk_number)
  DIM AS unsigned long v7;
  DIM AS double numval;
  DIM AS v7_val_t result;
  besARGUMENTS("ir")
    AT v7, AT numval
  besARGEND
  result = v7_mk_number(v7, numval);
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_get_double)
  DIM AS unsigned long v7;
  DIM AS double dblval, cdbl;
  besARGUMENTS("ir")
    AT v7, AT dblval
  besARGEND
  cdbl = v7_get_double(v7, d2v(dblval));
  besRETURN_DOUBLE(cdbl);
besEND

besFUNCTION(js_get_int)
  DIM AS unsigned long v7;
  DIM AS double intval;
  DIM AS int cint;
  besARGUMENTS("ir")
    AT v7, AT intval
  besARGEND
  cint = v7_get_int(v7, d2v(intval));
  besRETURN_LONG(cint);
besEND

besFUNCTION(js_is_number)
  DIM AS double numvar;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT numvar
  besARGEND
  istrue = v7_is_number(d2v(numvar));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_mk_boolean)
  DIM AS unsigned long v7;
  DIM AS v7_val_t boolval;
  DIM AS int istrue;
  besARGUMENTS("ii")
    AT v7, AT istrue
  besARGEND
  boolval = v7_mk_boolean(v7, istrue);
  besRETURN_DOUBLE(v2d(boolval));
besEND

besFUNCTION(js_get_bool)
  DIM AS unsigned long v7;
  DIM AS double boolval;
  DIM AS int truefalse;
  besARGUMENTS("ir")
    AT v7, AT boolval
  besARGEND
  truefalse = v7_get_bool(v7, d2v(boolval));
  besRETURN_LONG(truefalse);
besEND

besFUNCTION(js_is_boolean)
  DIM AS double boolval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT boolval
  besARGEND
  istrue = v7_is_boolean(d2v(boolval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_mk_null)
  DIM AS v7_val_t nullval;
  nullval = V7_NULL;
  besRETURN_DOUBLE(v2d(nullval));
besEND

besFUNCTION(js_is_null)
  DIM AS double nullval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT nullval
  besARGEND
  istrue = v7_is_null(d2v(nullval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_mk_undefined)
  DIM AS v7_val_t undefval;
  undefval = V7_UNDEFINED;
  besRETURN_DOUBLE(v2d(undefval));
besEND

besFUNCTION(js_is_undefined)
  DIM AS double undefval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT undefval
  besARGEND
  istrue = v7_is_undefined(d2v(undefval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_mk_foreign)
  DIM AS unsigned long v7;
  DIM AS v7_val_t forvar;
  DIM AS char *vptr;
  besARGUMENTS("i")
    AT v7
  besARGEND
  forvar = v7_mk_foreign(v7, (void *) vptr);
  besRETURN_DOUBLE(v2d(forvar));
besEND

besFUNCTION(js_get_ptr)
  DIM AS unsigned long v7, cvptr;
  DIM AS double ptrval;
  besARGUMENTS("ir")
    AT v7, AT ptrval
  besARGEND
  cvptr = v7_get_ptr(v7, d2v(ptrval));
  besRETURN_LONG(cvptr);
besEND

besFUNCTION(js_is_foreign)
  DIM AS double forval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT forval
  besARGEND
  istrue = v7_is_foreign(d2v(forval));
  besRETURN_LONG(istrue);
besEND


/********
 Strings
********/


besFUNCTION(js_mk_string)
  DIM AS VARIABLE Argument;
  DIM AS unsigned long v7;
  DIM AS const char *jsstr;
  DIM AS size_t jsstrlen;
  DIM AS int jsstrcopy, i;
  DIM AS v7_val_t strval;
  IF (besARGNR < 4) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 4 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO jsstr = STRINGVALUE(Argument);
    IF (i EQ 3) THEN_DO jsstrlen = (size_t)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO jsstrcopy = (int)LONGVALUE(Argument);
  NEXT
  strval = v7_mk_string(v7, jsstr, jsstrlen, jsstrcopy);
  besRETURN_DOUBLE(v2d(strval));
besEND

besFUNCTION(js_is_string)
  DIM AS double strval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT strval
  besARGEND
  istrue = v7_is_string(d2v(strval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_get_string)
  DIM AS unsigned long v7;
  DIM AS double argval;
  DIM AS v7_val_t strval;
  DIM const char *sbstr;
  DIM size_t *strsize;
  besARGUMENTS("ir")
    AT v7, AT argval
  besARGEND
  strval = d2v(argval);
  sbstr = v7_get_string(v7, &strval, NULL);
  besRETURN_STRING(sbstr);
besEND

besFUNCTION(js_get_cstring)
  DIM AS unsigned long v7;
  DIM AS double argval;
  DIM AS v7_val_t cstrval;
  DIM const char *sbcstr;
  DIM size_t cstrlen;
  besARGUMENTS("iri")
    AT v7, AT argval, AT cstrlen
  besARGEND
  cstrval = d2v(argval);
  sbcstr = v7_get_string(v7, &cstrval, &cstrlen);
  besRETURN_STRING(sbcstr);
besEND


/********
 Objects
********/


besFUNCTION(js_mk_object)
  DIM AS unsigned long v7;
  DIM AS v7_val_t objval;
  besARGUMENTS("i")
    AT v7
  besARGEND
  objval = v7_mk_object(v7);
  besRETURN_DOUBLE(v2d(objval));
besEND

besFUNCTION(js_is_object)
  DIM AS double objval;
  DIM AS int istrue;
  besARGUMENTS("r")
    AT objval
  besARGEND
  istrue = v7_is_object(d2v(objval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_get_proto)
  DIM AS unsigned long v7;
  DIM AS double objval;
  DIM AS v7_val_t protoval;
  besARGUMENTS("ir")
    AT v7, AT objval
  besARGEND
  protoval = v7_get_proto(v7, d2v(objval));
  besRETURN_DOUBLE(v2d(protoval));
besEND

besFUNCTION(js_set_proto)
  DIM AS unsigned long v7;
  DIM AS double objval, protoval;
  DIM AS v7_val_t oldprotoval;
  besARGUMENTS("irr")
    AT v7, AT objval, AT protoval
  besARGEND
  oldprotoval = v7_set_proto(v7, d2v(objval), d2v(protoval));
  besRETURN_DOUBLE(v2d(oldprotoval));
besEND

besFUNCTION(js_get)
  DIM AS unsigned long v7;
  DIM AS v7_val_t objval;
  DIM AS double obj;
  DIM AS const char *name;
  DIM AS size_t name_len;
  besARGUMENTS("irzi")
    AT v7, AT obj, AT name, AT name_len
  besARGEND
  objval = v7_get(v7, d2v(obj), name, name_len);
  besRETURN_DOUBLE(v2d(objval));
besEND

// TODO: v7_get_throwing()

besFUNCTION(js_def)
  DIM AS VARIABLE Argument;
  DIM AS unsigned long v7;
  DIM AS double objval, propval;
  DIM AS const char *objname;
  DIM AS size_t objnamelen;
  DIM AS v7_prop_attr_desc_t attrs_desc;
  DIM AS int rtnstatus, i;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO objval = DOUBLEVALUE(Argument);
    IF (i EQ 3) THEN_DO objname = STRINGVALUE(Argument);
    IF (i EQ 4) THEN_DO objnamelen = (size_t)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO attrs_desc = (v7_prop_attr_desc_t)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO propval = DOUBLEVALUE(Argument);
  NEXT
  rtnstatus = v7_def(v7, d2v(objval), objname, objnamelen, attrs_desc, d2v(propval));
  besRETURN_LONG(rtnstatus);
besEND

besFUNCTION(js_set)
  DIM AS VARIABLE Argument;
  DIM AS unsigned long v7;
  DIM AS double objval, propval;
  DIM AS const char *objname;
  DIM AS size_t objnamelen;
  DIM AS int rtnstatus, i;
  IF (besARGNR < 5) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 5 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO objval = DOUBLEVALUE(Argument);
    IF (i EQ 3) THEN_DO objname = STRINGVALUE(Argument);
    IF (i EQ 4) THEN_DO objnamelen = (size_t)LONGVALUE(Argument);
    IF (i EQ 5) THEN_DO propval = DOUBLEVALUE(Argument);
  NEXT
  rtnstatus = v7_set(v7, d2v(objval), objname, objnamelen, d2v(propval));
  besRETURN_LONG(rtnstatus);
besEND

// TODO: v7_set_method()

besFUNCTION(js_del)
  DIM AS unsigned long v7;

  DIM AS double objval;
  DIM AS const char *name;
  DIM AS size_t name_len;
  DIM AS int rtnstatus;
  besARGUMENTS("irzi")
    AT v7, AT objval, AT name, AT name_len
  besARGEND
  rtnstatus = v7_del(v7, d2v(objval), name, name_len);
  besRETURN_LONG(rtnstatus);
besEND

besFUNCTION(js_init_prop_iter_ctx)
  DIM AS unsigned long v7;
  DIM AS double objval;
  besARGUMENTS("ir")
    AT v7, AT objval
  besARGEND
  v7_init_prop_iter_ctx(v7, d2v(objval), &ctx);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_next_prop)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS v7_val_t propname, propval;
  DIM AS v7_prop_attr_t propattr;
  DIM AS int validprop, i;
  IF (besARGNR < 4) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 4 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO propname = DOUBLEVALUE(Argument);
    IF (i EQ 3) THEN_DO propval = DOUBLEVALUE(Argument);
    IF (i EQ 4) THEN_DO propattr = (v7_prop_attr_t)LONGVALUE(Argument);
  NEXT
  validprop = v7_next_prop(v7, &ctx, &propname, &propval, &propattr);
  IF (validprop NE NULL) THEN
    DEF_FOR (i = 2 TO i <= 4 STEP INCR i)
    BEGIN_FOR
      Argument = besARGUMENT(i);
      besLEFTVALUE(Argument,Lval);
      besRELEASE(*Lval);
      IF (i EQ 2) THEN
        *Lval = besNEWDOUBLE;
      	DOUBLEVALUE(*Lval) = v2d(propname);
      END_IF
      IF (i EQ 3) THEN
        *Lval = besNEWDOUBLE;
      	DOUBLEVALUE(*Lval) = v2d(propval);
      END_IF
      IF (i EQ 4) THEN
        *Lval = besNEWLONG;
      	LONGVALUE(*Lval) = propattr;
      END_IF
    NEXT
  besRETURN_LONG(-1);
  ELSE
    besRETURNVALUE = NULL;
  END_IF
besEND

besFUNCTION(js_destruct_prop_iter_ctx)
  DIM AS unsigned long v7;
  besARGUMENTS("i")
    AT v7
  besARGEND
  v7_destruct_prop_iter_ctx(v7, &ctx);
  besRETURNVALUE = NULL;
besEND

besFUNCTION(js_is_instanceof)
  DIM AS unsigned long v7;
  DIM AS double object;
  DIM AS const char *classname;
  DIM AS int rtnstatus;
  besARGUMENTS("irz")
    AT v7, AT object, AT classname
  besARGEND
  rtnstatus = v7_is_instanceof(v7, d2v(object), classname);
  besRETURN_LONG(rtnstatus);
besEND

besFUNCTION(js_is_instanceof_v)
  DIM AS unsigned long v7;
  DIM AS double object, objclass;
  DIM AS int rtnstatus;
  besARGUMENTS("irr")
    AT v7, AT object, AT objclass
  besARGEND
  rtnstatus = v7_is_instanceof_v(v7, d2v(object), d2v(objclass));
  besRETURN_LONG(rtnstatus);
besEND

// TODO: v7_set_user_data()

// TODO: v7_get_user_data()

// TODO: v7_set_destructor_cb()


/*******
 Arrays
*******/


besFUNCTION(js_mk_array)
  DIM AS unsigned long v7;
  DIM AS double arrayobj;
  besARGUMENTS("i")
    AT v7
  besARGEND
  arrayobj = v7_mk_array(v7);
  besRETURN_DOUBLE(v2d(arrayobj));
besEND

besFUNCTION(js_is_array)
  DIM AS unsigned long v7;
  DIM AS double arrayval;
  DIM AS int istrue;
  besARGUMENTS("ir")
    AT v7, AT arrayval
  besARGEND
  istrue = v7_is_array(v7, d2v(arrayval));
  besRETURN_LONG(istrue);
besEND

besFUNCTION(js_array_length)
  DIM AS unsigned long v7, arraylen;
  DIM AS double arrayval;
  besARGUMENTS("ir")
    AT v7, AT arrayval
  besARGEND
  arraylen = v7_array_length(v7, d2v(arrayval));
  besRETURN_LONG(arraylen);
besEND

besFUNCTION(js_array_push)
  DIM AS unsigned long v7;
  DIM AS double arrayobj, arrayval;
  DIM AS int rtncode;
  besARGUMENTS("irr")
    AT v7, AT arrayobj, AT arrayval
  besARGEND
  rtncode = v7_array_push(v7, d2v(arrayobj), d2v(arrayval));
  besRETURN_LONG(rtncode);
besEND

// TODO: v7_array_push_throwing()

besFUNCTION(js_array_get)
  DIM AS unsigned long v7, index;
  DIM AS double arr;
  DIM AS v7_val_t arrmem;
  besARGUMENTS("iri")
    AT v7, AT arr, AT index
  besARGEND
  arrmem = v7_array_get(v7, d2v(arr), index);
  besRETURN_DOUBLE(v2d(arrmem));
besEND

besFUNCTION(js_array_set)
  DIM AS unsigned long v7, index;
  DIM AS double arr, val;
  DIM AS int rtncode;
  besARGUMENTS("irir")
    AT v7, AT arr, AT index, AT val
  besARGEND
  rtncode = v7_array_set(v7, d2v(arr), index, d2v(val));
  besRETURN_LONG(rtncode);
besEND

// TODO: v7_array_set_throwing()

besFUNCTION(js_array_del)
  DIM AS unsigned long v7, index;
  DIM AS double arr;
  besARGUMENTS("iri")
    AT v7, AT arr, AT index
  besARGEND
  v7_array_del(v7, d2v(arr), index);
  besRETURNVALUE = NULL;
besEND


/**********
 Functions
**********/


// TODO: v7_mk_function()

// TODO: v7_mk_function_with_proto()

// TODO: v7_mk_cfunction()

// TODO: v7_is_callable()


/*******
 RegExp
*******/


besFUNCTION(js_mk_regexp)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS const char *regex, *flags;
  DIM AS size_t regex_len, flags_len;
  DIM AS v7_val_t resultval;
  DIM AS int i;
  enum v7_err rcode = V7_OK;
  IF (besARGNR < 6) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 6 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO regex = STRINGVALUE(Argument);
    IF (i EQ 3) THEN_DO regex_len = (size_t)LONGVALUE(Argument);
    IF (i EQ 4) THEN_DO flags = STRINGVALUE(Argument);
    IF (i EQ 5) THEN_DO flags_len = (size_t)LONGVALUE(Argument);
    IF (i EQ 6) THEN_DO rcode = (int)LONGVALUE(Argument);
  NEXT
  rcode = v7_mk_regexp(v7, regex, regex_len, flags, flags_len, &resultval);
  Argument = besARGUMENT(6);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rcode;
  besRETURN_DOUBLE(v2d(resultval));
besEND

besFUNCTION(js_is_regexp)
  DIM AS unsigned long v7;
  DIM AS double regex;
  DIM AS int istrue;
  besARGUMENTS("ir")
    AT v7, AT regex
  besARGEND
  istrue = v7_is_regexp(v7, d2v(regex));
  besRETURN_LONG(istrue);
besEND


/***********
 Conversion
***********/


besFUNCTION(js_stringify)
  DIM AS VARIABLE Argument;
  DIM AS unsigned long v7;
  DIM AS double object;
//  DIM AS char buf[2048];
  DIM AS char buf[1024];
  DIM AS char *resultstr;
  DIM AS int mode, i;
  IF (besARGNR < 3) THEN_DO RETURN_FUNCTION(EX_ERROR_TOO_FEW_ARGUMENTS);
  DEF_FOR (i = 1 TO i <= 3 STEP INCR i)
  BEGIN_FOR
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    IF (i EQ 1) THEN_DO v7 = (unsigned long)LONGVALUE(Argument);
    IF (i EQ 2) THEN_DO object = DOUBLEVALUE(Argument);
    IF (i EQ 3) THEN_DO mode = LONGVALUE(Argument);
  NEXT
  resultstr = v7_stringify(v7, d2v(object), buf, sizeof(buf), mode);
  besRETURN_STRING(resultstr);
  IF(resultstr NE buf) THEN
    free(resultstr);
  END_IF
besEND

// TODO: v7_stringify_throwing()

// TODO: v7_to_json()

// TODO: v7_is_truthy()


/**********
 Execution
**********/


besFUNCTION(js_exec)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS const char *js_code;
  DIM AS v7_val_t resultval;
  enum v7_err rcode = V7_OK;
  besARGUMENTS("iz")
    AT v7, AT js_code
  besARGEND
  rcode = v7_exec(v7, js_code, &resultval);
  Argument = besARGUMENT(3);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rcode;
  besRETURN_DOUBLE(v2d(resultval));
besEND

// TODO: v7_exec_opt()

besFUNCTION(js_exec_file)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS const char *js_code_file;
  DIM AS v7_val_t resultval;
  enum v7_err rcode = V7_OK;
  besARGUMENTS("iz")
    AT v7, AT js_code_file
  besARGEND
  rcode = v7_exec_file(v7, js_code_file, &resultval);
  Argument = besARGUMENT(3);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rcode;
  besRETURN_DOUBLE(v2d(resultval));
besEND

besFUNCTION(js_parse_json)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS const char *json_code;
  DIM AS v7_val_t resultval;
  enum v7_err rcode = V7_OK;
  besARGUMENTS("iz")
    AT v7, AT json_code
  besARGEND
  rcode = v7_parse_json(v7, json_code, &resultval);
  Argument = besARGUMENT(3);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rcode;
  besRETURN_DOUBLE(v2d(resultval));
besEND

besFUNCTION(js_parse_json_file)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS const char *json_code_file;
  DIM AS v7_val_t resultval;
  enum v7_err rcode = V7_OK;
  besARGUMENTS("iz")
    AT v7, AT json_code_file
  besARGEND
  rcode = v7_parse_json_file(v7, json_code_file, &resultval);
  Argument = besARGUMENT(3);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rcode;
  besRETURN_DOUBLE(v2d(resultval));
besEND

// TODO: v7_compile()

besFUNCTION(js_apply)
  VARIABLE Argument;
  LEFTVALUE Lval;
  unsigned long __refcount_;
  DIM AS unsigned long v7;
  DIM AS v7_val_t funcval, objval, argsval, result;
  DIM AS double _funcval, _objval, _argsval;
  enum v7_err rtncode = V7_OK;
  besARGUMENTS("irrr")
    AT v7, AT _funcval, AT _objval, AT _argsval
  besARGEND
  funcval = d2v(_funcval);
  objval = d2v(_objval);
  argsval = d2v(_argsval);
  IF (objval == 0) THEN
    rtncode = v7_apply(v7, funcval, V7_UNDEFINED, argsval, &result);
  ELSE
    rtncode = v7_apply(v7, funcval, objval, argsval, &result);
  END_IF
  Argument = besARGUMENT(5);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = besNEWLONG;
  LONGVALUE(*Lval) = rtncode;
  besRETURN_DOUBLE(v2d(result));
besEND

besFUNCTION(js_println)
  DIM AS unsigned long v7;
  DIM AS double objval;
  besARGUMENTS("ir")
    AT v7, AT objval
  besARGEND
  v7_println(v7, d2v(objval));
  besRETURNVALUE = NULL;
besEND
