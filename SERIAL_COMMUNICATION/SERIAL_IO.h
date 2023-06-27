#define PARM_COUNT 5

#ifdef _Serial_IO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

boolean stringComplete = false;

char  inString[256];
byte inStringPtr = 0;

char inData[256];
int string_index = 0;
#define PARM_COUNT 2
char* inputParmBuff[PARM_COUNT];

#else

extern void SetupSerialPC();
extern void HandleSerialIO();

#endif