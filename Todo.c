#include "Todo.h"
#include "Globals.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


#include "tinydir.h"

//FIXMEEE: test in a different file

u32 findOrMakeFileIndex(char* file) {
    for(Size i = 0; i < todoFileCount; i++) {
        if (strcmp(file, filePaths[i]) == 0) {
            return i;
        }
    }

    filePaths[todoFileCount] = strdup(file);
    todoFileCount++;
    return todoFileCount - 1;
}

Size parseLine(char* file, char* contents) {
    if (todoCount == MAX_TODOS) {
        return 0;
    }

    Size readpos = 0;

    // Lowercase the line
    while(contents[readpos] != '\n' && contents[readpos] != '\0') {
        if (contents[readpos] >= 'A' && contents[readpos] <= 'Z') {
            contents[readpos] += 32;
        }

        readpos++;
    }

    if (readpos < 5) {
        return 0;
    }

    for(Size i = 0; i < readpos - 5; i++) {
        if (strncmp("todo", contents + i, 4) == 0 || strncmp("fixme", contents + i, 5) == 0) {
            TodoEntry newtd;   
            newtd.severity = 1;
            newtd.fileIndex = findOrMakeFileIndex(file);

            bool wasTodo = false;
            if(strncmp("todo", contents + i, 4) == 0) {
                char* start = contents + i + 4;
                while(*start == 'o') {
                    newtd.severity++;
                    start++;
                }

                wasTodo = true;
            }
            else if (strncmp("fixme", contents + i, 5) == 0) {
                char* start = contents + i + 5;
                while(*start == 'e') {
                    newtd.severity++;
                    start++;
                }
            }

            char* tocopy = contents;
            tocopy += 2; // 2 to skip the //
            readpos -= 2;
            tocopy += wasTodo ? 4 : 5; // skip the trigger word
            readpos -= wasTodo ? 4 : 5;

            // pretty up our text output
            while(
                    *tocopy == 'o' ||
                    *tocopy == 'e' ||
                    *tocopy == ':' ||
                    *tocopy == ' ' ||
                    *tocopy == ',' ||
                    *tocopy == '_' ||
                    *tocopy == '-'
            )
            {
                tocopy++;
                readpos--;
            }


            Size mem = MIN(readpos, config.maxLineMem);
            newtd.text = (char*)malloc(mem + 1);
            newtd.text[mem] = '\0';
            memcpy(newtd.text, tocopy, mem);
            

            foundTodos[todoCount] = newtd;
            todoCount++;

            return readpos;
        }
    }

    return 0;
}

void parseFile(char* path) {
    Size len = strlen(path);

    if (whiteExtCount != 0) {
        bool matches = false;

        for(Size i = 0; i < whiteExtCount; i++) {
            if (strcmp(whiteExt[i].str, path + len - whiteExt[i].length) == 0) {
                matches = true;
                break;
            }
        }

        if (!matches) {
            return;
        }
    }

    if (blackExtCount != 0) {
        for(Size i = 0; i < blackExtCount; i++) {
            if (strcmp(blackExt[i].str, path + len - blackExt[i].length) == 0) {
                #if DEBUG
                printf("Skipping file on failed ext black: %s\n", path);
                #endif
                return;
            }
        }
    }


    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        #if DEBUG
        fprintf(stderr, "Failed to open file: %s\n", path);
        #endif
        return;
    }

    struct stat buf;
    fstat(fd, &buf);

    // open memory in both rw, but map private so changes here don't affect the file
    char* contents = (char*)mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);

    if (!contents || contents == MAP_FAILED) {
        #if DEBUG
        fprintf(stderr, "Failed to mmap file: %s\n", path);
        #endif

        return;
    }
    
    // Minus one on buffer so we don't run off the end with our slash check
    for(Size i = 0; i < buf.st_size - 1; i++) {
        if (contents[i] == '/' && contents[i + 1] == '/') {
            i += parseLine(path, contents + i);
        }
    }

    if (munmap(contents, buf.st_size) != 0) {
        #if DEBUG
        fprintf(stderr, "Failed to munmap file: %s\n", path);
        #endif
    }
}

void printTodos_recurse(char* path, Size depth) {
    #if DEBUG
    printf("Searching dir: %s\n", path);
    #endif

    if (depth >= 100) {
        fprintf(stderr, "Folder structure too deep, bailing");
        return;
    }

    tinydir_dir dir;
    tinydir_open(&dir, path);

    while(dir.has_next) {
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        
        // Skip unix . and .. paths to avoid infinite recursion + scanning unwanted dirs
        Size len = strlen(file.path);
        if (file.path[len - 1] != '.') {
            if (file.is_dir) {
                bool isSkipDir = false;

                for(Size i = 0; i < skipDirCount; i++) {
                    if (strcmp(skipDirs[i].str, file.path + len - skipDirs[i].length) == 0) { 
                        #if DEBUG
                        printf("Skipping dir %s\n", file.path);
                        #endif
                        isSkipDir = true;
                        break;
                    }
                }

                if (!isSkipDir) {
                    printTodos_recurse(file.path, depth + 1);
                }
            }
            else if (file.is_reg) {
                parseFile(file.path);       
            }
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

bool compareTodos(u32 x, u32 y) {
    if (config.orderTodoBy == ORDERTYPE_FILE) {
        if (config.orderFileBy == ORDERTYPE_DISCOVERY) {
            if (foundTodos[x].fileIndex < foundTodos[y].fileIndex) {
                return true;
            }
        }
        else if (config.orderFileBy == ORDERTYPE_LENGTH) {
            char* xn = filePaths[foundTodos[x].fileIndex];
            char* yn = filePaths[foundTodos[y].fileIndex];

            if (strlen(xn) > strlen(yn)) {
                return true;
            }
        }
        else if (config.orderFileBy == ORDERTYPE_ALPHABET) {
            char* xn = filePaths[foundTodos[x].fileIndex];
            char* yn = filePaths[foundTodos[y].fileIndex];

            if (strcmp(xn, yn) < 0) {
                return true;
            }
        }

        return false;
    }
    else if (config.orderTodoBy == ORDERTYPE_SEV) {
        if (foundTodos[x].severity < foundTodos[y].severity) {
            return true;
        }

        return false;
    }

    return false;
}

char* formatFolderName(char* name) {
    char* fileName = name;
    if (strncmp(fileName, config.dirToParse, config.parseDirLen) == 0) {
        fileName += config.parseDirLen;
        // just to make sure we're not starting on a / if it's not
        // the actual root dir
        if (fileName[0] == '/') { 
            fileName++;
        }
    }

    return fileName;
}

void printTodos(char* path) {
    printTodos_recurse(path, 0);

    // Sort the todos
    if (todoCount == 0) {
        return;
    }

    bool sorted = false;
    while(!sorted) {
        bool didSwap = false;

        for(Size i = 0; i < todoCount - 1; i++) {
            if (compareTodos(i, i + 1)) {
                TodoEntry cp;
                cp = foundTodos[i];
                foundTodos[i] = foundTodos[i + 1];
                foundTodos[i + 1] = cp;
                didSwap = true;
            }
        }

        if (!didSwap) {
            sorted = true;
        }
    }        

    for(Size i = 0; i < todoCount; i++) {
        if (config.printFilename && (i == 0 || foundTodos[i].fileIndex != foundTodos[i - 1].fileIndex)) {
            printf("%s\n", formatFolderName(filePaths[foundTodos[i].fileIndex]));
        }

        printf("\t[%d] %s\n", foundTodos[i].severity, foundTodos[i].text);
    }
}