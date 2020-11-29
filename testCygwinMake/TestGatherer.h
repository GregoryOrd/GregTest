#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct FileList{
    int size;
    char** files;
} FileList;

void loadFilePaths(FileList* fileList, const char* basePath);
void printFileList(const FileList fileList);
void addFileToList(FileList* list, const char* path);
void freeFileListFiles(FileList* list);
bool isTestDir(char* dirName);
bool isTestFile(char* dirName);
char* lowerString(char* str);

#endif