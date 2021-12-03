#ifndef TDF_GLOBALS_H
#define TDF_GLOBALS_H

#include "Types.h"
#include "Configuration.h"

#define CONF_STORAGE_COUNT 256
#define MAX_TODOS 4096

extern Config config;

extern Size skipDirCount;
extern LenStr skipDirs[CONF_STORAGE_COUNT];

extern Size whiteExtCount;
extern LenStr whiteExt[CONF_STORAGE_COUNT];

extern Size blackExtCount;
extern LenStr blackExt[CONF_STORAGE_COUNT];

extern Size todoFileCount;
extern char* filePaths[MAX_TODOS];

extern Size todoCount;
extern TodoEntry foundTodos[MAX_TODOS];

void cleanupGlobals();

#endif
