#include "fanctl.h"

result_t bracketsGetTemperatureConfig(FILE*, Argument*, int*);
result_t getTemperatureInputFromFile(char*, FILE*, u_int8_t*);

result_t getTempsConfig(const char* configFilename, char* nameOfFanToWork, Argument* temperaturesPointer, int* temperatureIndex_ptr) {
    FILE* configFile = fopen(configFilename, "r");
    char ch = '\0';

    if(!configFile){
        switch(errno){
        case ENOENT:
            printf("%s does not exist.\n", configFilename);
            return RESULT_FAIL;

        case EPERM:
            printf("%s is not readable, check its permissions.\n", configFilename);
            return RESULT_FAIL;

        default:
            printf("Error: %s.\n", strerror(errno));
            return RESULT_FAIL;
        }
    }

    while (1) { //line loop
        //character loop

        ch = fgetc(configFile);

        if(feof(configFile))
            return RESULT_TRUE;

        switch (ch) {
        case '#': //ignore theses lines
            while(ch != '\n')
                ch = fgetc(configFile);

            break;

        case '\t': //ignore formatting characters, note that this case will accept input things like [   5,  5   ]
        case ' ':
        case '\n':
            break;

        case '[':
        case ']':
            printf("Error: brackets '[]' outside a fan.\n");
            return RESULT_FAIL;

        default:{
            {
                int i=0;
                for(; isgraph(ch) && ch != ':'; ++i){
                    nameOfFanToWork[i] = ch;
                    ch = fgetc(configFile);
                }

                nameOfFanToWork[i] = '\0';

                puts(nameOfFanToWork);

                //ch = fgetc(configFile);
            }

            result_t result = RESULT_CONTINUE;

            //TODO move this piece of code to another function
            while(result == RESULT_CONTINUE){//inner
                result = bracketsGetTemperatureConfig(configFile, temperaturesPointer, temperatureIndex_ptr);

                if(feof(configFile))
                    return RESULT_TRUE;

            }

        }
        }

        //ch = fgetc(configFile);
    }

    fclose(configFile);
    return RESULT_TRUE;
}

result_t bracketsGetTemperatureConfig(FILE* configFile, Argument* temperaturesToReturn, int* temperaturesIndex){
    char ch = fgetc(configFile);

    switch(ch){
    case '\t': //ignore formatting characters
    case ' ':
    case '\n':
        break;

    case '#': //ignore theses lines
        while(ch != '\n')
            ch = fgetc(configFile);
        break;

    case '[':{
        int argsTaken = 0;
        result_t result = RESULT_TRUE;

        while(ch != ']'){
            ch = fgetc(configFile);

            switch(ch){
            case '\t': //ignore formatting characters
            case ' ':
            case '\n':
                break;

                /*
            case '#': //ignore theses lines
                while(ch != '\n')
                    ch = fgetc(configFile);

                break;
                */

            case ',':
                if(argsTaken == 0){
                    puts("Error: ',' (comma) without first argument.\n");
                    *temperaturesIndex = ERROR_IOCONTROL;
                    return RESULT_FAIL;
                }

                else if(argsTaken == 1)
                    break;

                else if(argsTaken == 2){
                    puts("Error: extra comma (,).\n");
                    *temperaturesIndex = ERROR_IOCONTROL;
                    return RESULT_FAIL;
                }

                break;

            case ']':
                printf("tofix");
                return RESULT_CONTINUE;

            default:{

                switch (argsTaken) {
                case 0: //note, value can be -99 to 999
                    result = getTemperatureInputFromFile(&ch, configFile, &(temperaturesToReturn[*temperaturesIndex].temp) );

                    if(result == RESULT_FAIL){
                        *temperaturesIndex = ERROR_IOCONTROL;
                        return result;
                    }

                    ++argsTaken;

                    if(result == RESULT_STOP){
                        return RESULT_CONTINUE;
                    }
                    break;

                case 1:
                    result = getTemperatureInputFromFile(&ch, configFile, &(temperaturesToReturn[*temperaturesIndex].fan) );

                    if(result == RESULT_FAIL){
                        *temperaturesIndex = ERROR_IOCONTROL;
                        return result;
                    }

                    ++argsTaken;

                    if(result == RESULT_STOP){
                        (*temperaturesIndex)++;
                        return RESULT_CONTINUE;
                    }
                    break;

                default:
                    puts("Error: extra items on brackets.\n");
                    *temperaturesIndex = ERROR_IOCONTROL;
                    return RESULT_FAIL;


                }
            }

                //ch = fgetc(configFile);

            }
        }


        if(argsTaken < 2){
            puts("Error: failed to get two arguments.\n");
            *temperaturesIndex = ERROR_IOCONTROL;
            return RESULT_FAIL;
        }

        (*temperaturesIndex)++;
    }

        return RESULT_TRUE;
    }

    return RESULT_TRUE;
}

result_t getTemperatureInputFromFile(char* ch, FILE* configFile, u_int8_t* toReceiveInput){
    char argTempOrFan[4] = "\0\0\0\0";

    for(int i=0; i < 3; ++i){
        if(isdigit(*ch)){
            argTempOrFan[i] = *ch;
            *ch = fgetc(configFile);
        }

        else if(*ch == ','){
            *(toReceiveInput) = parseInt(argTempOrFan);
            return RESULT_CONTINUE;
        }

        else if(*ch == ']'){
            *(toReceiveInput) = parseInt(argTempOrFan);
            return RESULT_STOP;
        }

        else if(*ch == ' ' || *ch == '\t')
            continue;

        else if(*ch == '-'){
            puts("Error: negative output.\n");
            return RESULT_FAIL;
        }


        else{
            printf("Error: %c on argument.\n", *ch);
            return RESULT_FAIL;
        }
    }

    *(toReceiveInput) = parseInt(argTempOrFan);
    return RESULT_TRUE;
}
