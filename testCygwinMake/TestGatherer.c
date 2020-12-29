#include "TestGatherer.h"

#include <ctype.h>
#include "TestDefinitions.h"
#include "TestMainWriter.h"

int main(void) 
{     
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = ".";

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    initTestCases(testCases);

    loadTests(testCases, startingDirectory);
    writeTestsToTestMain(testCases);

    freeTestCasesList(testCases);
    exit(0);
} 

void initTestCases(TestCaseList* testCases)
{
    testCases->size = 0;
    testCases->cases = (TestCase*)malloc(sizeof(TestCase));
    testCases->cases[0].testFile = NULL;
    testCases->cases[0].testName = NULL; 
}

void loadTests(TestCaseList* testCases, char* basePath)
{
    char* fileOrSubDirectoryFullPath = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    struct dirent *fileOrSubDirectory;

    DIR *basePathDirectory = opendir(basePath);
    if (!basePathDirectory)
    {
        return;
    }

    while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL)
    {
        copyFileOrSubDirectoryNameIntoPath(fileOrSubDirectoryFullPath, basePath, fileOrSubDirectory->d_name);
        addTestCasesOrEnterSubDirectoryForRecursion(testCases, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
    }

    closedir(basePathDirectory);
    free(fileOrSubDirectoryFullPath);
}

void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath)
{
    if(isTestDir(basePath) && isTestFile(fileOrSubDirectory))
    {
        addTestCasesToList(testCases, fileOrSubDirectoryFullPath);
    }
    if (isDirectory(fileOrSubDirectory))
    {
        loadTests(testCases, fileOrSubDirectoryFullPath);
    }
}

bool isDirectory(struct dirent *fileOrSubDirectory)
{
    return strncmp(fileOrSubDirectory->d_name, ".", 1) != 0 && fileOrSubDirectory->d_type == DT_DIR;
}

void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName)
{
    strcpy(path, basePath);
    strcat(path, "/");
    strcat(path, fileOrSubDirectoryName);  
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

void addTestCasesToList(TestCaseList* list, const char* pathToTestFile)
{
    FILE *testFile;
    char* buffer = (char*)malloc(255*sizeof(char));
    testFile = fopen(pathToTestFile, "r");

    while(fgets(buffer, 255, (FILE*)testFile) != NULL)
    {
        if(isTestCaseDefinition(buffer))
        {
            addSingleTestCaseToList(list, pathToTestFile, buffer);
        }  
    }

    free(buffer);
    fclose(testFile);
}

void addSingleTestCaseToList(TestCaseList* list, const char* pathToTestFile, char* buffer)
{
    list->cases = (TestCase*)realloc(list->cases, ((list->size + 1) * sizeof(TestCase)));
    list->cases[list->size].testName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    list->cases[list->size].testFile = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 

    trimTestName(buffer); 

    strcpy(list->cases[list->size].testName, buffer);
    strcpy(list->cases[list->size].testFile, pathToTestFile);
    list->size++;
}