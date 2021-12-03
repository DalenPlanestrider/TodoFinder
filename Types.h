#ifndef TDF_TYPES_H
#define TDF_TYPES_H

#define DEBUG 0

#if defined(__linux)
typedef unsigned long Size;
typedef int i32;
typedef unsigned int u32;
typedef char bool;
#elif defined(_WIN32)
typedef unsigned long long Size;
typedef int i32;
typedef char bool;
#endif


#define ORDERTYPE_NONE 0
#define ORDERTYPE_FILE 1
#define ORDERTYPE_SEV 2
#define ORDERTYPE_ALPHABET 3
#define ORDERTYPE_LENGTH 4
#define ORDERTYPE_DISCOVERY 5


#define ARG_SUCCESS 0
#define ARG_UNRECOGNIZED 1
#define ARG_QUIT 2


#define true 1
#define false 0

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

typedef struct LenStr_T {
    Size length;
    char* str;
} LenStr;


typedef struct TodoEntry_T {
    i32 fileIndex;
    u32 severity;
    char* text;
} TodoEntry;

#endif //TDF_TYPES_H

