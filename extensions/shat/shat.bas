' Raspberry Pi Sense HAT

MODULE SHAT

DECLARE SUB  ::Init          ALIAS  "shInit"          LIB  "shat"
DECLARE SUB  ::GetAccel      ALIAS  "shGetAccel"      LIB  "shat"
DECLARE SUB  ::GetGyro       ALIAS  "shGetGyro"       LIB  "shat"
DECLARE SUB  ::GetMagneto    ALIAS  "shGetMagneto"    LIB  "shat"
DECLARE SUB  ::GetPressure   ALIAS  "shGetPressure"   LIB  "shat"
DECLARE SUB  ::GetTempHumid  ALIAS  "shGetTempHumid"  LIB  "shat"
DECLARE SUB  ::Shutdown      ALIAS  "shShutdown"      LIB  "shat"

END MODULE


