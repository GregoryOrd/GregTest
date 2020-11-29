#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define WINDOWS_MAX_PATH_LENGTH 260

int listFilesRecursively(const char *basePath);
void getFilePaths(char** filePaths, int* count, const char *basePath);
void printStringArray(char** str);

  
int main(void) 
{ 
    char dir[WINDOWS_MAX_PATH_LENGTH] = ".";
    int numFolders = listFilesRecursively(dir);

    int* count = (int*)malloc(sizeof(int));
    *count = 0;

    char** filePaths = (char**)malloc(numFolders * WINDOWS_MAX_PATH_LENGTH * sizeof(char*) - 1);
    for(int i = 0; i < numFolders; i++)
    {
        filePaths[i] = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*) - 1);
        filePaths[i] = "Hello";
    }

    getFilePaths(filePaths, count, dir);

    for(int i = 0; i < numFolders; i++)
    {
        printf("filePaths[%d]: %s\n", i, filePaths[i]);
    }

    free(count);
    free(filePaths);
    exit(0);
} 

//Run this first to learn how much memory to allocate for the list
int listFilesRecursively(const char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    int count = 0;

    if (!dir)
        return 0;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == DT_DIR)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            count++;
            count += listFilesRecursively(path);
        }
    }

    closedir(dir);
    return count;
}

//Get the names of all subdirectories
void getFilePaths(char** filePaths, int* count, const char *basePath)
{

    char* path = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*) - 1);
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
    {
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == DT_DIR)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            filePaths[(*count)] = path;

            (*count)++;
            getFilePaths(filePaths, count, path);
        }
    }

    closedir(dir);
    free(path);
}