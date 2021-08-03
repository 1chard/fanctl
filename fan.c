#include "fanctl.h"
#include <signal.h>
#include <unistd.h>

result_t filenameGuess(char* startingName){ //this function swap '?' with a number of 0 >= x < 10
    FILE* directory;
    char* toModify = strchr(startingName, '?');

    if(!toModify)
        return RESULT_FAIL;

    for(int i=0; i < 10; ++i){
        *toModify = toChar(i);

        directory = fopen(startingName, "r");

        if(directory){
            puts(startingName);
            return RESULT_TRUE;
        }

    }

    return RESULT_FAIL;
}
result_t generic100to255fanCtl(char* directoryOfGenericFan,Argument* fansValues, int fansValueIndex){
    int fanTemperature;

    char nameTemperatureValue[100];
    strcpy(nameTemperatureValue, directoryOfGenericFan);
    strcat(nameTemperatureValue, "temp1_input");
    fileTemperatureValue = fopen(nameTemperatureValue, "r");

    {//for ram save

        char nameAutoFanControl[100];
        strcpy(nameAutoFanControl, directoryOfGenericFan);
        strcat(nameAutoFanControl, "pwm1_enable");
        fileAutoFanControl = fopen(nameAutoFanControl, "w");


        char nameManualFanControl[100];
        strcpy(nameManualFanControl, directoryOfGenericFan);
        strcat(nameManualFanControl, "pwm1");
        fileManualFanControl = fopen(nameManualFanControl, "w");

    }

    if(!fileManualFanControl){
        printf("Error: %s.\n", strerror(errno));
        return RESULT_FAIL;
    }

    argumentsToExit = bitOn(argumentsToExit, 7);


    fprintf(fileAutoFanControl, "%d", 1);
    rewind(fileAutoFanControl);
    argumentsToExit = bitOn(argumentsToExit, 6);

    while(1){
        fscanf(fileTemperatureValue, "%d", &fanTemperature);
        fclose(fileTemperatureValue);
        fanTemperature /= 1000;


        for(; fanTemperature != 0; --fanTemperature){


            for(int i=0; i<fansValueIndex; ++i){
                if(fanTemperature == fansValues[i].temp){
                    fprintf(fileManualFanControl, "%d", conv100to255(fansValues[i].fan));
                    goto endLoop;
                }
            }
        }


endLoop:;

        rewind(fileManualFanControl);
        sleep(sleepTime);
        fileTemperatureValue = fopen(nameTemperatureValue, "r");
    }
}

result_t asusFanCtl(Argument* fansValues, int fansValueIndex){
    char directoryOfAsusFan[] = "/sys/devices/platform/asus-nb-wmi/hwmon/hwmon?/"; //array recommended
    result_t result = filenameGuess(directoryOfAsusFan);

    if(result == RESULT_FAIL)
        return RESULT_FAIL;

    result = generic100to255fanCtl(directoryOfAsusFan, fansValues, fansValueIndex);

    return result;
}

result_t startFanCtl(const char* fanName, Argument* fansValues, int fansValueIndex){
    result_t result = 0;

    if(!strcmp(fanName, "asus"))
        result = asusFanCtl(fansValues, fansValueIndex);
    else if(!strcmp(fanName, "amdgpu"))
        result = 0;
    else{
        result = RESULT_FAIL;
    }

    return result;

}
