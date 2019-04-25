/***********************
 Portable User Interface
***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iup.h>
#include <iupcontrols.h>
#include "../../basext.h"


typedef struct _ModuleObject {
  void *HandleArray;
}ModuleObject,*pModuleObject;

#define ELEMENTS(x) (sizeof (x) / sizeof *(x))

Ihandle *evtHandle = 1;
int btnPressed, btnState;
int listPos, listIndex;

char listSelectedText[4096];


/*******************
 CallBack Processing
*******************/


int Event(Ihandle *object) {
  evtHandle = object;
  return IUP_DEFAULT;
}


int BtnEvent(Ihandle *object, int WhichButton, int BtnState) {
  evtHandle = object;
  btnPressed = WhichButton;
  btnState = BtnState;
  return IUP_DEFAULT;
}


int WinEvent(Ihandle *object) {
	evtHandle = object;
	return IUP_CLOSE;
}


int ListEvent (Ihandle *ih, char *text, int pos, int selected) {
	evtHandle = ih;
	listPos=pos;
	listIndex=selected;
	strcpy(listSelectedText, text);
	return IUP_DEFAULT;
}


besFUNCTION(SB_GetEvent)
  Ihandle *iTmp = evtHandle;
  evtHandle = NULL;
  besRETURN_POINTER(iTmp)
besEND


besFUNCTION(SB_GetBtnPressed)
  besRETURN_LONG(btnPressed);
  btnPressed = NULL;
besEND


besFUNCTION(SB_GetBtnState)
  besRETURN_LONG(btnState);
  btnState = NULL;
besEND


besFUNCTION(SB_GetListPos)
  besRETURN_LONG(listPos);
  listPos = NULL;
besEND


besFUNCTION(SB_GetListIndex)
  besRETURN_LONG(listIndex);
  listIndex = NULL;
besEND


besFUNCTION(SB_GetListText)
  besRETURN_STRING(listSelectedText);
besEND


/**************************
 Extension Module Functions
**************************/


besVERSION_NEGOTIATE
  return (int)INTERFACE_VERSION;
besEND


besSUB_START
  pModuleObject p;

  besMODULEPOINTER = besALLOC(sizeof(ModuleObject));
  if( besMODULEPOINTER == NULL )return 0;

  p = (pModuleObject)besMODULEPOINTER;
  return 0;
besEND


besSUB_FINISH
  pModuleObject p;

  p = (pModuleObject)besMODULEPOINTER;
  if( p == NULL )return 0;
  return 0;
besEND


/*********************
 IUP System Functions
*********************/


/*
int IupOpen(int *argc, char ***argv);  <iup.h>

Initializes the IUP toolkit. Must be called before any other IUP function.
*/
besFUNCTION(PuiOpen)
  besRETURN_LONG(IupOpen(0,0));
besEND

besFUNCTION(PuiImageLibOpen)
  IupImageLibOpen();
besEND

/*
void IupClose(void);  <iup.h>

Ends the IUP toolkit and releases internal memory.
*/
besFUNCTION(PuiClose)
  IupClose();
besEND


/*
char* IupVersion(void);  <iup.h>

Returns a string with the IUP version number.
*/
besFUNCTION(PuiVersion)
  besRETURN_STRING(IupVersion());
besEND


/*
char *IupLoad(const char *filename);  <iup.h>

Compiles a LED specification.
*/
besFUNCTION(PuiLoad)
  const char *filename;

  besARGUMENTS("z")
    &filename
  besARGEND

  besRETURN_STRING(IupLoad(filename));
besEND


/*
char *IupLoadBuffer(const char *buffer);

See above - load from buffer.
*/
besFUNCTION(PuiLoadBuffer)
  const char *buffer;

  besARGUMENTS("z")
    &buffer
  besARGEND

  besRETURN_STRING(IupLoadBuffer(buffer));
besEND

/*
void IupSetLanguage(const char *lng);  <iup.h>

Defines the language used by some pre-defined dialogs.
*/
besFUNCTION(PuiSetLanguage)
  const char *lng;

  besARGUMENTS("z")
    &lng
  besARGEND

  IupSetLanguage(lng);
besEND


/*
char* IupGetLanguage(void);  <iup.h>

Returns the global attribute LANGUAGE.
*/
besFUNCTION(PuiGetLanguage)
  besRETURN_STRING(IupGetLanguage());
besEND


/***********************
 IUP Attribute Functions
***********************/


/*
void IupStoreAttribute(Ihandle *ih, const char *name, const char *value);  <iup.h>

Defines an attribute for an interface element but the string is internally
duplicated.
*/
besFUNCTION(PuiStoreAttribute)
  Ihandle *ih;
  const char *name, *value;

  besARGUMENTS("pz[z]")
    &ih, &name, &value
  besARGEND

  IupStoreAttribute(ih, name, value);
besEND


besFUNCTION(PuiStoreGlobalAttribute)
  Ihandle *ih;
  const char *name, *value;

  besARGUMENTS("z[z]")
    &name, &value
  besARGEND

  ih = NULL;
  IupStoreAttribute(ih, name, value);
besEND

/*
void IupStoreAttributeId(Ihandle *ih, const char *name, int id, const char *value);  <iup.h>

See above - used when the attribute has an additional id.
*/
besFUNCTION(PuiStoreAttributeId)
  Ihandle *ih;
  const char *name, *value;
  int id;

  besARGUMENTS("pzi[z]")
    &ih, &name, &id, &value
  besARGEND

  IupStoreAttributeId(ih, name, id, value);
besEND


besFUNCTION(PuiStoreGlobalAttributeId)
  Ihandle *ih;
  const char *name, *value;
  int id;

  besARGUMENTS("zi[z]")
    &name, &id, &value
  besARGEND

  ih = NULL;
  IupStoreAttributeId(ih, name, id, value);
besEND


/*
void IupSetAttribute(Ihandle *ih, const char *name, const char *value);  <iup.h>

Defines an attribute for an interface element.
*/
besFUNCTION(PuiSetAttribute)
  Ihandle *ih;
  const char *name, *value;

  besARGUMENTS("pz[z]")
    &ih, &name, &value
  besARGEND

  IupSetAttribute(ih, name, value);
besEND


besFUNCTION(PuiSetGlobalAttribute)
  Ihandle *ih;
  const char *name, *value;

  besARGUMENTS("z[z]")
    &name, &value
  besARGEND

  ih = NULL;
  IupSetAttribute(ih, name, value);
besEND


/*
void IupSetAttributeId(Ihandle *ih, const char *name, int id, const char *value);  <iup.h>

See above - id: used when the attribute has an additional id.
*/
besFUNCTION(PuiSetAttributeId)
  Ihandle *ih;
  const char *name, *value;
  int id;

  besARGUMENTS("pzi[z]")
    &ih, &name, &id, &value
  besARGEND

  IupSetAttributeId(ih, name, id, value);
besEND


besFUNCTION(PuiSetGlobalAttributeId)
  Ihandle *ih;
  const char *name, *value;
  int id;

  besARGUMENTS("zi[z]")
    &name, &id, &value
  besARGEND

  ih = NULL;
  IupSetAttributeId(ih, name, id, value);
besEND


/*
void IupSetfAttribute(Ihandle *ih, const char *name, const char *format, ...);  <iup.h>

Defines an attribute for an interface element.
*/
besFUNCTION(PuiSetfAttribute)
  Ihandle *ih;
  const char *name, *format;

  besARGUMENTS("pzz")
    &ih, &name, &format
  besARGEND

  IupSetfAttribute(ih, name, format);
besEND


/*
void IupSetfAttributeId(Ihandle *ih, const char *name, int id, const char *format, ...);  <iup.h>

See above - used when the attribute has additional ids.
*/
besFUNCTION(PuiSetfAttributeId)
  Ihandle *ih;
  const char *name, *format;
  int id;

  besARGUMENTS("pziz")
    &ih, &name, &id, &format
  besARGEND

  IupSetfAttributeId(ih, name, id, format);
besEND



/*
void IupSetfAttributeId2(Ihandle* ih, const char* name, int lin, int col, const char* format, ...);  <iup.h>

See above - used when the attribute has line / column arguments.
*/
besFUNCTION(PuiSetfAttributeId2)
  Ihandle *ih;
  const char *name, *format;
  int lin, col;

  besARGUMENTS("pziiz")
    &ih, &name, &lin, &col, &format
  besARGEND

  IupSetfAttributeId2(ih, name, lin, col, format);
besEND


/*
Ihandle *IupSetAttributes(Ihandle *ih, const char *str);  <iup.h>

Defines a set of attributes for an interface element.
*/
besFUNCTION(PuiSetAttributes)
  Ihandle *ih, *sameih;
  const char *attstr;

  besARGUMENTS("pz")
    &ih, &attstr
  besARGEND

  sameih = IupSetAttributes(ih, attstr);
  besRETURN_POINTER(sameih);
besEND


/*
void IupResetAttribute(Ihandle *ih, const char *name);  <iup.h>

Removes an attribute from the hash table of the element, and its children if
the attribute is inheritable. It is useful to reset the state of inheritable
attributes in a tree of elements.
*/
besFUNCTION(PuiResetAttribute)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  IupResetAttribute(ih, name);
besEND


besFUNCTION(PuiResetGlobalAttribute)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("z")
    &name
  besARGEND

  ih = NULL;
  IupResetAttribute(ih, name);
besEND


/*
Ihandle* IupSetAtt(const char* handle_name, Ihandle* ih, const char* name, ...);  <iup.h>

Defines a set of attributes for an interface element and optionally sets its handle.
*/
besFUNCTION(PuiSetAtt)
  VARIABLE Argument;
  Ihandle *ih;
  const char *handle_name;
  char *attr, *val;
  int i;

 // if ((unsigned)besARGNR % 2 > 0) return EX_ERROR_TOO_FEW_ARGUMENTS;

  besARGUMENTS("zp")
    &handle_name, &ih
  besARGEND

  if (handle_name != "") IupSetHandle(handle_name, ih);

  for( i=3 ; i <= (unsigned)besARGNR ; i += 2){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    besCONVERT2ZCHAR(Argument,attr);

    Argument = besARGUMENT(i + 1);
    besDEREFERENCE(Argument);
    besCONVERT2ZCHAR(Argument,val);

    IupSetAttribute(ih, attr, val);
    }

  besRETURN_POINTER(ih);
besEND


/*
void IupSetAttributeHandle(Ihandle *ih, const char *name, Ihandle *ih_named);  <iup.h>

Instead of using IupSetHandle and IupSetAttribute with a new creative name,
this function automatically creates a non conflict name and associates the
name with the attribute. It is very usefull for associating images and menus.
*/
besFUNCTION(PuiSetAttributeHandle)
  Ihandle *ih, *ih_named;
  const char *name;

  besARGUMENTS("pzp")
    &ih, &name, &ih_named
  besARGEND

  IupSetAttributeHandle(ih, name, ih_named);
besEND


/*
Ihandle* IupGetAttributeHandle(Ihandle *ih, const char *name);  <iup.h>

Instead of using IupGetAttribute and IupGetHandle, this function directly
returns the associated handle.
*/
besFUNCTION(PuiGetAttributeHandle)
  Ihandle *ih, *ih_assoc;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  ih_assoc = IupGetAttributeHandle(ih, name);
  besRETURN_POINTER(ih_assoc);
besEND


/*
char *IupGetAttribute(Ihandle *ih, const char *name);  <iup.h>

Returns the name of an interface element attribute.
*/
besFUNCTION(PuiGetAttribute)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  besRETURN_STRING(IupGetAttribute(ih, name));
besEND


besFUNCTION(PuiGetGlobalAttribute)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("z")
    &name
  besARGEND

  ih = NULL;
  besRETURN_STRING(IupGetAttribute(ih, name));
besEND


/*
char *IupGetAttributeId(Ihandle *ih, const char *name, int id);  <iup.h>

See above - used when the attribute has an additional id.
*/
besFUNCTION(PuiGetAttributeId)
  Ihandle *ih;
  const char *name;
  int id;

  besARGUMENTS("pzi")
    &ih, &name, &id
  besARGEND

  besRETURN_STRING(IupGetAttributeId(ih, name, id));
besEND


besFUNCTION(PuiGetGlobalAttributeId)
  Ihandle *ih;
  const char *name;
  int id;

  besARGUMENTS("zi")
    &name, &id
  besARGEND

  ih = NULL;
  besRETURN_STRING(IupGetAttributeId(ih, name, id));
besEND


/*
int IupGetAllAttributes(Ihandle* ih, char** names, int max_n);  <iup.h>

Returns the names of all attributes of an element that are defined in its
internal hash table only.
*/
besFUNCTION(PuiGetAllAttributes)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  besARGUMENTS("p")
    &ih
  besARGEND

  Argument = besARGUMENT(2);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  Argument = besARGUMENT(3);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  if (!max_n) max_n = IupGetAllAttributes(ih, names, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetAllAttributes(ih, names, max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)), names[i], strlen(names[i]));
    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
char* IupGetAttributes (Ihandle *ih);  <iup.h>

Returns all attributes of a given element that are in the internal hash table.
*/
besFUNCTION(PuiGetAttributes)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_STRING(IupGetAttributes (ih));
besEND


/*
float IupGetFloat(Ihandle *ih, const char *name);  <iup.h>

Returns the value of an interface element attribute as a floating point number.
*/
besFUNCTION(PuiGetFloat)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  besRETURN_DOUBLE(IupGetFloat(ih, name));
besEND


/*
float IupGetFloatId(Ihandle *ih, const char *name, int id);  <iup.h>

See above - used when the attribute has additional ids.
*/
besFUNCTION(PuiGetFloatId)
  Ihandle *ih;
  const char *name;
  int id;

  besARGUMENTS("pzi")
    &ih, &name, &id
  besARGEND

  besRETURN_DOUBLE(IupGetFloatId(ih, name, id));
besEND


/*
float IupGetFloatId2(Ihandle* ih, const char* name, int lin, int col);  <iup.h>

See above - used when the attribute has additional line/column properties.
*/
besFUNCTION(PuiGetFloatId2)
  Ihandle *ih;
  const char *name;
  int lin, col;

  besARGUMENTS("pzii")
    &ih, &name, &lin, &col
  besARGEND

  besRETURN_DOUBLE(IupGetFloatId2(ih, name, lin, col));
besEND


/*
int IupGetInt(Ihandle *ih, const char *name);  <iup.h>

Returns the value of an interface element attribute as an integer. (first)
*/
besFUNCTION(PuiGetInt)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  besRETURN_LONG(IupGetInt(ih, name));
besEND


/*
int IupGetInt2(Ihandle *ih, const char *name);  <iup.h>

Returns the value of an interface element attribute as an integer. (second)
*/
besFUNCTION(PuiGetInt2)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  besRETURN_LONG(IupGetInt2(ih, name));
besEND


/*
int IupGetIntInt(Ihandle *ih, const char *name, int *i1, int *i2);  <iup.h>

See above - returns the number of integers found.
*/
besFUNCTION(PuiGetIntInt)

//TODO: Needs LVal return on i1 & i2

besEND


/*
int IupGetIntId(Ihandle *ih, const char *name, int id);  <iup.h>

See above - used when the attribute has additional ids.
*/
besFUNCTION(PuiGetIntId)
  Ihandle *ih;
  const char *name;
  int id;

  besARGUMENTS("pzi")
    &ih, &name, &id
  besARGEND

  besRETURN_LONG(IupGetIntId(ih, name, id));
besEND


/*
int IupGetIntId2(Ihandle* ih, const char* name, int lin, int col);  <iup.h>

See above - used when the attribute has additional line/column properties.
*/
besFUNCTION(PuiGetIntId2)
  Ihandle *ih;
  const char *name;
  int lin, col;

  besARGUMENTS("pzii")
    &ih, &name, &lin, &col
  besARGEND

  besRETURN_LONG(IupGetIntId2(ih, name, lin, col));
besEND


/*
void IupStoreGlobal(const char *name, const char *value);  <iup.h>

Defines an attribute for the global environment but the string is internally duplicated. 
*/
besFUNCTION(PuiStoreGlobal)
  const char *name, *value;

  besARGUMENTS("z[z]")
    &name, &value
  besARGEND

  IupStoreGlobal(name, value);
besEND


/*
void IupSetGlobal(const char *name, const char *value);  <iup.h>

Defines an attribute for the global environment. If the driver process the
attribute then it will not be stored internally.
*/
besFUNCTION(PuiSetGlobal)
  const char *name, *value;

  besARGUMENTS("z[z]")
    &name, &value
  besARGEND

  IupSetGlobal(name, value);
besEND


/*
char *IupGetGlobal(const char *name);  <iup.h>

Returns an attribute value from the global environment. The value can be
returned from the driver or from the internal storage.
*/
besFUNCTION(PuiGetGlobal)
  const char *name;

  besARGUMENTS("z")
    &name
  besARGEND

  besRETURN_STRING(IupGetGlobal(name));
besEND


/*******************
 IUP Event Functions
*******************/


/*
int IupMainLoop(void);  <iup.h>

Executes the user interaction until a callback returns IUP_CLOSE, IupExitLoop
is called, or hiding the last visible dialog.
*/
besFUNCTION(PuiMainLoop)

  besRETURN_LONG(IupMainLoop());
besEND


/*
int IupMainLoopLevel(void);  <iup.h>

Returns the current cascade level of IupMainLoop. When no calls were done, return value is 0.
*/
besFUNCTION(PuiMainLoopLevel)

  besRETURN_LONG(IupMainLoopLevel());
besEND


/*
int IupLoopStep(void);  <iup.h>

Runs one iteration of the message loop.
*/
besFUNCTION(PuiLoopStep)

retry_event:
	IupLoopStep();
  if (evtHandle == NULL){
  	 usleep(50);
  	 goto retry_event;
  }	 
  besRETURN_LONG(IUP_DEFAULT);
besEND


/*
int IupLoopStepWait(void);  <iup.h>

See above - put the system in idle until a message is processed.
*/
besFUNCTION(PuiLoopStepWait)

retry_event:
  IupLoopStepWait();
  if (evtHandle == NULL) goto retry_event;
  besRETURN_LONG(IUP_DEFAULT);
besEND


/*
void IupExitLoop(void);  <iup.h>

Terminates the current message loop. It has the same effect of a callback
returning IUP_CLOSE.
*/
besFUNCTION(PuiExitLoop)
  IupExitLoop();
besEND


/*
void IupFlush(void);  <iup.h>

Processes all pending messages in the message queue.
*/
besFUNCTION(PuiFlush)
  IupFlush();
besEND


/*
Icallback IupGetCallback(Ihandle* ih, const char *name);  <iup.h>

Returns the callback associated to an event.

TODO: SB script function interface needed
*/
besFUNCTION(PuiGetCallback)
  Icallback  *ic;
  Ihandle *ih;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  ic = IupGetCallback(ih, name);
  besRETURN_POINTER(ic);
besEND


/*
Icallback IupSetCallback(Ihandle* ih, const char *name, Icallback func);  <iup.h>

Associates a callback to an event.

TODO: Allow function name to be NULL clearing callback for both IUP and SB event{} array.
*/
besFUNCTION(PuiSetCallback)
  Ihandle *ih;
  const char *name;
  char *class_name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  class_name = IupGetClassName(ih);

  if (strcmp(class_name, "dialog") == 0) {
/*  IupSetCallback(ih, name, (Icallback)&WinEvent); */
    IupSetFunction(name, (Icallback)&WinEvent);
    IupSetAttribute(ih, name, name);
    }
  else if (strcmp(class_name, "button") == 0) {
/*  IupSetCallback(ih, name, (Icallback)&BtnEvent); */
    IupSetFunction(name, (Icallback)&BtnEvent);
    IupSetAttribute(ih, name, name);
    }
  else if (strcmp(class_name, "list") == 0) {
/*  IupSetCallback(ih, name, (Icallback)&ListEvent); */
    IupSetFunction(name, (Icallback)&ListEvent);
    IupSetAttribute(ih, name, name);
    }
  else {
/*  IupSetCallback(ih, name, (Icallback)&Event);  */
    IupSetFunction(name, (Icallback)&Event);
    IupSetAttribute(ih, name, name);
    }
  besRETURN_STRING(class_name);
besEND


/*
Ihandle* IupSetCallbacks(Ihandle* ih, const char *name, Icallback func, ...);  <iup.h>

Associates several callbacks to an event.
*/
besFUNCTION(PuiSetCallbacks)

//TODO: Getting a single SB callback working is the focus at this point

besEND


/*
const char* IupGetActionName(void);  <iup.h>

Should return the name of the action being executed by the application.
*/
besFUNCTION(PuiGetActionName)

  besRETURN_STRING(IupGetActionName());
besEND


/*
Icallback IupSetFunction(const char *name, Icallback func);  <iup.h>

Associates a function to an action.
*/
besFUNCTION(PuiSetFunction)
  const char *name;
  Icallback ic;

  besARGUMENTS("z")
    &name
  besARGEND

  ic = IupSetFunction(name, NULL);
  besRETURN_POINTER(ic);
besEND


/*
int IupRecordInput(const char *filename, int mode);  <iup.h>

Records all mouse and keyboard input in a file for later reproduction.
*/
besFUNCTION(PuiRecordInput)
  const char *filename;
  int mode;

  besARGUMENTS("[z][i]")
    &filename, &mode
  besARGEND

  besRETURN_LONG(IupRecordInput(filename, mode));
besEND


/*
int IupPlayInput(const char *filename);  <iup.h>

Reproduces all mouse and keyboard input from a given file.
*/
besFUNCTION(PuiPlayInput)
  const char *filename;

  besARGUMENTS("[z]")
    &filename
  besARGEND

  besRETURN_LONG(IupPlayInput(filename));
besEND


/********************
 IUP Layout Functions
********************/


/*
Ihandle* IupCreate(const char *classname);  <iup.h>

Creates an interface element given its class name and parameters.
*/
besFUNCTION(PuiCreate)
  Ihandle *ih;
  const char *classname;

  besARGUMENTS("z")
    &classname
  besARGEND

  ih = IupCreate(classname);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle *IupCreatep(const char *classname, void* params0, ...)
*/
besFUNCTION(PuiCreatep)
  VARIABLE Argument;
  Ihandle *ih;
  const char *classname;
  unsigned long i;
  char *child;

  besARGUMENTS("z")
    &classname
  besARGEND

  ih = IupCreate(classname);

  for( i=2 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }
  besRETURN_POINTER(ih);
besEND

/*
void IupDestroy(Ihandle *ih);  <iup.h>

Destroys an interface element and all its children.
*/
besFUNCTION(PuiDestroy)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupDestroy(ih);
besEND


/*
int IupMap(Ihandle* ih);  <iup.h>

Creates (maps) the native interface objects corresponding to the given IUP
interface elements. It will also create the native element of all the children
in the element's tree. The element must be already attached to a container if
not a dialog.
*/
besFUNCTION(PuiMap)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_LONG(IupMap(ih));
besEND


/*
void IupUnmap(Ihandle* ih);  <iup.h>

Unmap the element from the native system. It will also unmap all its
children. It will NOT detach the element from its parent, , and it will
NOT destroy the IUP element.
*/
besFUNCTION(PuiUnmap)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupUnmap(ih);
besEND


/*
int IupGetAllClasses(char** names, int max_n);  <iup.h>

Returns the names of all registered classes.
*/
besFUNCTION(PuiGetAllClasses)
  VARIABLE Argument;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  Argument = besARGUMENT(2);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  Argument = besARGUMENT(1);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  if (!max_n) max_n = IupGetAllClasses(NULL, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetAllClasses(names,max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)),names[i],strlen(names[i]));

    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
char* IupGetClassName(Ihandle* ih);  <iup.h>

Returns the name of the class of an interface element.
*/
besFUNCTION(PuiGetClassName)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_STRING(IupGetClassName(ih));
besEND


/*
char* IupGetClassType(Ihandle* ih);  <iup.h>

Returns the name of the native type of an interface element.
(void, control, canvas, dialog, image and menu)
*/
besFUNCTION(PuiGetClassType)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_STRING(IupGetClassType(ih));
besEND


/*
int IupClassMatch(Ihandle* ih, const char* classname);  <iup.h>

Checks if the give class name matches the class name of the given
interface element.
*/
besFUNCTION(PuiClassMatch)
  Ihandle *ih;
  const char *classname;

  besARGUMENTS("pz")
    &ih, &classname
  besARGEND

  besRETURN_LONG(IupClassMatch(ih, classname));
besEND


/*
int IupGetClassAttributes(const char* classname, char** names, int max_n);  <iup.h>

Returns the names of all registered attributes of a class.

BUG: Returns wrong max count and partial list of attributes.
*/
besFUNCTION(PuiGetClassAttributes)
  VARIABLE Argument;
  const char* classname;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  besARGUMENTS("z")
    &classname
  besARGEND

  Argument = besARGUMENT(2);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  Argument = besARGUMENT(3);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  if (!max_n) max_n = IupGetClassAttributes(classname, NULL, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetClassAttributes(classname, names, max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)), names[i], strlen(names[i]));
    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
int IupGetClassCallbacks(const char* classname, char** names, int max_n);  <iup.h>

Returns the names of all registered callbacks of a class.
(not much value as callbacks are done is ScriptBasic scripts)
((A list of SB callback functions might be available to return ???))
*/
besFUNCTION(PuiGetClassCallbacks)
  VARIABLE Argument;
  const char* classname;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  besARGUMENTS("z")
    &classname
  besARGEND

  Argument = besARGUMENT(2);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);

  *Lval = NULL;

  Argument = besARGUMENT(3);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  if (!max_n) max_n = IupGetClassCallbacks(classname, NULL, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetClassCallbacks(classname, names, max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)), names[i], strlen(names[i]));
    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
void IupSaveClassAttributes(Ihandle* ih);  <iup.h>

Saves all registered attributes on the internal hash table.
*/
besFUNCTION(PuiSaveClassAttributes)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupSaveClassAttributes(ih);
besEND


/*
void IupCopyClassAttributes(Ihandle* src_ih, Ihandle* dst_ih);  <iup.h>

Copies all registered attributes from one element to another. Both elements
must be of the same class.
*/
besFUNCTION(PuiCopyClassAttributes)
  Ihandle *src_ih, *dst_ih;

  besARGUMENTS("pp")
    &src_ih, &dst_ih
  besARGEND

  IupCopyClassAttributes(src_ih, dst_ih);
besEND


/*
void IupSetClassDefaultAttribute(const char* classname, const char *name, const char *value);  <iup.h>

Changes the default value of an attribute for a class. It can be any
attribute, i.e. registered attributes or user custom attributes.
*/
besFUNCTION(PuiSetClassDefaultAttribute)
  const char *classname, *name, *value;

  besARGUMENTS("zzz")
    &classname, &name, &value
  besARGEND

  IupSetClassDefaultAttribute(classname, name, value);
besEND


/*
Ihandle* IupFill(void);  <iup.h>

Creates a Fill composition element, which dynamically occupies empty spaces
always trying to expand itself. Its parent must be an IupHbox or an IupVbox.
If an EXPAND is set on at least one of the other children of the box, then
the Fill is ignored.
*/
besFUNCTION(PuiFill)
  Ihandle *ih;

  ih = IupFill();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupHbox(Ihandle *child, ...);  <iup.h>

Creates a void container for composing elements horizontally. It is a box
that arranges the elements it contains from left to right.
*/
besFUNCTION(PuiHbox)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupHbox(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupVbox(Ihandle *child, ...);  <iup.h>

Creates a void container for composing elements vertically. It is a box that
arranges the elements it contains from top to bottom.
*/
besFUNCTION(PuiVbox)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupVbox(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupZbox (Ihandle *child, ...);  <iup.h>

Creates a void container for composing elements in hidden layers with only one
layer visible. It is a box that piles up the children it contains, only the
one child is visible.
*/
besFUNCTION(PuiZbox)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupZbox(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupRadio(Ihandle *child);  <iup.h>

Creates a void container for grouping mutual exclusive toggles. Usually it
is a vbox or an hbox containing the toggles associated to the radio. Only
one of its descendent toggles will be active at a time. The toggles can be
at any composition.
*/
besFUNCTION(PuiRadio)
  Ihandle *ih, *child;

  besARGUMENTS("[p]")
    &child
  besARGEND

  ih = IupRadio(child);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupNormalizer(Ihandle *ih_first, ...);  <iup.h>

Normalizes all controls from a list so their natural size to be the biggest
natural size among them. All natural width will be set to the biggest width,
and all natural height will be set to the biggest height according to is value. 

TODO: Allow support for multiple controls
Test append &n attribute methods - docs unclear if addition after creation is allowed.
IupAppend(ih, ih_first)
IupAttribute(ih,"ADDCONTROL_HANDLE",control_handle_name)
*/
besFUNCTION(PuiNormalizer)
  Ihandle *ih;

  ih = IupNormalizer(NULL);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupCbox(Ihandle* child, ...);  <iup.h>

Creates a void container for position elements in absolute coordinates. It is
a concrete layout container.
*/
besFUNCTION(PuiCbox)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupCbox(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupSbox(Ihandle* child);  <iup.h>

Creates a void container that allows its child to be resized. Allows expanding
and contracting the child size in one direction.
*/
besFUNCTION(PuiSbox)
  Ihandle *ih, *child;

  besARGUMENTS("[p]")
    &child
  besARGEND

  ih = IupSbox(child);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupSplit(Ihandle* child1, Ihandle* child2);  <iup.h>

Creates a void container that split its client area in two. Allows the
provided controls to be enclosed in a box that allows expanding and
contracting the element size in one direction, but when one is expanded
the other is contracted.
*/
besFUNCTION(PuiSplit)
  Ihandle *ih, *child1, *child2;

  besARGUMENTS("[p][p]")
    &child1, &child2 
  besARGEND

  ih = IupSplit(child1, child2);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupAppend(Ihandle* ih, Ihandle* new_child);  <iup.h>

Inserts an interface element at the end of the container, after the last
element of the container. Valid for any element that contains other elements
like dialog, frame, hbox, vbox, zbox or menu.
*/
besFUNCTION(PuiAppend)
  Ihandle *ih, *new_child, *actual_parent;

  besARGUMENTS("pp")
    &ih, &new_child
  besARGEND

  actual_parent = IupAppend(ih, new_child);
  besRETURN_POINTER(actual_parent);
besEND


/*
void IupDetach(Ihandle *child);  <iup.h>

Detaches an interface element from its parent.
*/
besFUNCTION(PuiDetach)
  Ihandle *child;

  besARGUMENTS("p")
    &child
  besARGEND

  IupDetach(child);
besEND


/*
Ihandle* IupInsert(Ihandle* ih, Ihandle* ref_child, Ihandle* new_child);  <iup.h>

Inserts an interface element before another child of the container.
*/
besFUNCTION(PuiInsert)
  Ihandle *ih, *ref_child, *new_child, *actual_parent;

  besARGUMENTS("ppp")
    &ih, &ref_child, &new_child
  besARGEND

  actual_parent = IupInsert(ih, ref_child, new_child);
  besRETURN_POINTER(actual_parent);
besEND


besFUNCTION(PuiInsertTop)
  Ihandle *ih, *ref_child, *new_child, *actual_parent;

  besARGUMENTS("ppp")
    &ih, &new_child
  besARGEND

  ref_child = NULL;
  actual_parent = IupInsert(ih, ref_child, new_child);
  besRETURN_POINTER(actual_parent);
besEND


/*
int IupReparent(Ihandle* child, Ihandle* new_parent, Ihandle* ref_child);  <iup.h>

Moves an interface element from one position in the hierarchy tree to another.
Both new_parent and child must be mapped or unmapped at the same time. If
ref_child is NULL, then it will append the child to the new_parent. If
ref_child is NOT NULL then it will insert child before ref_child inside the
new_parent.
*/
besFUNCTION(PuiReparent)
  Ihandle *child, *new_parent, *ref_child;

  besARGUMENTS("pp[p]")
    &child, &new_parent, &ref_child
  besARGEND

  besRETURN_LONG(IupReparent(child, new_parent, ref_child));
besEND


/*
Ihandle* IupGetParent(Ihandle *ih);  <iup.h>

Returns the parent of a control.
*/
besFUNCTION(PuiGetParent)
  Ihandle *ih, *parent;

  besARGUMENTS("p")
    &ih
  besARGEND

  parent = IupGetParent(ih);
  besRETURN_POINTER(parent);
besEND


/*
Ihandle *IupGetChild(Ihandle* ih, int pos);  <iup.h>

Returns the a child of the control given its position.
*/
besFUNCTION(PuiGetChild)
  Ihandle *ih, *child;
  int pos;

  besARGUMENTS("pi")
    &ih, &pos
  besARGEND

  child = IupGetChild(ih, pos);
  besRETURN_POINTER(child);
besEND


/*
int IupGetChildPos(Ihandle* ih, Ihandle* child);  <iup.h>

Returns the position of a child of the given control.
*/
besFUNCTION(PuiGetChildPos)
  Ihandle *ih, *child;

  besARGUMENTS("pp")
    &ih, &child
  besARGEND

  besRETURN_LONG(IupGetChildPos(ih, child));
besEND


/*
int IupGetChildCount(Ihandle* ih);  <iup.h>

Returns the number of children of the given control.
*/
besFUNCTION(PuiGetChildCount)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_LONG(IupGetChildCount(ih));
besEND


/*
Ihandle *IupGetNextChild(Ihandle* ih, Ihandle* child);  <iup.h>

Returns the a child of the given control given its brother.
*/
besFUNCTION(PuiGetNextChild)
  Ihandle *ih, *child, *next_child;

  besARGUMENTS("p[p]")
    &ih, &child
  besARGEND

  next_child = IupGetNextChild(ih, child);
  besRETURN_POINTER(next_child);
besEND


/*
Ihandle* IupGetBrother(Ihandle* ih);  <iup.h>

Returns the brother of an element.
*/
besFUNCTION(PuiGetBrother)
  Ihandle *ih, *brother;

  besARGUMENTS("p")
    &ih
  besARGEND

  brother = IupGetBrother(ih);
  besRETURN_POINTER(brother);
besEND


/*
Ihandle* IupGetDialog(Ihandle *ih);  <iup.h>

Returns the handle of the dialog that contains that interface element.
Works also for children of a menu that is associated with a dialog.
*/
besFUNCTION(PuiGetDialog)
  Ihandle *ih, *dh;

  besARGUMENTS("p")
    &ih
  besARGEND

  dh = IupGetDialog(ih);
  besRETURN_POINTER(dh);
besEND


/*
Ihandle* IupGetDialogChild(Ihandle *ih, const char* name);  <iup.h>

Returns the identifier of the child element that has the NAME attribute
equals to the given value on the same dialog hierarchy. Works also for
children of a menu that is associated with a dialog.
*/
besFUNCTION(PuiGetDialogChild)
  Ihandle *ih, *child;
  const char *name;

  besARGUMENTS("pz")
    &ih, &name
  besARGEND

  child = IupGetDialogChild(ih, name);
  besRETURN_POINTER(child);
besEND


/*
void IupRefresh(Ihandle *ih);  <iup.h>

Updates the size and layout of controls after changing size attributes. Can
be used for any element inside a dialog, the layout of the dialog will be
updated. It can change the layout of all the controls inside the dialog
because of the dynamic layout positioning. 
*/
besFUNCTION(PuiRefresh)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupRefresh(ih);
besEND


/*
void IupRefreshChildren(Ihandle *ih);  <iup.h>

Updates the size and layout of controls after changing size attributes. Can
be used for any element inside a dialog, only its children will be updated.
It can change the layout of all the controls inside the given element
because of the dynamic layout positioning. 
*/
besFUNCTION(PuiRefreshChildren)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupRefreshChildren(ih);
besEND


/*
void IupUpdate(Ihandle* ih);  <iup.h>

Mark the element to be redraw when the control returns to the system.
*/
besFUNCTION(PuiUpdate)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupUpdate(ih);
besEND


/*
void IupUpdateChildren(Ihandle* ih);  <iup.h>

Mark the elements children to be redraw when the control returns to the system.
*/
besFUNCTION(PuiUpdateChildren)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupUpdateChildren(ih);
besEND


/*
void IupRedraw(Ihandle* ih, int children);  <iup.h>

Force the element and its children to be redraw immediately.
*/
besFUNCTION(PuiRedraw)
  Ihandle *ih;
  int children;

  besARGUMENTS("pi")
    &ih, &children
  besARGEND

  IupRedraw(ih, children);
besEND


/*
int IupConvertXYToPos(Ihandle *ih, int x, int y);  <iup.h>

Converts a (x,y) coordinate in an item position.
*/
besFUNCTION(PuiConvertXYToPos)
  Ihandle *ih;
  int x, y;

  besARGUMENTS("pii")
    &ih, &x, &y
  besARGEND

  besRETURN_LONG(IupConvertXYToPos(ih, x, y));
besEND
 

/********************
 IUP Dialog Functions
********************/


/*
Ihandle* IupDialog(Ihandle *child);  <iup.h>

Creates a dialog element.
*/
besFUNCTION(PuiDialog)
  Ihandle *ih, *child;

  besARGUMENTS("[p]")
    &child
  besARGEND


  ih = IupDialog(child);
  besRETURN_POINTER(ih);
besEND


/*
int IupPopup(Ihandle *ih, int x, int y);  <iup.h>

Shows a dialog or menu and restricts user interaction only to the specified element.
*/
besFUNCTION(PuiPopup)
  Ihandle *ih;
  int x, y;

  besARGUMENTS("pii")
    &ih, &x, &y
  besARGEND

  besRETURN_LONG(IupPopup(ih, x, y));
besEND


/*
int IupShow(Ihandle *ih);  <iup.h>

Displays a dialog in the current position, or changes a control VISIBLE
attribute. If the dialog needs to be mapped and the current position is
not known then the dialog is centered.
*/
besFUNCTION(PuiShow)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_LONG(IupShow(ih));
besEND


/*
int IupShowXY(Ihandle *ih, int x, int y);  <iup.h>

Displays a dialog in a given position on the screen.
*/
besFUNCTION(PuiShowXY)
  Ihandle *ih;
  int x, y;

  besARGUMENTS("pii")
    &ih, &x, &y
  besARGEND

  besRETURN_LONG(IupShowXY(ih, x, y));
besEND


/*
int IupHide(Ihandle *ih);  <iup.h>

Hides an interface element. This function has the same effect as attributing
value "NO" to the interface element’s VISIBLE attribute.
*/
besFUNCTION(PuiHide)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_LONG(IupHide(ih));
besEND


/*
Ihandle* IupFileDlg(void);  <iup.h>

Creates the File Dialog element.
*/
besFUNCTION(PuiFileDlg)
  Ihandle *ih;

  ih = IupFileDlg();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupMessageDlg(void);  <iup.h>

Creates the Message Dialog element.
*/
besFUNCTION(PuiMessageDlg)
  Ihandle *ih;

  ih = IupMessageDlg();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupColorDlg(void);  <iup.h>

Creates the Color Dialog element.
*/
besFUNCTION(PuiColorDlg)
  Ihandle *ih;

  ih = IupColorDlg();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupFontDlg(void);  <iup.h>

Creates the Font Dialog element.
*/
besFUNCTION(PuiFontDlg)
  Ihandle *ih;

  ih = IupFontDlg();
  besRETURN_POINTER(ih);
besEND


/*
int IupAlarm(const char *t, const char *m, const char *b1, const char *b2, const char *b3);  <iup.h>

Shows a modal dialog containing a message and up to three buttons.
*/
besFUNCTION(PuiAlarm)
const char *t, *m, *b1, *b2, *b3;

  besARGUMENTS("zzz[z][z]")
    &t, &m, &b1, &b2, &b3
  besARGEND

if ((unsigned)besARGNR == 3) {
  b2 = NULL;
  b3 = NULL;};

if ((unsigned)besARGNR == 4) b3 = NULL;

  besRETURN_LONG(IupAlarm(t, m, b1, b2, b3));
besEND


/*
int IupGetFile(char *filename);  <iup.h>

Shows a modal dialog of the native interface system to select a filename.
*/
besFUNCTION(PuiGetFile)
  char *filename;

  besARGUMENTS("z")
    &filename
  besARGEND

  besRETURN_LONG(IupGetFile(filename));
besEND


/*
int IupGetColor(int x, int y, unsigned char *r, unsigned char *g, unsigned char *b);  <iup.h>

Shows a modal dialog which allows the user to select a color.
*/
besFUNCTION(PuiGetColor)
  int x, y;
  unsigned char *r, *g, *b;

  besARGUMENTS("iisss")
    &x, &y, &r, &g, &b
  besARGEND

  besRETURN_LONG(IupGetColor(x, y, r, g, b));
besEND


/*
int IupGetParam(const char* title, Iparamcb action, void* user_data, const char* format,...);  <iup.h>

Shows a modal dialog for capturing parameter values using several types of controls.
*/
besFUNCTION(PuiGetParam)

// TODO: Complex funtion to be address in phase II.

besEND


/*
int IupGetText(const char* title, char *text);  <iup.h>

Shows a modal dialog to edit a multiline text.
*/
besFUNCTION(PuiGetText)
  const char *title;
  char *text;

  besARGUMENTS("zz")
    &title, &text
  besARGEND

  besRETURN_LONG(IupGetText(title, text));
besEND


/*
int IupListDialog(int type, const char *title, int size, const char** list, int op, int max_col, int max_lin, int* marks);  <iup.h>

Shows a modal dialog to select items from a simple or multiple selection list.
*/
besFUNCTION(PuiListDialog)

// TODO: Complex funtion to be address in phase II.

besEND


/*
void IupMessage(const char *title, const char *message);  <iup.h>

Shows a modal dialog containing a message.
*/
besFUNCTION(PuiMessage)
  const char *title, *message;

  besARGUMENTS("zz")
    &title, &message
  besARGEND

  IupMessage(title, message);
besEND


/*
Ihandle* IupLayoutDialog(Ihandle* dialog);  <iup.h>

Creates a Layout Dialog. It is a predefined dialog to visually edit
the layout of another dialog in run time.
*/
besFUNCTION(PuiLayoutDialog)
  Ihandle *ih, *dialog;

  besARGUMENTS("p")
    &dialog
  besARGEND

  ih = IupLayoutDialog(dialog);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupElementPropertiesDialog(Ihandle* elem);  <iup.h>

Creates an Element Properties Dialog. It is a predefined dialog to edit
the properties of an element in run time.
*/
besFUNCTION(PuiElementPropertiesDialog)
  Ihandle *ih, *elem;

  besARGUMENTS("p")
    &elem
  besARGEND

  ih = IupElementPropertiesDialog(elem);
  besRETURN_POINTER(ih);
besEND


/*********************
 IUP Control Functions
*********************/


/*
Ihandle* IupButton(const char *title, const char *action);  <iup.h>

Creates an interface element that is a button. When selected, this element
activates a function in the application. Its visual presentation can contain
a text and/or an image.
*/
besFUNCTION(PuiButton)
  Ihandle *ih;
  const char *title, *action;

  besARGUMENTS("[z][z]")
    &title, &action
  besARGEND

  ih = IupButton(title, action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupCanvas(const char *action);  <iup.h>

Creates an interface element that is a canvas - a working area
for your application.
*/
besFUNCTION(PuiCanvas)
  Ihandle *ih;
  const char *action;

  besARGUMENTS("[z]")
    &action
  besARGEND

  ih = IupCanvas(action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupFrame(Ihandle *child);  <iup.h>

Creates a Frame interface element, which draws a frame with a title
around an interface element.
*/
besFUNCTION(PuiFrame)
  Ihandle *ih, *child;

  besARGUMENTS("[p]")
    &child
  besARGEND

  ih = IupFrame(child);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupLabel(const char *title);  <iup.h>

Creates a label interface element, which displays a separator, a text
or an image.
*/
besFUNCTION(PuiLabel)
  Ihandle *ih;
  const char *title;

  besARGUMENTS("[z]")
    &title
  besARGEND

  ih = IupLabel(title);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupList(const char *action);  <iup.h>

Creates an interface element that displays a list of items. The list
can be visible or can be dropped down. It also can have an edit box for
text input. So it is a 4 in 1 element. In native systems the dropped
down case is called Combo Box.
*/
besFUNCTION(PuiList)
  Ihandle *ih;
  const char *action;

  besARGUMENTS("[z]")
    &action
  besARGEND

  ih = IupList(action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupMultiLine(const char *action);  <iup.h>
 
Creates an editable field with one or more lines.
Note: Use IupText element with MULTILINE=YES
*/
besFUNCTION(PuiMultiLine)
  Ihandle *ih;
  const char *action;

  besARGUMENTS("[z]")
    &action
  besARGEND

  ih = IupMultiLine(action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupProgressBar(void);  <iup.h>

Creates a progress bar control. Shows a percent value that can be updated
to simulate a progression.
*/
besFUNCTION(PuiProgressBar)
  Ihandle *ih;

  ih = IupProgressBar();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupSpin(void);  <iup.h>

This functions will create a control set with a vertical box containing two
buttons, one with an up arrow and the other with a down arrow, to be used to
increment and decrement values.
*/
besFUNCTION(PuiSpin)
  Ihandle *ih;

  ih = IupSpin();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupTabs(Ihandle* child, ...);  <iup.h>

Creates a Tabs element. Allows a single dialog to have several screens,
grouping options. The grouping is done in a single line of tabs arranged
according to the tab type.
*/
besFUNCTION(PuiTabs)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupTabs(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupTabsv(Ihandle** children);  <iup.h>

See above.

TODO: Fix this.
*/
besFUNCTION(PuiTabsv)

// See IupCreatev("tabs", (void**)params);

besEND


/*
Ihandle* IupText(const char *action);  <iup.h>

Creates an editable text field.
*/
besFUNCTION(PuiText)
  Ihandle *ih;
  const char *action;

  besARGUMENTS("[z]")
    &action
  besARGEND

  ih = IupText(action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupToggle(const char *title, const char *action);  <iup.h>

Creates the toggle interface element. It is a two-state (on/off) button
that, when selected, generates an action that activates a function in
the associated application. Its visual representation can contain a text
or an image.
*/
besFUNCTION(PuiToggle)
  Ihandle *ih;
  const char *title, *action;

  besARGUMENTS("[z][z]")
    &title, &action
  besARGEND

  ih = IupToggle(title, action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupTree(void);  <iup.h>

Creates a tree containing nodes of branches or leaves. Both branches and
leaves can have an associated text and image. The branches can be expanded
or collapsed. When a branch is expanded, its immediate children are visible,
and when it is collapsed they are hidden.
*/
besFUNCTION(PuiTree)
  Ihandle *ih;

  ih = IupTree();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupVal(const char *orientation);  <iup.h>

Creates a Valuator control. Selects a value in a limited interval.
*/
besFUNCTION(PuiVal)
  Ihandle *ih;
  const char *orientation;

  besARGUMENTS("[z]")
    &orientation
  besARGEND

  ih = IupVal(orientation);
  besRETURN_POINTER(ih);
besEND



/**********************
 IUP Resource Functions
**********************/


/*
Ihandle* IupImage(int width, int height, const unsigned char *pixels);  <iup.h>

Creates an image to be shown on a label, button, toggle, or as a cursor.
*/
besFUNCTION(PuiImage)
  int width, height;
  const unsigned char *pixels;
  Ihandle *ih;

  besARGUMENTS("iis")
    &width, &height, &pixels
  besARGEND

  ih = IupImage(width, height, pixels);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupImageRGB(int width, int height, const unsigned char *pixels);  <iup.h>

See above - uses 3 values per pixel.
*/
besFUNCTION(PuiImageRGB)
  int width, height;
  const unsigned char *pixels;
  Ihandle *ih;

  besARGUMENTS("iis")
    &width, &height, &pixels
  besARGEND

  ih = IupImageRGB(width, height, pixels);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupImageRGBA(int width, int height, const unsigned char *pixels);  <iup.h>

See above - uses 4 values per pixel.
*/
besFUNCTION(PuiImageRGBA)
  int width, height;
  const unsigned char *pixels;
  Ihandle *ih;

  besARGUMENTS("iis")
    &width, &height, &pixels
  besARGEND

  ih = IupImageRGBA(width, height, pixels);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupNextField(Ihandle* ih);  <iup.h>

Shifts the focus to the next element that can have the focus. It is relative
to the given element and does not depend on the element currently with the focus.
*/
besFUNCTION(PuiNextField)
  Ihandle *ih, *nif;

  besARGUMENTS("p")
    &ih
  besARGEND

  nif = IupNextField(ih);
  besRETURN_POINTER(nif);
besEND


/*
Ihandle* IupPreviousField(Ihandle* ih);  <iup.h>

Shifts the focus to the previous element that can have the focus.
*/
besFUNCTION(PuiPreviousField)
  Ihandle *ih, *nif;

  besARGUMENTS("p")
    &ih
  besARGEND

  nif = IupPreviousField(ih);
  besRETURN_POINTER(nif);
besEND


/*
Ihandle* IupGetFocus(void);  <iup.h>

Returns the identifier of the interface element that has the keyboard focus,
i.e. the element that will receive keyboard events.
*/
besFUNCTION(PuiGetFocus)
  Ihandle *ih;

  ih = IupGetFocus();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle *IupSetFocus(Ihandle *ih);  <iup.h>

Defines the interface element that will receive the keyboard focus,
i.e., the element that will receive keyboard events. 
*/
besFUNCTION(PuiSetFocus)
  Ihandle *ih, *nif;

  besARGUMENTS("p")
    &ih
  besARGEND

  nif = IupSetFocus(ih);
  besRETURN_POINTER(nif);
besEND


/*
Ihandle* IupItem(const char *title, const char *action);  <iup.h>

Creates an item of the menu interface element. When selected, it generates an action.
*/
besFUNCTION(PuiItem)
  const char *title, *action;
  Ihandle *ih;

  besARGUMENTS("[z][z]")
    &title, &action
  besARGEND

  ih = IupItem(title, action);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupMenu(Ihandle *child, ...);  <iup.h>

Creates a menu element, which groups 3 types of interface elements: item,
submenu and separator.
*/
besFUNCTION(PuiMenu)
  VARIABLE Argument;
  Ihandle *ih;
  unsigned long i;
  char *child;

  ih = IupMenu(NULL);

  for( i=1 ; i <= (unsigned)besARGNR ; i++ ){
    Argument = besARGUMENT(i);
    besDEREFERENCE(Argument);
    memcpy(&child, STRINGVALUE(Argument), sizeof(child));
    IupAppend(ih, child);
    }

  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupMenuv(Ihandle **children);  <iup.h>
*/
besFUNCTION(PuiMenuv)

// TODO: Determine if needed.

besEND


/*
Ihandle* IupSeparator(void);  <iup.h>

Creates the separator interface element. It shows a line between two menu items.
*/
besFUNCTION(PuiSeparator)
  Ihandle *ih;

  ih = IupSeparator();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupSubmenu(const char *title, Ihandle *menu);  <iup.h>

Creates a menu item that, when selected, opens another menu.
*/
besFUNCTION(PuiSubmenu)
  Ihandle *ih, *menu;
  const char *title;

  besARGUMENTS("[z][p]")
    &title, &menu
  besARGEND

  ih = IupSubmenu(title, menu);
  besRETURN_POINTER(ih);
besEND


/*
Ihandle *IupSetHandle(const char *name, Ihandle *ih);  <iup.h>

Associates a name with an interface element.
*/
besFUNCTION(PuiSetHandle)
  Ihandle *ih, *pih;
  const char *name;

  besARGUMENTS("z[p]")
    &name, &ih
  besARGEND

  pih = IupSetHandle(name, ih);
  besRETURN_POINTER(pih);
besEND


/*
Ihandle *IupGetHandle(const char *name);  <iup.h>

Returns the identifier of an interface element that has an associated
name using IupSetHandle or using LED.
*/
besFUNCTION(PuiGetHandle)
  Ihandle *ih;
  const char *name;

  besARGUMENTS("z")
    &name
  besARGEND

  ih = IupGetHandle(name);
  besRETURN_POINTER(ih);
besEND


/*
char* IupGetName(Ihandle* ih);  <iup.h>

Returns the name of an interface element, if the element has an associated name
using IupSetHandle or using LED.
*/
besFUNCTION(PuiGetName)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  besRETURN_STRING(IupGetName(ih));
besEND


/*
int IupGetAllNames(char** names, int max_n);  <iup.h>

Returns the names of all interface elements that have an associated name
using IupSetHandle or using LED.
*/
besFUNCTION(PuiGetAllNames)
  VARIABLE Argument;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  Argument = besARGUMENT(2);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  Argument = besARGUMENT(1);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  if (!max_n) max_n = IupGetAllNames(NULL, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetAllNames(names,max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)),names[i],strlen(names[i]));
    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
int IupGetAllDialogs(char** names, int max_n);  <iup.h>

Returns the names of all dialogs that have an associated name using IupSetHandle or
using LED. Other dialogs will not be returned.
*/
besFUNCTION(PuiGetAllDialogs)
  VARIABLE Argument;
  unsigned long __refcount_;
  LEFTVALUE Lval;

  int n, i, max_n;
  char **names;

  Argument = besARGUMENT(2);
  besDEREFERENCE(Argument);
  max_n = LONGVALUE(Argument);

  Argument = besARGUMENT(1);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  if (!max_n) max_n = IupGetAllDialogs(NULL, 0);
  *Lval = besNEWARRAY(0,max_n-1);

  names = (char **) malloc (max_n * sizeof(char *));
  if( names == NULL )return COMMAND_ERROR_MEMORY_LOW;

  n = IupGetAllDialogs(names,max_n);

  for (i = 0; i < n; i++) {
    ARRAYVALUE(*Lval,i) = besNEWSTRING(strlen(names[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,i)),names[i],strlen(names[i]));
    }

  free(names);

  besRETURN_LONG(n);
besEND


/*
Ihandle* IupClipboard(void);  <iup.h>

Creates an element that allows access to the clipboard.
*/
besFUNCTION(PuiClipboard)
  Ihandle *ih;

  ih = IupClipboard();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupTimer(void);  <iup.h>

Creates a timer which periodicaly invokes a callback when the time is up.
Each timer should be destroyed using IupDestroy.
*/
besFUNCTION(PuiTimer)
  Ihandle *ih;

  ih = IupTimer();
  besRETURN_POINTER(ih);
besEND


/*
Ihandle* IupUser(void);  <iup.h>

Creates a user element in IUP, which is not associated to any interface
element. It is used to map an external element to a IUP element. Its use
is usually done by CPI elements, but you can use it to create an Ihandle
to store private attributes.
*/
besFUNCTION(PuiUser)
  Ihandle *ih;

  ih = IupUser();
  besRETURN_POINTER(ih);
besEND


/*
int IupHelp(const char* url);  <iup.h>

Opens the given URL.
*/
besFUNCTION(PuiHelp)
  const char *url;

  besARGUMENTS("z")
    &url
  besARGEND

  besRETURN_LONG(IupHelp(url));
besEND


/****************
 Helper Functions
****************/


/* deprecated - Use IupSetAttribute */
besFUNCTION(SB_ClearList)
  Ihandle *ih;

  besARGUMENTS("p")
    &ih
  besARGEND

  IupSetAttribute(ih,"1", NULL);
besEND


besFUNCTION(SB_Info)
  VARIABLE Argument;
  unsigned long __refcount_;
  LEFTVALUE Lval;
  char buffer[1024];
  int i;
  const char *glbvar[] = {
  "SYSTEMLANGUAGE",
  "DRIVER",
  "SYSTEM",
  "SYSTEMLOCALE",
  "COMPUTERNAME",
  "USERNAME",
  "MONITORSINFO",
  "SCREENSIZE",
  "SCREENDEPTH",
  "VIRTUALSCREEN",
  "DLGFGCOLOR",
  "DLGBGCOLOR",
  "DEFAULTFONT",
  "DEFAULTFONTSIZE",
  "TXTFGCOLOR",
  "TXTBGCOLOR"
  };
  
  besRETURNVALUE = NULL;

  Argument = besARGUMENT(1);
  besLEFTVALUE(Argument,Lval);
  besRELEASE(*Lval);
  *Lval = NULL;

  *Lval = besNEWARRAY(0,ELEMENTS(glbvar)*2-1);
  if( *Lval == NULL )return COMMAND_ERROR_MEMORY_LOW;

  for (i =0; i < ELEMENTS(glbvar);i++) {
    ARRAYVALUE(*Lval,2*i) = besNEWSTRING(strlen(glbvar[i]));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,2*i)),glbvar[i],strlen(glbvar[i]));
    memset(buffer,0,1024);
    strcpy(buffer, IupGetGlobal(glbvar[i]));
    ARRAYVALUE(*Lval,2*i+1) = besNEWSTRING(strlen(buffer));
    memcpy(STRINGVALUE(ARRAYVALUE(*Lval,2*i+1)),buffer,strlen(buffer));
  }

  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) = -1;
besEND


/************************
 Exported Function List
************************/


START_FUNCTION_TABLE(IUP_SLFST)
// Ext. module
  EXPORT_MODULE_FUNCTION(versmodu)
  EXPORT_MODULE_FUNCTION(bootmodu)
  EXPORT_MODULE_FUNCTION(finimodu)
// System
  EXPORT_MODULE_FUNCTION(PuiOpen)
  EXPORT_MODULE_FUNCTION(PuiImageLibOpen)
  EXPORT_MODULE_FUNCTION(PuiClose)
  EXPORT_MODULE_FUNCTION(PuiVersion)
  EXPORT_MODULE_FUNCTION(PuiLoad)
  EXPORT_MODULE_FUNCTION(PuiLoadBuffer)
  EXPORT_MODULE_FUNCTION(PuiSetLanguage)
  EXPORT_MODULE_FUNCTION(PuiGetLanguage)
// Attribute
  EXPORT_MODULE_FUNCTION(PuiStoreAttribute)
  EXPORT_MODULE_FUNCTION(PuiStoreGlobalAttribute)
  EXPORT_MODULE_FUNCTION(PuiStoreAttributeId)
  EXPORT_MODULE_FUNCTION(PuiStoreGlobalAttributeId)
  EXPORT_MODULE_FUNCTION(PuiSetAttribute)
  EXPORT_MODULE_FUNCTION(PuiSetGlobalAttribute)
  EXPORT_MODULE_FUNCTION(PuiSetGlobalAttributeId)
  EXPORT_MODULE_FUNCTION(PuiSetAttributeId)
  EXPORT_MODULE_FUNCTION(PuiSetfAttribute)
  EXPORT_MODULE_FUNCTION(PuiSetfAttributeId)
  EXPORT_MODULE_FUNCTION(PuiSetfAttributeId2)
  EXPORT_MODULE_FUNCTION(PuiSetAttributes)
  EXPORT_MODULE_FUNCTION(PuiResetAttribute)
  EXPORT_MODULE_FUNCTION(PuiResetGlobalAttribute)
  EXPORT_MODULE_FUNCTION(PuiSetAtt)
  EXPORT_MODULE_FUNCTION(PuiSetAttributeHandle)
  EXPORT_MODULE_FUNCTION(PuiGetAttributeHandle)
  EXPORT_MODULE_FUNCTION(PuiGetAttribute)
  EXPORT_MODULE_FUNCTION(PuiGetGlobalAttribute)
  EXPORT_MODULE_FUNCTION(PuiGetAttributeId)
  EXPORT_MODULE_FUNCTION(PuiGetGlobalAttributeId)
  EXPORT_MODULE_FUNCTION(PuiGetAllAttributes)
  EXPORT_MODULE_FUNCTION(PuiGetAttributes)
  EXPORT_MODULE_FUNCTION(PuiGetFloat)
  EXPORT_MODULE_FUNCTION(PuiGetFloatId)
  EXPORT_MODULE_FUNCTION(PuiGetFloatId2)
  EXPORT_MODULE_FUNCTION(PuiGetInt)
  EXPORT_MODULE_FUNCTION(PuiGetInt2)
  EXPORT_MODULE_FUNCTION(PuiGetIntInt)
  EXPORT_MODULE_FUNCTION(PuiGetIntId)
  EXPORT_MODULE_FUNCTION(PuiGetIntId2)
  EXPORT_MODULE_FUNCTION(PuiStoreGlobal)
  EXPORT_MODULE_FUNCTION(PuiSetGlobal)
  EXPORT_MODULE_FUNCTION(PuiGetGlobal)
// Events
  EXPORT_MODULE_FUNCTION(PuiMainLoop)
  EXPORT_MODULE_FUNCTION(PuiMainLoopLevel)
  EXPORT_MODULE_FUNCTION(PuiLoopStep)
  EXPORT_MODULE_FUNCTION(PuiLoopStepWait)
  EXPORT_MODULE_FUNCTION(PuiExitLoop)
  EXPORT_MODULE_FUNCTION(PuiFlush)
  EXPORT_MODULE_FUNCTION(PuiGetCallback)
  EXPORT_MODULE_FUNCTION(PuiSetCallback)
  EXPORT_MODULE_FUNCTION(PuiSetCallbacks)
  EXPORT_MODULE_FUNCTION(PuiGetActionName)
  EXPORT_MODULE_FUNCTION(PuiSetFunction)
  EXPORT_MODULE_FUNCTION(PuiRecordInput)
  EXPORT_MODULE_FUNCTION(PuiPlayInput)
// Layout
  EXPORT_MODULE_FUNCTION(PuiCreate)
  EXPORT_MODULE_FUNCTION(PuiCreatep)
  EXPORT_MODULE_FUNCTION(PuiDestroy)
  EXPORT_MODULE_FUNCTION(PuiMap)
  EXPORT_MODULE_FUNCTION(PuiUnmap)
  EXPORT_MODULE_FUNCTION(PuiGetAllClasses)
  EXPORT_MODULE_FUNCTION(PuiGetClassName)
  EXPORT_MODULE_FUNCTION(PuiGetClassType)
  EXPORT_MODULE_FUNCTION(PuiClassMatch)
  EXPORT_MODULE_FUNCTION(PuiGetClassAttributes)
  EXPORT_MODULE_FUNCTION(PuiGetClassCallbacks)
  EXPORT_MODULE_FUNCTION(PuiSaveClassAttributes)
  EXPORT_MODULE_FUNCTION(PuiCopyClassAttributes)
  EXPORT_MODULE_FUNCTION(PuiSetClassDefaultAttribute)
  EXPORT_MODULE_FUNCTION(PuiFill)
  EXPORT_MODULE_FUNCTION(PuiHbox)
  EXPORT_MODULE_FUNCTION(PuiVbox)
  EXPORT_MODULE_FUNCTION(PuiZbox)
  EXPORT_MODULE_FUNCTION(PuiRadio)
  EXPORT_MODULE_FUNCTION(PuiNormalizer)
  EXPORT_MODULE_FUNCTION(PuiCbox)
  EXPORT_MODULE_FUNCTION(PuiSbox)
  EXPORT_MODULE_FUNCTION(PuiSplit)
  EXPORT_MODULE_FUNCTION(PuiAppend)
  EXPORT_MODULE_FUNCTION(PuiDetach)
  EXPORT_MODULE_FUNCTION(PuiInsert)
  EXPORT_MODULE_FUNCTION(PuiInsertTop)
  EXPORT_MODULE_FUNCTION(PuiReparent)
  EXPORT_MODULE_FUNCTION(PuiGetParent)
  EXPORT_MODULE_FUNCTION(PuiGetChild)
  EXPORT_MODULE_FUNCTION(PuiGetChildPos)
  EXPORT_MODULE_FUNCTION(PuiGetChildCount)
  EXPORT_MODULE_FUNCTION(PuiGetNextChild)
  EXPORT_MODULE_FUNCTION(PuiGetBrother)
  EXPORT_MODULE_FUNCTION(PuiGetDialog)
  EXPORT_MODULE_FUNCTION(PuiGetDialogChild)
  EXPORT_MODULE_FUNCTION(PuiRefresh)
  EXPORT_MODULE_FUNCTION(PuiRefreshChildren)
  EXPORT_MODULE_FUNCTION(PuiUpdate)
  EXPORT_MODULE_FUNCTION(PuiUpdateChildren)
  EXPORT_MODULE_FUNCTION(PuiRedraw)
  EXPORT_MODULE_FUNCTION(PuiConvertXYToPos)
// Dialog
  EXPORT_MODULE_FUNCTION(PuiDialog)
  EXPORT_MODULE_FUNCTION(PuiPopup)
  EXPORT_MODULE_FUNCTION(PuiShow)
  EXPORT_MODULE_FUNCTION(PuiShowXY)
  EXPORT_MODULE_FUNCTION(PuiHide)
  EXPORT_MODULE_FUNCTION(PuiFileDlg)
  EXPORT_MODULE_FUNCTION(PuiMessageDlg)
  EXPORT_MODULE_FUNCTION(PuiColorDlg)
  EXPORT_MODULE_FUNCTION(PuiFontDlg)
  EXPORT_MODULE_FUNCTION(PuiAlarm)
  EXPORT_MODULE_FUNCTION(PuiGetFile)
  EXPORT_MODULE_FUNCTION(PuiGetColor)
  EXPORT_MODULE_FUNCTION(PuiGetParam)
  EXPORT_MODULE_FUNCTION(PuiGetText)
  EXPORT_MODULE_FUNCTION(PuiListDialog)
  EXPORT_MODULE_FUNCTION(PuiMessage)
  EXPORT_MODULE_FUNCTION(PuiLayoutDialog)
  EXPORT_MODULE_FUNCTION(PuiElementPropertiesDialog)
//Controls
  EXPORT_MODULE_FUNCTION(PuiButton)
  EXPORT_MODULE_FUNCTION(PuiCanvas)
  EXPORT_MODULE_FUNCTION(PuiFrame)
  EXPORT_MODULE_FUNCTION(PuiLabel)
  EXPORT_MODULE_FUNCTION(PuiList)
  EXPORT_MODULE_FUNCTION(PuiMultiLine)
  EXPORT_MODULE_FUNCTION(PuiProgressBar)
  EXPORT_MODULE_FUNCTION(PuiSpin)
  EXPORT_MODULE_FUNCTION(PuiTabs)
  EXPORT_MODULE_FUNCTION(PuiTabsv)
  EXPORT_MODULE_FUNCTION(PuiText)
  EXPORT_MODULE_FUNCTION(PuiToggle)
  EXPORT_MODULE_FUNCTION(PuiTree)
  EXPORT_MODULE_FUNCTION(PuiVal)
//Resources
  EXPORT_MODULE_FUNCTION(PuiImage)
  EXPORT_MODULE_FUNCTION(PuiImageRGB)
  EXPORT_MODULE_FUNCTION(PuiImageRGBA)
  EXPORT_MODULE_FUNCTION(PuiNextField)
  EXPORT_MODULE_FUNCTION(PuiPreviousField)
  EXPORT_MODULE_FUNCTION(PuiGetFocus)
  EXPORT_MODULE_FUNCTION(PuiSetFocus)
  EXPORT_MODULE_FUNCTION(PuiItem)
  EXPORT_MODULE_FUNCTION(PuiMenu)
  EXPORT_MODULE_FUNCTION(PuiMenuv)
  EXPORT_MODULE_FUNCTION(PuiSeparator)
  EXPORT_MODULE_FUNCTION(PuiSubmenu)
  EXPORT_MODULE_FUNCTION(PuiSetHandle)
  EXPORT_MODULE_FUNCTION(PuiGetHandle)
  EXPORT_MODULE_FUNCTION(PuiGetName)
  EXPORT_MODULE_FUNCTION(PuiGetAllNames)
  EXPORT_MODULE_FUNCTION(PuiGetAllDialogs)
  EXPORT_MODULE_FUNCTION(PuiClipboard)
  EXPORT_MODULE_FUNCTION(PuiTimer)
  EXPORT_MODULE_FUNCTION(PuiUser)
  EXPORT_MODULE_FUNCTION(PuiHelp)
// Helper Functions
  EXPORT_MODULE_FUNCTION(SB_GetEvent)
  EXPORT_MODULE_FUNCTION(SB_GetListText)
  EXPORT_MODULE_FUNCTION(SB_GetBtnPressed)
  EXPORT_MODULE_FUNCTION(SB_ClearList)
  EXPORT_MODULE_FUNCTION(SB_Info)
END_FUNCTION_TABLE

