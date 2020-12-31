#ifndef SOURCE_FILE_STRUCTURE_DEFS_H
#define SOURCE_FILE_STRUCTURE_DEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct SourceFile
{
    char* name;
} SourceFile;

typedef struct SourceFileList
{
    int size;
    SourceFile* files;
} SourceFileList;

#endif