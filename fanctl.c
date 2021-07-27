#include "fanctl.h"
#include <math.h>
#include <stdarg.h>
#include <limits.h>

//variables & pointers

byte argumentsToExit = 0b00000000;
FILE* fileAutoFanControl;
FILE* fileManualFanControl;
FILE* fileTemperatureValue;

//functions

int highest(int size, ...){
    int toReturn = INT_MIN;
    int tempArg;

    va_list list;

    va_start(list, size);

    for(int i=0; i<size; ++i){
        tempArg = va_arg(list, int);

        if(toReturn < tempArg)
            toReturn = tempArg;
    }

    va_end(list);

    return toReturn;

}

int conv100to255(int initialValue){
    return (51 * initialValue)/20;
}

char toChar(int number){
    if(number >= 0 && number < 10)
        return number + 48;

    return '\0';
}

int toInt(char ch){
    if(ch >= 48 && ch < 58)
        return ch - 48;

    return -1;
}

int parseInt(const char* string){
    int intReturn = 0;
    char shouldFlip = 0;
    char length = strlen(string);

    for(int i=0; i < length; ++i){
        if(isdigit(string[i]))
            intReturn += (int)pow(10, length - i - 1) * toInt(string[i]);
        else if(string[i] == '-'){
            if(i == 0)
                shouldFlip = 1;
            else{
                puts("Error: extra '-' sign.\n");
                return 0;
            }
        }
        else{
            printf("Error on character \"%c\".\n", string[i]);
            return 0;
        }
    }

    return (shouldFlip)? -intReturn : intReturn;
}

byte bitStatus(byte eightBitsArg, int bitPosition){
    return (1 << bitPosition) & eightBitsArg;
}

byte bitOn(byte eightBitsArg, int bitPosition){
    return (1 << bitPosition) | eightBitsArg;
}

byte bitOff(byte eightBitsArg, int bitPosition){
    return (1 << bitPosition) & ~eightBitsArg;
}
