/* Raspberry Pi Sense HAT
UXLIBS: -lpthread -lm -lc /usr/lib/gcc/arm-linux-gnueabihf/6/libgcc.a
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "../../basext.h"


/***************************
 Extension Module Functions
***************************/

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


/***************
 GPIO Functions
***************/

// I2C file handles
static int file_bus = -1; // i2c bus
static int file_hum = -1; // humidity/temp sensor
static int file_pres = -1; // pressure sensor
static int file_acc = -1; // accelerometer/gyro
static int file_mag = -1; // magnetometer

static int i2cRead(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen);
static int i2cWrite(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen);
// humidity/temp calibration values
static int H0_rH_x2, H1_rH_x2, T0_degC_x8;
static int T1_degC_x8, H0_T0_OUT;
static int H1_T0_OUT, T0_OUT, T1_OUT;

static int i2cRead(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen)
{
int rc;

  rc = write(iHandle, &ucAddr, 1);
  if (rc == 1)
  {
    rc = read(iHandle, buf, iLen);
  }
  return rc;
} /* i2cRead() */

static int i2cWrite(int iHandle, unsigned char ucAddr, unsigned char *buf, int iLen)
{
unsigned char ucTemp[512];
int rc;

  if (iLen > 511 || iLen < 1 || buf == NULL)
    return -1; // invalid write

  ucTemp[0] = ucAddr; // send the register number first
  memcpy(&ucTemp[1], buf, iLen); // followed by the data
  rc = write(iHandle, ucTemp, iLen+1);
  return rc-1;

} /* i2cWrite() */


besFUNCTION(shInit)
  int iChannel;

  besARGUMENTS("i")
    &iChannel
  besARGEND

  unsigned char ucTemp[32];
  char filename[32];

  sprintf(filename, "/dev/i2c-%d", iChannel);
  if ((file_bus = open(filename, O_RDWR)) < 0)
  {
    fprintf(stderr, "Failed to open the i2c bus\n");
    besRETURN_LONG(-1);
  }


  file_acc = open(filename, O_RDWR);
  if (ioctl(file_acc, I2C_SLAVE, 0x6a) < 0)
  {
    fprintf(stderr, "Failed to acquire bus for accelerometer\n");
    goto badexit;
  }

  file_mag = open(filename, O_RDWR);
  if (ioctl(file_mag, I2C_SLAVE, 0x1c) < 0)
  {
    fprintf(stderr, "Failed to acquire bus for magnetometer\n");
    goto badexit;
  }

  file_hum = open(filename, O_RDWR);
  if (ioctl(file_hum, I2C_SLAVE, 0x5f) < 0)
  {
    fprintf(stderr, "Failed to acquire bus for Humidity sensor\n");
    goto badexit;
  }
  file_pres = open(filename, O_RDWR);
  if (ioctl(file_pres, I2C_SLAVE, 0x5C) < 0)
  {
    fprintf(stderr, "Failed to aquire bus for Pressure sensor\n");
    goto badexit;
  }
  // Prepare humidity sensor
  i2cRead(file_hum, 0x10, ucTemp, 1); // AV_CONF
  ucTemp[0] &= 0xc0;
  ucTemp[0] |= 0x1b; // avgt=16, avgh=32
  i2cWrite(file_hum, 0x10, ucTemp, 1);

  i2cRead(file_hum, 0x20+0x80, ucTemp, 3); // get CTRL_REG 1-3
  ucTemp[0] &= 0x78; // keep reserved bits
  ucTemp[0] |= 0x81; // turn on + 1Hz sample rate
  ucTemp[1] &= 0x7c; // turn off heater + boot + one shot
  i2cWrite(file_hum, 0x20+0x80, ucTemp, 3); // turn on + set sample rate

  // Get the H/T calibration values
  i2cRead(file_hum, 0x30+0x80, ucTemp, 16);
  H0_rH_x2 = ucTemp[0];
  H1_rH_x2 = ucTemp[1];
  T0_degC_x8 = ucTemp[2];
  T1_degC_x8 = ucTemp[3];
  T0_degC_x8 |= ((ucTemp[5] & 0x3) << 8); // 2 msb bits
  T1_degC_x8 |= ((ucTemp[5] & 0xc) << 6);
  H0_T0_OUT = ucTemp[6] | (ucTemp[7] << 8);
  H1_T0_OUT = ucTemp[10] | (ucTemp[11] << 8);
  T0_OUT = ucTemp[12] | (ucTemp[13] << 8);
  T1_OUT = ucTemp[14] | (ucTemp[15] << 8);
  if (H0_T0_OUT > 32767) H0_T0_OUT -= 65536; // signed
  if (H1_T0_OUT > 32767) H1_T0_OUT -= 65536;
  if (T0_OUT > 32767) T0_OUT -= 65536;
  if (T1_OUT > 32767) T1_OUT -= 65536;

  // prepare pressure sensor
  ucTemp[0] = 0x90; // turn on and set 1Hz update
  i2cWrite(file_pres, 0x20, ucTemp, 1);

  // Init magnetometer
  ucTemp[0] = 0x48; // output data rate/power mode
  ucTemp[1] = 0x00; // default scale
  ucTemp[2] = 0x00; // continuous conversion
  ucTemp[3] = 0x08; // high performance mode
  i2cWrite(file_mag, 0x20+0x80, ucTemp, 4);

  // Init accelerometer/gyroscope
  ucTemp[0] = 0x60; // 119hz accel
  i2cWrite(file_acc, 0x20, ucTemp, 1);
  ucTemp[0] = 0x38; // enable gyro on all axes
  i2cWrite(file_acc, 0x1e, ucTemp, 1);
        ucTemp[0] = 0x28; // data rate + full scale + bw selection
  // bits:        ODR_G2 | ODR_G1 | ODR_G0 | FS_G1 | FS_G0 | 0 | BW_G1 | BW_G0
  // 0x28 = 14.9hz, 500dps
  i2cWrite(file_acc, 0x10, ucTemp, 1); // gyro ctrl_reg1

  besRETURN_LONG(1);

  // problems opening the I2C handles/addresses
  badexit:
  if (file_hum != -1)
  {
    close(file_hum);
    file_hum = -1;
  }
  if (file_pres != -1)
  {
    close(file_pres);
    file_pres = -1;
  }
  if (file_acc != -1)
  {
    close(file_acc);
    file_acc = -1;
  }
  if (file_mag != -1)
  {
    close(file_mag);
    file_mag = -1;
  }
  besRETURN_LONG(0);
besEND


besFUNCTION(shGetAccel)
  VARIABLE Argument1;
  VARIABLE Argument2;
  VARIABLE Argument3;
  unsigned long __refcount_;
  LEFTVALUE Lval1;
  LEFTVALUE Lval2;
  LEFTVALUE Lval3;

  Argument1 = besARGUMENT(1);
  besLEFTVALUE(Argument1,Lval1);
  besRELEASE(*Lval1);
  *Lval1 = NULL;

  Argument2 = besARGUMENT(2);
  besLEFTVALUE(Argument2,Lval2);
  besRELEASE(*Lval2);
  *Lval2 = NULL;

  Argument3 = besARGUMENT(3);
  besLEFTVALUE(Argument3,Lval3);
  besRELEASE(*Lval3);
  *Lval3 = NULL;

  unsigned char ucTemp[8];
  int rc;

  *Lval1 = besNEWLONG;
  *Lval2 = besNEWLONG;
  *Lval3 = besNEWLONG;

  rc = i2cRead(file_acc, 0x28+0x80, ucTemp, 6);
  if (rc == 6)
  {
    int x, y, z;

    x = ucTemp[0] + (ucTemp[1] << 8);
    y = ucTemp[2] + (ucTemp[3] << 8);
    z = ucTemp[4] + (ucTemp[5] << 8);
    // fix the signed values
    if (x > 32767) x -= 65536;
    if (y > 32767) y -= 65536;
    if (z > 32767) z -= 65536;

    LONGVALUE(*Lval1) = x;
    LONGVALUE(*Lval2) = y;
    LONGVALUE(*Lval3) = z;
    besRETURN_LONG(1);
  }
  besRETURN_LONG(0);
besEND


besFUNCTION(shGetGyro)
  VARIABLE Argument1;
  VARIABLE Argument2;
  VARIABLE Argument3;
  unsigned long __refcount_;
  LEFTVALUE Lval1;
  LEFTVALUE Lval2;
  LEFTVALUE Lval3;

  Argument1 = besARGUMENT(1);
  besLEFTVALUE(Argument1,Lval1);
  besRELEASE(*Lval1);
  *Lval1 = NULL;

  Argument2 = besARGUMENT(2);
  besLEFTVALUE(Argument2,Lval2);
  besRELEASE(*Lval2);
  *Lval2 = NULL;

  Argument3 = besARGUMENT(3);
  besLEFTVALUE(Argument3,Lval3);
  besRELEASE(*Lval3);
  *Lval3 = NULL;

  unsigned char ucTemp[8];
  int rc;

  *Lval1 = besNEWLONG;
  *Lval2 = besNEWLONG;
  *Lval3 = besNEWLONG;

  rc = i2cRead(file_acc, 0x18+0x80, ucTemp, 6);
  if (rc == 6)
  {
    LONGVALUE(*Lval1) = ucTemp[0] + (ucTemp[1] << 8);
    LONGVALUE(*Lval2) = ucTemp[2] + (ucTemp[3] << 8);
    LONGVALUE(*Lval3) = ucTemp[4] + (ucTemp[5] << 8);
    besRETURN_LONG(1);
  }
  besRETURN_LONG(0);
besEND


besFUNCTION(shGetMagneto)
  VARIABLE Argument1;
  VARIABLE Argument2;
  VARIABLE Argument3;
  unsigned long __refcount_;
  LEFTVALUE Lval1;
  LEFTVALUE Lval2;
  LEFTVALUE Lval3;

  Argument1 = besARGUMENT(1);
  besLEFTVALUE(Argument1,Lval1);
  besRELEASE(*Lval1);
  *Lval1 = NULL;

  Argument2 = besARGUMENT(2);
  besLEFTVALUE(Argument2,Lval2);
  besRELEASE(*Lval2);
  *Lval2 = NULL;

  Argument3 = besARGUMENT(3);
  besLEFTVALUE(Argument3,Lval3);
  besRELEASE(*Lval3);
  *Lval3 = NULL;

  unsigned char ucTemp[8];
  int rc;

  *Lval1 = besNEWLONG;
  *Lval2 = besNEWLONG;
  *Lval3 = besNEWLONG;

  rc = i2cRead(file_mag, 0x28+0x80, ucTemp, 6);
  if (rc == 6)
  {
    int x, y, z;
    x = ucTemp[0] + (ucTemp[1] << 8);
    y = ucTemp[2] + (ucTemp[3] << 8);
    z = ucTemp[4] + (ucTemp[5] << 8);
    // fix signed values
    if (x > 32767) x -= 65536;
    if (y > 32767) y -= 65536;
    if (z > 32767) z -= 65536;

    LONGVALUE(*Lval1) = x;
    LONGVALUE(*Lval2) = y;
    LONGVALUE(*Lval3) = z;
    besRETURN_LONG(1);
  }
  besRETURN_LONG(0);
besEND

besFUNCTION(shGetPressure)
  VARIABLE Argument1;
  VARIABLE Argument2;
  unsigned long __refcount_;
  LEFTVALUE Lval1;
  LEFTVALUE Lval2;

  Argument1 = besARGUMENT(1);
  besLEFTVALUE(Argument1,Lval1);
  besRELEASE(*Lval1);
  *Lval1 = NULL;

  Argument2 = besARGUMENT(2);
  besLEFTVALUE(Argument2,Lval2);
  besRELEASE(*Lval2);
  *Lval2 = NULL;

  unsigned char ucTemp[8];
  int rc, P, T;

  *Lval1 = besNEWLONG;
  *Lval2 = besNEWLONG;

  if (file_pres != -1)
  {
    rc = i2cRead(file_pres, 0x28+0x80, ucTemp, 5);
    if (rc == 5)
    {
      P = ucTemp[0] + (ucTemp[1]<<8) + (ucTemp[2]<<16);
      LONGVALUE(*Lval1) = P / 4096; //hPa
      T = ucTemp[3] + (ucTemp[4] << 8);
      if (T > 32767) T -= 65536; // twos compliment
      T = 425 + (T / 48); // 42.5 + T value/480
      LONGVALUE(*Lval2) = T;
    }
    besRETURN_LONG(1);
  }
  besRETURN_LONG(0);
besEND


besFUNCTION(shGetTempHumid)
  VARIABLE Argument1;
  VARIABLE Argument2;
  unsigned long __refcount_;
  LEFTVALUE Lval1;
  LEFTVALUE Lval2;

  Argument1 = besARGUMENT(1);
  besLEFTVALUE(Argument1,Lval1);
  besRELEASE(*Lval1);
  *Lval1 = NULL;

  Argument2 = besARGUMENT(2);
  besLEFTVALUE(Argument2,Lval2);
  besRELEASE(*Lval2);
  *Lval2 = NULL;

  unsigned char ucTemp[4];
  int rc;
  int H_T_out, T_out, T0_degC, T1_degC;
  int H0_rh, H1_rh;
  int tmp;

  *Lval1 = besNEWLONG;
  *Lval2 = besNEWLONG;

  rc = i2cRead(file_hum, 0x28+0x80, ucTemp, 4);
  if (rc == 4)
  {
    H_T_out = ucTemp[0] + (ucTemp[1] << 8);
    T_out = ucTemp[2] + (ucTemp[3] << 8);
    if (H_T_out > 32767) H_T_out -=65536;
    if (T_out > 32767) T_out -= 65536;
    T0_degC = T0_degC_x8 / 8;
    T1_degC = T1_degC_x8 / 8;
    H0_rh = H0_rH_x2 / 2;
    H1_rh = H1_rH_x2 / 2;
    tmp = (H_T_out - H0_T0_OUT) * (H1_rh - H0_rh)*10;
    LONGVALUE(*Lval2) = tmp / (H1_T0_OUT - H0_T0_OUT) + H0_rh*10;
    tmp = (T_out - T0_OUT) * (T1_degC - T0_degC)*10;
    LONGVALUE(*Lval1) = tmp / (T1_OUT - T0_OUT) + T0_degC*10;
    besRETURN_LONG(1);
  }
  besRETURN_LONG(0); // not ready
besEND

besFUNCTION(shShutdown)
  // Close all I2C file handles
  if (file_bus != -1) close(file_bus);
  if (file_hum != -1) close(file_hum);
  if (file_pres != -1) close(file_pres);
  if (file_acc != -1) close(file_acc);
  if (file_mag != -1) close(file_mag);
  file_bus = file_hum = file_pres = file_acc = file_mag = -1;
besEND


