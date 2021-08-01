#include "fanctl.h"
#include "signal.h"

void sigintHandler(){

    if(bitStatus(argumentsToExit, 0))//write 2 to autocontrol
        fprintf(fileAutoFanControl, "%d", 2);


    if(bitStatus(argumentsToExit, 1)){ //close files
        fclose(fileAutoFanControl);
        fclose(fileManualFanControl);
    }



    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
    Argument argumentArray[64];
    char fanName[64];
    int argumentArrayIndex = 0;
    char nameOfConfigFile[512] = "/etc/fanctl.conf";


    //args manager, search --help first
    for(int i=1; i < argc && !strcmp(argv[i], "--help"); ++i){
        printf("Usage: %s (--help, --config-file, --sleep-time). \n"
               "\tOptions:\n"
               "\t --config-file=value\n"
               "\t\tUse 'value' as argument.\n", strrchr(argv[0], '/') + 1);
        return 0;
    }

    for(int i=1; i < argc; ++i){

        if(!strncmp(argv[i], "--config-file", 13)){


            if(argv[i][13] == '='){

                strncpy(nameOfConfigFile, (argv[i] + 14), 512);

            }
            else if(strlen(argv[i]) == 13 && (i + 1) < argc)
                strncpy(nameOfConfigFile, argv[++i], 512);
            else{
                puts("unformated argument.\n");
                return 1;
            }

            puts(nameOfConfigFile);
        }
    }

    signal(SIGINT, sigintHandler);

    result_t result = getTempsConfig(nameOfConfigFile, fanName, argumentArray, &argumentArrayIndex);

    if(result == RESULT_FAIL)
        raise(SIGINT);
    else if(argumentArrayIndex < 1){
        printf("Error: only %d arguments.\n", argumentArrayIndex);
        raise(SIGINT);
    }



    setFanMode(fanName, argumentArray, argumentArrayIndex);

    raise(SIGINT);
}
