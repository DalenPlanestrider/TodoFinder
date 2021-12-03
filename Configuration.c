#include "Configuration.h"
#include "Globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


char* argFullName[] = {
    "--help",
    "--base-dir",
    "--exclude-dir",
    "--add-extension",
    "--exclude-extension",
    "--orderby-file",
    "--orderby-severity",
    "--enable-terminalcolor",
    "--no-filename",
    "--order-filenameby"
};

char* argShortName[] = {
    "-h",
    "-bd",
    "-ed",
    "-ae",
    "-ee",
    "-obf",
    "-obs",
    "-etc",
    "-nfn",
    "-ofn"
};

char* argDesc[] = {
    "Display this message",
    "Set the base directory to search",
    "Add a directory name to skip searching",
    "Add a file extension to the whitelist",
    "Add a file extension to the blacklist",
    "Order output by file grouping",
    "Order output by severity",
    "Enable colored output to the terminal",
    "Disable the filename printing",
    "Order the filenames by: [alphabetical, discovery, length]"
};



bool printHelp(char* p);
bool setBaseDir(char* p);
bool addSkipDir(char* p);
bool addWhiteExt(char* p);
bool addBlackExt(char* p);
bool orderFile(char* p);
bool orderSev(char* p);
bool setTermColor(char* p);
bool disableFilename(char* p);
bool orderFilename(char* p);

bool (*argFuncs[])(char*) = {
    printHelp,
    setBaseDir,
    addSkipDir,
    addWhiteExt,
    addBlackExt,
    orderFile,
    orderSev,
    setTermColor,
    disableFilename,
    orderFilename
};

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

void initConfig() {
    config.dirToParse = getcwd(malloc(PATH_MAX + 1), PATH_MAX + 1);
    if (config.dirToParse) { 
        config.parseDirLen = strlen(config.dirToParse);
    }

    config.orderTodoBy = ORDERTYPE_FILE;
    config.orderFileBy = ORDERTYPE_DISCOVERY;
    config.maxLineMem = 100;
    config.terminalColor = false;
    config.printFilename = true;

    addBlackExt(".jpeg");
    addBlackExt(".jpg");
    addBlackExt(".tiff");
    addBlackExt(".tif");
    addBlackExt(".png");
    addBlackExt(".psd");
    addBlackExt(".xcf");
    addBlackExt(".ai");
    addBlackExt(".cdr");
    addBlackExt(".gif");
    addBlackExt(".obj");
    addBlackExt(".blend");
    addBlackExt(".gltf");

}

void cleanupConfig() {
    free(config.dirToParse);
}

i32 parseArg(char* arg) {
    if (!arg) {
        return false;
    }

    Size argDefs = sizeof(argFullName) / sizeof(argFullName[0]);
    Size alen = strlen(arg);

    for(i32 i = 0; i < argDefs; i++) {
        Size slen = strlen(argShortName[i]);
        Size llen = strlen(argFullName[i]);

        //Size mslen = slen > alen ? alen : slen;
        //Size mllen = llen > alen ? alen : llen;
        Size mslen = MIN(slen, alen);
        Size mllen = MIN(llen, alen);

        if (strncmp(argFullName[i], arg, mllen) == 0) {
            char* toPass = arg + mllen;
            if (toPass[0] == '=') {
                toPass++;
            }

            if (!argFuncs[i](toPass)) {
                return ARG_QUIT;
            }
            else {
                return ARG_SUCCESS;
            }
        }
        else if (strncmp(argShortName[i], arg, mslen) == 0) {
            char* toPass = arg + mslen;
            if (toPass[0] == '=') {
                toPass++;
            }

            if (!argFuncs[i](toPass)) {
                return ARG_QUIT;
            }
            else {
                return ARG_SUCCESS;
            }
        }
    }

    return ARG_UNRECOGNIZED;
}



bool setBaseDir(char* p) {
    if(p) { 
        free(config.dirToParse);
        config.dirToParse = p;
        config.parseDirLen = strlen(p);
    }

    return true;
}

bool addSkipDir(char* p) {
    if (skipDirCount == CONF_STORAGE_COUNT) { 
        fprintf(stderr, "Max skip dirs is %d\n", CONF_STORAGE_COUNT);
        return false;
    }

    if (p) {
        LenStr newstr;
        newstr.str = strdup(p);
        newstr.length = strlen(p);
        skipDirs[skipDirCount] = newstr;
        skipDirCount++;
    }

    return true;
}

bool addWhiteExt(char* p) {
    if (p) {
        LenStr x;
        x.str = strdup(p);
        x.length = strlen(p);
        whiteExt[whiteExtCount] = x;
        whiteExtCount++;
    }

    return true;
}

bool addBlackExt(char* p) {
    if (p) {
        LenStr x;
        x.str = strdup(p);
        x.length = strlen(p);
        blackExt[blackExtCount] = x;
        blackExtCount++;
    }

    return true;
}

bool orderFile(char* p) {
    config.orderTodoBy = ORDERTYPE_FILE;
    return true;
}

bool orderSev(char* p) {
    config.orderTodoBy = ORDERTYPE_SEV;
    return true;
}

bool setTermColor(char* p) {
    config.terminalColor = false;
    return true;
}

bool disableFilename(char* p) {
    config.printFilename = false;
    return true;
}

bool orderFilename(char* p) {
    if (p) {
        if (strcmp(p, "length") == 0) {
            config.orderFileBy = ORDERTYPE_LENGTH;
        }
        else if (strcmp(p, "discovery") == 0) {
            config.orderFileBy = ORDERTYPE_DISCOVERY;
        } 
        else if (strcmp(p, "alphabetical")) {
            config.orderFileBy = ORDERTYPE_ALPHABET;
        }
    }

    return true;
}

bool printHelp(char* p) {
    Size argDefs = sizeof(argFullName) / sizeof(argFullName[0]);
    Size longestShort = 0;
    Size longestLong = 0;
    for(Size i = 0; i < argDefs; i++) {
        Size slen = strlen(argShortName[i]);
        Size llen = strlen(argFullName[i]);
        if (slen > longestShort) {
            longestShort = slen;
        }

        if (llen > longestLong) {
            longestLong = llen;
        }
    }

    char fmtBuff[100];
    snprintf(fmtBuff, 100, "%%-%lus    %%-%lus    %%s\n", longestShort, longestLong);


    for(Size i = 0; i < argDefs; i++) {
        //printf("%3s  %s %s\n", argShortName[i], argFullName[i], argDesc[i]);
        printf(fmtBuff, argShortName[i], argFullName[i], argDesc[i]);
    }

    return false;
}