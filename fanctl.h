#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef FANCTL_HEADER
#define FANCTL_HEADER

typedef struct{
    u_int8_t temp;
    u_int8_t fan;
} Argument;

enum common_returns{
    ERROR_IOCONTROL = -127,

    RESULT_FAIL = -1,
    RESULT_TRUE = 1,
    RESULT_STOP = 0,
    RESULT_CONTINUE = RESULT_TRUE
};

typedef signed char result_t;
typedef u_int8_t byte;

int highest(int __size, ...);
char toChar(int __integer);
int toInt(char __character);
int parseInt(const char* __string);
int conv100to255(int __100basevalue);
result_t startFanCtl(const char* fanName, Argument* fansValues, int fansValueIndex);
byte bitStatus(byte eightBitsArg, int bitPosition);
byte bitOn(byte eightBitsArg, int bitPosition);
byte bitOff(byte eightBitsArg, int bitPosition);
byte bitFlip(byte eightBitsArg, int bitPosition);
result_t getTempsConfig(const char* configFilename, char* nameOfFanToWork, Argument* temperaturesPointer, int* temperatureIndex_ptr);

extern unsigned int sleepTime;
extern byte argumentsToExit;
/*      closefilesbelow.                        parsefileerror.
 *      v                                       v
 *  0b  0 0 0 0 0                            0  0          0
 *        ^                                  ^             ^
 *        clearfancontrol: write 2 to file.  write error.  other errors.
 */

extern FILE* fileAutoFanControl;
extern FILE* fileManualFanControl;
extern FILE* fileTemperatureValue;

#endif
