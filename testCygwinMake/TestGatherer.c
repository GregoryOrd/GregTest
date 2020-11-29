#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "TestGatherer.h"


int main(void) 
{ 
    char dir[WINDOWS_MAX_PATH_LENGTH] = ".";

    FileList* tests = (FileList*)malloc(sizeof(FileList*));
    tests->size = 0;
    tests->files = (char**)malloc(sizeof(char*));
    tests->files[0] = NULL;

    TestCaseList* testCases;

    loadTests(tests, testCases, dir);
    printf("\n\nFINAL PRINT:\n");
    printFileList(*tests);   

    freeFileListFiles(tests);
    free(tests);
    exit(0);
} 

void loadTests(FileList* fileList, TestCaseList* testCases, char* basePath)
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
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);
  
        if(isTestDir(basePath) && isTestFile(dp->d_name))
        {
            addFileToList(fileList, path);
            addTestCasesToList(testCases, path);
        }
        if (strncmp(dp->d_name, ".", 1) != 0 && dp->d_type == DT_DIR)
        {
            loadTests(fileList, testCases, path);
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

void addTestCasesToList(TestCaseList* list, const char* path)
{
    printf("Looking through %s to find test cases\n", path);
    FILE *fp;
    char buff[255];

    fp = fopen(path, "r");

    while(fgets(buff, 255, (FILE*)fp) != NULL)
    {
        if(isTestCaseDefinition(buff))
        {
            printf("%s\n", buff );  
        }  
    }

    fclose(fp);
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

bool isTestCaseDefinition(char* line)
{
    bool correctStartOfLine = strncmp(line, "bool test", 9) == 0;
    bool singleSpaceBetweenBoolAndTestName = (strstr(line, " ") == &line[4]);

    int numSpaces = 0;
    int numLeftBrackets = 0;
    int numRightBrackets = 0;
    bool hasSpecialCharacters = false;
    int leftBracketIndex = 0;
    int rightBracketIndex = 0;

    int length = 0;
    char* currentPtr = line;
    while(*currentPtr != '\0')
    {
        if(*currentPtr == ' ')
        {
            numSpaces++;
        }
        else if(*currentPtr == '(')
        {
            numLeftBrackets++;
            leftBracketIndex = length;
        }
        else if(*currentPtr == ')')
        {
            numRightBrackets++;
            rightBracketIndex = length;
        }
        else if(!(*currentPtr >= 'A' && *currentPtr <= 'Z') && !(*currentPtr >= 'a' && *currentPtr <= 'z'))
        {
            hasSpecialCharacters = true;
        }
        length++;
        currentPtr++;
    }


    bool correctSpaces = singleSpaceBetweenBoolAndTestName && numSpaces == 1;
    bool correctBrackets = numLeftBrackets == 1 && numRightBrackets == 1 && leftBracketIndex == length-3 && rightBracketIndex == length - 2;

    if(correctStartOfLine && correctSpaces && correctBrackets)
    {
        return true;
    }
    return false;
}