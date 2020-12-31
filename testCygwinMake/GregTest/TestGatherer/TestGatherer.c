#include "TestGatherer.h"

#include <ctype.h>
#include "TestDefinitions.h"
#include "TestMainWriter.h"

void initTestCases(TestCaseList* testCases)
{
    testCases->size = 0;
    testCases->cases = (TestCase*)malloc(sizeof(TestCase));
    testCases->cases[0].testFile = NULL;
    testCases->cases[0].testName = NULL; 
}

void initSourceFiles(SourceFileList* sourceFiles)
{
    sourceFiles->size = 0;
    sourceFiles->files = (SourceFile*)malloc(sizeof(SourceFile));
    sourceFiles->files[0].name = NULL;
}

void loadTests(TestCaseList* testCases, SourceFileList* sourceFiles, char* basePath)
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
        addTestCasesOrEnterSubDirectoryForRecursion(testCases, sourceFiles, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
    }

    closedir(basePathDirectory);
    free(fileOrSubDirectoryFullPath);
}

void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, SourceFileList* sourceFiles, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath)
{
    if(isTestDir(basePath) && isTestFile(fileOrSubDirectory))
    {
        addTestCasesToList(testCases, fileOrSubDirectoryFullPath);
    }
    if (isDirectory(fileOrSubDirectory))
    {
        loadTests(testCases, sourceFiles, fileOrSubDirectoryFullPath);
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

void freeSourceFileList(SourceFileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i].name);
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