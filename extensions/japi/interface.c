/*
READ THIS FILE AND CHANGE THE SOURCE WHEREVER YOU SEE COMMENTS STARTING
WITH THE WORD *TODO*

WHEN YOU ARE FINISHED YOU CAN 

  FILE   : interface.c
  HEADER : interface.h
  BAS    : japi.bas
  AUTHOR : *TODO*

  DATE: 

  CONTENT:
  This is the interface.c file for the ScriptBasic module japi
----------------------------------------------------------------------------

Remove the two characters // from following line if this module is supposed to
be compiled under Windows. If there is a need for some library to successfully
compile the module under Windows specify the names of the libraries on the line
as it is listed for the linker application. This is usually something like

libname1.lib libname2.lib ... libnameX.lib

If there are no libraries, but still the module is to be compiled under Windows
do remove the // characters so that the program setup.pl will know that the
module is meant for Windows.
//NTLIBS:
----------------------------------------------------------------------------

Remove the two characters // from following line if this module is supposed to
be compiled under UNIX. If there is a need for some library to successfully
compile the module under UNIX specify the names of the libraries on the line
as it is listed for the linker application. This is usually something like

-lm -ldl -la

If there are no libraries, but still the module is to be compiled under UNIX
do remove the // characters so that the program setup.pl will know that the
module is meant for UNIX.

//UXLIBS:
----------------------------------------------------------------------------

Remove the two characters // from following line if this module is supposed to
be compiled under MacOS. If there is a need for some library to successfully
compile the module under MacOS specify the names of the libraries on the line
as it is listed for the linker application.
If there are no libraries, but still the module is to be compiled under MacOS
do remove the // characters so that the program setup.pl will know that the
module is meant for MacOS.

//MCLIBS:
----------------------------------------------------------------------------

Remove the two characters // from following line if this module is supposed to
be compiled under Darwin. If there is a need for some library to successfully
compile the module under Darwin specify the names of the libraries on the line
as it is listed for the linker application.
If there are no libraries, but still the module is to be compiled under Darwin
do remove the // characters so that the program setup.pl will know that the
module is meant for Darwin.

//DWLIBS:

*/

/*
*TODO*
INCLUDE HERE THE SYSTEM HEADER FILES THAT ARE NEEDED TO COMPILE THIS MODULE
*/

#ifdef WIN32
/*
*TODO*
INCLUDE HERE THE WIN32 SPECIFIC HEADER FILES THAT ARE NEEDED TO COMPILE THIS MODULE
*/
#else
/*
*TODO*
INCLUDE HERE THE UNIX SPECIFIC HEADER FILES THAT ARE NEEDED TO COMPILE THIS MODULE
*/
#endif

/*
*TODO*
INCLUDE HERE THE LOCAL HEADER FILES THAT ARE NEEDED TO COMPILE THIS MODULE
*/
#include <stdio.h>
#include "../../basext.h"

/*
*TODO*
INSERT THE BASIC CODE THAT WILL GET INTO THE FILE japi.BAS
AFTER THE LINE 'TO_BAS:' AND BEFORE THE LINE END OF THE COMMENT

NOTE THAT SUB AND COMMAND DECLARATIONS ARE CREATED AUTOMATICALLY
FROM THE FUNCTION DEFINTIONS WHEN THE MODULE IS CONFIGURED BEFORE
COMPILATION

TO_BAS:
*/

/*
*TODO*
DECLARE HERE THE MODULE OBJECT TYPE. THIS STRUCTURE SHOULD HOLD THE
DATA AVAILABLE FOR EACH INTERPRETER THREAD. USE THIS STRUCTURE TO
STORE GLOBAL VALUES INSTEAD OF USING GLOBAL VARIABLES.
*/
typedef struct _ModuleObject {
  char a; /* You may delete this. It is here to make the initial interface.c compilable. */
  }ModuleObject,*pModuleObject;


/*
*TODO*
ALTER THE VERSION NEGOTIATION CODE IF YOU NEED
*/
besVERSION_NEGOTIATE
  return (int)INTERFACE_VERSION;
besEND

/*
*TODO*
ALTER THE ERROR MESSAGE FUNCTION
*/
besSUB_ERRMSG

  switch( iError ){
    case 0x00080000: return "ERROR HAS HAPPENED";
    }
  return "Unknown japi module error.";
besEND

/*
*TODO*
ALTER THE MODULE INITIALIZATION CODE
*/
besSUB_START
  pModuleObject p;

  besMODULEPOINTER = besALLOC(sizeof(ModuleObject));
  if( besMODULEPOINTER == NULL )return 0;

/*
*TODO*
INSERT HERE ANY CODE THAT YOU NEED TO INITIALIZE THE MODULE FOR THE
ACTUAL INTERPRETER THREAD
*/

besEND

/*
*TODO*
ALTER THE MODULE FINISH CODE IF NEEDED
*/
besSUB_FINISH
  pModuleObject p;

  /*
    YOU CERTAINLY NEED THIS POINTER TO FREE ALL RESOURCES THAT YOU ALLOCATED
    YOU NEED NOT CALL besFREE TO FREE THE MEMORY ALLOCATED USING besALLOC
    CLOSE ANY FILE THAT REMAINED OPEN, RELEASE DATABASE HANDLES AND ALL
    OTHER RESOURCES INCLUDING MEMORY *NOT* ALLOCATED CALLING besALLOC
  */
  p = (pModuleObject)besMODULEPOINTER;
  if( p == NULL )return 0;

  return 0;
besEND


/*
*TODO*
WRITE YOUR MODULE INTERFACE FUNCTIONS FOLLOWING THIS SKELETON

NOTE THAT THIS IS A SAMPLE FUNCTION, YOU CAN ALSO DELETE
LINES FROM IT IF NEEDED
*/
/**
=section functionname
=H title that goes into the BASIC documentation for this function

detail here what the function does so that the BASIC programmer
can understand how he/she can use it
*/
besFUNCTION(functionname)
  pModuleObject p;
  VARIABLE Argument;

  p = (pModuleObject)besMODULEPOINTER;

  Argument = besARGUMENT(1);
  besDEREFERENCE(Argument);

  /* if argument is undef then return undef */
  if( Argument == NULL ){
    besRETURNVALUE = NULL;
    return COMMAND_ERROR_SUCCESS;
    }

  return COMMAND_ERROR_SUCCESS;
besEND

/*
*TODO*
INSERT HERE THE NAME OF THE FUNCTION AND THE FUNCTION INTO THE
TABLE. THIS TABLE IS USED TO FIND THE FUNCTIONS WHEN THE MODULE
INTERFACE FILE IS COMPILED TO BE LINKED STATIC INTO A VARIATION
OF THE INTERPRETER.
*/

SLFST JAPI_SLFST[] ={

{ "versmodu" , versmodu },
{ "bootmodu" , bootmodu },
{ "finimodu" , finimodu },
{ "emsgmodu" , emsgmodu },
{ "functionname" , functionname },
{ NULL , NULL }
  };
