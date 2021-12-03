#include "Globals.h"

#include <stdlib.h>

char* dirToParse;
Size parseDirLen;
i32 orderTodoBy;
i32 orderFileBy;
Size maxLineMem;
Config config;

Size skipDirCount;
LenStr skipDirs[CONF_STORAGE_COUNT];

Size whiteExtCount;
LenStr whiteExt[CONF_STORAGE_COUNT];

Size blackExtCount;
LenStr blackExt[CONF_STORAGE_COUNT];

Size todoFileCount;
char* filePaths[MAX_TODOS];

Size todoCount;
TodoEntry foundTodos[MAX_TODOS];

void cleanupGlobals() {
    for(Size i = 0; i < todoFileCount; i++) {
        free(filePaths[i]);
    }

    for(Size i = 0; i < todoCount; i++) {
        free(foundTodos[i].text);
    }

    for(Size i = 0; i < blackExtCount; i++) {
        free(blackExt[i].str);
    }

    for(Size i = 0; i < whiteExtCount; i++) {
        free(whiteExt[i].str);
    }
}