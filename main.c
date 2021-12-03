#include "Types.h"
#include "Globals.h"
#include "Configuration.h"
#include "Todo.h"

#include <stdio.h>

//TODO: a test
//TODOOOOOOOO another test
//FIXME - a third test


i32 main(i32 argc, char** argv) {
    initConfig();
    
    for(i32 i = 1; i < argc; i++) {
        i32 code = parseArg(argv[i]);

        if (code == ARG_UNRECOGNIZED) {
            fprintf(stderr, "Unrecognized argument: %s\n", argv[i]);
        }
        else if (code == ARG_QUIT) {
            return 0;
        }
    }

    printTodos(config.dirToParse);

    cleanupConfig();
    cleanupGlobals();

    return 0;
}









