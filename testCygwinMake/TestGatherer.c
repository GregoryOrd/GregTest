#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct FileList{
    int size;
    char** files;
} FileList;

void loadFilePaths(FileList* fileList, const char* basePath);
void printFileList(const FileList fileList);
void addFileToList(FileList* list, const char* path);
void freeFileListFiles(FileList* list);


int main(void) 
{ 
    const char dir[WINDOWS_MAX_PATH_LENGTH] = ".";

    FileList* fileList = (FileList*)malloc(sizeof(FileList*));
    fileList->size = 0;
    fileList->files = (char**)malloc(sizeof(char*));
    fileList->files[0] = NULL;

    loadFilePaths(fileList, dir);
    printf("\n\nFINAL PRINT:\n");
    printFileList(*fileList);   

    freeFileListFiles(fileList);
    free(fileList);
    exit(0);
} 

void loadFilePaths(FileList* fileList, const char* basePath)
{
    char* path = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
    {
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strncmp(dp->d_name, ".", 1) != 0 && dp->d_type == DT_DIR)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            addFileToList(fileList, path);

            loadFilePaths(fileList, path);
        }
    }

    closedir(dir);
    free(path);
}

void printFileList(const FileList list)
{
    printf("====================================\n");
    for(int i = 0; i < list.size; i++)
    {
        printf("files[%d]: %s\n", i, list.files[i]);
    }  
    printf("====================================\n");
}

void freeFileListFiles(FileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i]);
    }
}

void addFileToList(FileList* list, const char* path)
{
    int beforeAdditionSize = list->size;
    int afterAdditionSize = beforeAdditionSize + 1;

    list->files = (char**)realloc(list->files, afterAdditionSize * sizeof(char*));
    list->files[beforeAdditionSize] = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    list->size++;

    strcpy(list->files[beforeAdditionSize], path);
}