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

    TestFileList* tests = (TestFileList*)malloc(sizeof(TestFileList));
    tests->size = 0;
    tests->files = (char**)malloc(sizeof(char*));
    tests->files[0] = NULL;

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    testCases->size = 0;
    testCases->cases = (TestCase*)malloc(sizeof(TestCase));
    testCases->cases[0].testFile = NULL;
    testCases->cases[0].testName = NULL;

    loadTests(tests, testCases, dir);
    printf("\n\nFINAL PRINT:\n");
    printTestFileList(*tests);
    printTestCaseList(*testCases);   

    freeTestFilesList(tests);
    freeTestCasesList(testCases);
    exit(0);
} 

void loadTests(TestFileList* TestFileList, TestCaseList* testCases, char* basePath)
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
            addFileToList(TestFileList, path);
            addTestCasesToList(testCases, path);
        }
        if (strncmp(dp->d_name, ".", 1) != 0 && dp->d_type == DT_DIR)
        {
            loadTests(TestFileList, testCases, path);
        }
    }

    closedir(dir);
    free(path);
}

void printTestFileList(const TestFileList list)
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

void printTestCaseList(const TestCaseList list)
{
    printf("====================================\n");
    for(int i = 0; i < list.size; i++)
    {
        printf("file: %s || testCase: %s\n", list.cases[i].testFile, list.cases[i].testName);
    }  
    printf("====================================\n");
}

void freeTestFilesList(TestFileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i]);
    }
    free(list);
}


void freeTestCasesList(TestCaseList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->cases[i].testFile);
        free(list->cases[i].testName);
    }
    free(list);
}

void addFileToList(TestFileList* list, const char* path)
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
    FILE *fp;
    char* buff = (char*)malloc(255*sizeof(char));

    fp = fopen(path, "r");

    while(fgets(buff, 255, (FILE*)fp) != NULL)
    {
        if(isTestCaseDefinition(buff))
        {
            list->cases = (TestCase*)realloc(list->cases, ((list->size + 1) * sizeof(TestCase)));
            list->cases[list->size].testName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
            list->cases[list->size].testFile = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 

            trimTestName(buff); 

            strcpy(list->cases[list->size].testName, buff);
            strcpy(list->cases[list->size].testFile, path);
            list->size++;
        }  
    }

    free(buff);
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

void trimTestName(char* testName)
{
    //testName will come in looking like:
    //  bool testExampleName()
    //This function will trim of the bool and the brackets

    int count = 0;
    char* currentPtr = testName;
    while(*currentPtr != '\0')
    {
        count++;   
        currentPtr++;
    }

    char temp[count];
    strcpy(temp, testName);

    for(int i = 0; i < count; i++)
    {
        if(i < count - 8)
        {
            testName[i] = testName[i+5];
        }
        else{
            testName[i] = '\0';
        }
    }
}