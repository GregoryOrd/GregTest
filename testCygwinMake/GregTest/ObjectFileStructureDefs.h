#ifndef OBJECT_FILE_STRUCTURE_DEFS_H
#define OBJECT_FILE_STRUCTURE_DEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

#include <stdbool.h>

typedef struct ObjectFile
{
    char* name;
    bool isFromSource;
} ObjectFile;

typedef struct ObjectFileList
{
    int size;
    ObjectFile* files;
} ObjectFileList;

#endif