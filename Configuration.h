#ifndef TDF_CONFIG_H
#define TDF_CONFIG_H

#include "Types.h"

typedef struct Config_T {
    char* dirToParse;
    Size parseDirLen;
    i32 orderTodoBy;
    i32 orderFileBy;
    Size maxLineMem;
    bool terminalColor;
    bool printFilename;
} Config;

void initConfig();
void cleanupConfig();

i32 parseArg(char* arg);

#endif