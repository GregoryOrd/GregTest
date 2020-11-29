#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "TestGatherer.h"


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
        printf("files[%d]: %s - ", i, list.files[i]);
        if(isTestDir(list.files[i]))
        {
            printf("(T)\n");   
        }
        else
        {
            printf("\n");
        }   
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

bool isTestDir(char* dirName)
{
    bool result = false;
    char* lower = lowerString(dirName);
    if(strstr(lower, "/test") != NULL)
    {
        result = true;
    }
    free(lower);
    return result;
}

bool isTestFile(char* dirName)
{
    bool result = false;
    char* lower = lowerString(dirName);
    if(strncmp(lower, "test", 4) == 0 && (strstr(lower, ".c") != NULL || strstr(lower, ".cpp") != NULL))
    {
        result = true;
    }
    free(lower);
    return result;
}

char* lowerString(char* str)
{
    int size = 0;

    char* lower = (char*)malloc(sizeof(char));
    lower[0] = '\0';

    char* itr = str;
    while(*itr != '\0')
    {
        char l = tolower(*itr);
        lower = (char*)realloc(lower, size+2);
        lower[size] = l;
        lower[size+1] = '\0';
        size++;
        itr++;
    }
    return lower;
}