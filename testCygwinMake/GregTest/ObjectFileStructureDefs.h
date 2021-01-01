#ifndef OBJECT_FILE_STRUCTURE_DEFS_H
#define OBJECT_FILE_STRUCTURE_DEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct ObjectFile
{
    char* name;
} ObjectFile;

typedef struct ObjectFileList
{
    int size;
    ObjectFile* files;
} ObjectFileList;

#endif