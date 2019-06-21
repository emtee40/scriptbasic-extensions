/*
WiringPi Extension Module
UXLIBS: -lc -lwiringPi
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../basext.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringShift.h>
#include <wiringPiSPI.h>

#define MAXTIMINGS  85



/**************************
 Extension Module Functions
**************************/

typedef struct _ModuleObject {
  void *HandleArray;
}ModuleObject,*pModuleObject;


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



/******************
 WiringPi Functions
******************/

// Core wiringPi functions


besFUNCTION(sb_wiringPiSetup)
  int status;
  status = wiringPiSetup();
  besRETURN_LONG(status);
besEND


besFUNCTION(sb_wiringPiSetupSys)
  int status;
  status = wiringPiSetupSys();
  besRETURN_LONG(status);
besEND


besFUNCTION(sb_wiringPiSetupGpio)
  int status;
  status = wiringPiSetupGpio();
  besRETURN_LONG(status);
besEND


besFUNCTION(sb_wiringPiSetupPhys)
  int status;
  status = wiringPiSetupPhys();
  besRETURN_LONG(status);
besEND


besFUNCTION(sb_pinModeAlt)
  VARIABLE Argument;
  int pin, mode;
  besARGUMENTS("ii")
    &pin, &mode
  besARGEND
  pinModeAlt(pin,mode);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pinMode)
  VARIABLE Argument;
  int pin, mode;
  besARGUMENTS("ii")
    &pin, &mode
  besARGEND
  pinMode(pin,mode);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pullUpDnControl)
  VARIABLE Argument;
  int pin, pud;
  besARGUMENTS("ii")
    &pin, &pud
  besARGEND
  pullUpDnControl(pin,pud);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_digitalRead)
  int pin, rtn;
  besARGUMENTS("i")
    &pin
  besARGEND
  rtn = digitalRead(pin);
  besRETURN_LONG(rtn);
besEND

 
besFUNCTION(sb_digitalWrite)
  VARIABLE Argument;
  int pin, value;
  besARGUMENTS("ii")
    &pin, &value
  besARGEND
  digitalWrite(pin,value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_digitalRead8)
  int pin;
  unsigned int rtn;
  besARGUMENTS("i")
    &pin
  besARGEND
  rtn = digitalRead8(pin);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_digitalWrite8)
  VARIABLE Argument;
  int pin, value;
  besARGUMENTS("ii")
    &pin, &value
  besARGEND
  digitalWrite8(pin,value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pwmWrite)
  VARIABLE Argument;
  int pin, value;
  besARGUMENTS("ii")
    &pin, &value
  besARGEND
  pwmWrite(pin,value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_analogRead)
  int pin, rtn;
  besARGUMENTS("i")
    &pin
  besARGEND
  rtn = analogRead(pin);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_analogWrite)
  VARIABLE Argument;
  int pin, value;
  besARGUMENTS("ii")
    &pin, &value
  besARGEND
  analogWrite(pin,value);
  besRETURNVALUE = NULL;
besEND


// On-Board Raspberry Pi hardware specific


besFUNCTION(sb_piGpioLayout)
  int rtn;
  rtn = piGpioLayout();
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wpiPinToGpio)
  int wpiPin, rtn;
  besARGUMENTS("i")
    &wpiPin
  besARGEND
  rtn = wpiPinToGpio(wpiPin);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_physPinToGpio)
  int physPin, rtn;
  besARGUMENTS("i")
    &physPin
  besARGEND
  rtn = physPinToGpio(physPin);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_setPadDrive)
  VARIABLE Argument;
  int group, value;
  besARGUMENTS("ii")
    &group, &value
  besARGEND
  setPadDrive(group,value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_getAlt)
  int pin, rtn;
  besARGUMENTS("i")
    &pin
  besARGEND
  rtn = getAlt(pin);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_pwmToneWrite)
  VARIABLE Argument;
  int pin, freq;
  besARGUMENTS("ii")
    &pin, &freq
  besARGEND
  pwmToneWrite(pin,freq);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pwmSetMode)
  VARIABLE Argument;
  int mode;
  besARGUMENTS("i")
    &mode
  besARGEND
  pwmSetMode(mode);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pwmSetRange)
  VARIABLE Argument;
  unsigned int range;
  besARGUMENTS("i")
    &range
  besARGEND
  pwmSetRange(range);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_pwmSetClock)
  VARIABLE Argument;
  unsigned int divisor;
  besARGUMENTS("i")
    &divisor
  besARGEND
  pwmSetClock(divisor);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_gpioClockSet)
  VARIABLE Argument;
  int pin, freq;
  besARGUMENTS("ii")
    &pin, &freq
  besARGEND
  gpioClockSet(pin,freq);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_digitalReadByte)
  unsigned int rtn;
  rtn = digitalReadByte();
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_digitalReadByte2)
  unsigned int rtn;
  rtn = digitalReadByte2();
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_digitalWriteByte)
  VARIABLE Argument;
  int value;
  besARGUMENTS("i")
    &value
  besARGEND
  digitalWriteByte(value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_digitalWriteByte2)
  VARIABLE Argument;
  int value;
  besARGUMENTS("i")
    &value
  besARGEND
  digitalWriteByte2(value);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_waitForInterrupt)
  int pin, ms, rtn;
  besARGUMENTS("ii")
    &pin, &ms
  besARGEND
  rtn = waitForInterrupt(pin,ms);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_piHiPri)
  const int pri;
  int rtn;
  besARGUMENTS("i")
    &pri
  besARGEND
  rtn = piHiPri(pri);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_BitStreamRead)
  char buf[850];
  int pin, status, idx;
  besARGUMENTS("i")
    &pin
  besARGEND
  for (idx = 0; idx < 850; idx++){
    status = digitalRead(pin);
    delayMicroseconds(5);
    if (status == 0){
      buf[idx] = 0x30;
    }else{
      buf[idx] = 0x31;
    }
  }
  besRETURN_STRING(buf);
besEND



// Simplified I2C access routines


besFUNCTION(sb_wiringPiI2CRead)
  int fd,rtn;
  VARIABLE Argument;
  besARGUMENTS("i")
    &fd
  besARGEND
  rtn = wiringPiI2CRead(fd);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CReadReg8)
  int fd,reg,rtn;
  VARIABLE Argument;
  besARGUMENTS("ii")
    &fd,&reg
  besARGEND
  rtn = wiringPiI2CReadReg8(fd,reg);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CReadReg16)
  int fd,reg,rtn;
  VARIABLE Argument;
  besARGUMENTS("ii")
    &fd,&reg
  besARGEND
  rtn = wiringPiI2CReadReg16(fd,reg);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CWrite)
  int fd,data,rtn;
  VARIABLE Argument;
  besARGUMENTS("ii")
    &fd,&data
  besARGEND
  rtn = wiringPiI2CWrite(fd,data);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CWriteReg8)
  int fd,reg,data,rtn;
  VARIABLE Argument;
  besARGUMENTS("iii")
    &fd,&reg,&data
  besARGEND
  rtn = wiringPiI2CWriteReg8(fd,reg,data);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CWriteReg16)
  int fd,reg,data,rtn;
  VARIABLE Argument;
  besARGUMENTS("iii")
    &fd,&reg,&data
  besARGEND
  rtn = wiringPiI2CWriteReg16(fd,reg,data);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CSetupInterface)
  const char *device;
  int devId,rtn;
  VARIABLE Argument;
  besARGUMENTS("zi")
    &device,&devId
  besARGEND
  rtn = wiringPiI2CSetupInterface(device,devId);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiI2CSetup)
  const int devId;
  int rtn;
  VARIABLE Argument;
  besARGUMENTS("i")
    &devId
  besARGEND
  rtn = wiringPiI2CSetup(devId);
  besRETURN_LONG(rtn);
besEND



// Shift Library


besFUNCTION(sb_shiftIn)
  VARIABLE Argument;
  uint8_t dPin,cPin,order,rtn;
  besARGUMENTS("iii")
    &dPin,&cPin,&order
  besARGEND
  rtn = shiftIn(dPin,cPin,order);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_shiftOut)
  VARIABLE Argument;
  uint8_t dPin,cPin,order,val;
  besARGUMENTS("iiii")
    &dPin,&cPin,&order,&val
  besARGEND
  shiftOut(dPin,cPin,order,val);
  besRETURNVALUE = NULL;
besEND



// SPI Library


besFUNCTION(sb_wiringPiSPIGetFd)
  int channel,rtn;
  VARIABLE Argument;
  besARGUMENTS("i")
    &channel
  besARGEND
  rtn = wiringPiSPIGetFd(channel);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiSPIDataRW)
  int channel,len,rtn;
  unsigned char *data;
  VARIABLE Argument;
  besARGUMENTS("izi")
    &channel,&data,&len
  besARGEND
  rtn = wiringPiSPIDataRW(channel,data,len);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiSPISetupMode)
  int channel,speed,mode,rtn;
  VARIABLE Argument;
  besARGUMENTS("iii")
    &channel,&speed,&mode
  besARGEND
  rtn = wiringPiSPISetupMode(channel,speed,mode);
  besRETURN_LONG(rtn);
besEND


besFUNCTION(sb_wiringPiSPISetup)
  int channel,speed,rtn;
  VARIABLE Argument;
  besARGUMENTS("ii")
    &channel,&speed
  besARGEND
  rtn = wiringPiSPISetup(channel,speed);
  besRETURN_LONG(rtn);
besEND



/*****************
 Utility Functions
*****************/

besFUNCTION(sb_msSleep)
  //DIM AS int msval, t;
  long t;
  besARGUMENTS("i")
    &t
  besARGEND
  usleep(t);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_delay)
  unsigned int howLong;
  besARGUMENTS("i")
    &howLong
  besARGEND
  delay(howLong);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_delayMicroseconds)
  unsigned int howLong;
  besARGUMENTS("i")
    &howLong
  besARGEND
  delayMicroseconds(howLong);
  besRETURNVALUE = NULL;
besEND


besFUNCTION(sb_bin2int)
  const char* s;
  besARGUMENTS("z")
    &s
  besARGEND
  register unsigned char *p = s;
  register unsigned int   r = 0;
  while (p && *p ) {
    r <<= 1;
    r += (unsigned int)((*p++) & 0x01);
  }
  besRETURN_LONG(r);
besEND





