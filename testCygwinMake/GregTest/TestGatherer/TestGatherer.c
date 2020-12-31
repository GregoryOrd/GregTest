#include "TestGatherer.h"

#include <ctype.h>
#include "TestMainWriter.h"
#include "TestAndSrcDefinitions.h"

void initTestFiles(TestFileList* testFiles)
{
    testFiles->size = 0;
    testFiles->files = (TestFile*)malloc(sizeof(TestFile));
    testFiles->files[0].name = NULL;
    testFiles->files[0].numTestCases = 0;
    testFiles->files[0].cases = (TestCase*)malloc(sizeof(TestCase));
}

void initSourceFiles(SourceFileList* sourceFiles)
{
    sourceFiles->size = 0;
    sourceFiles->files = (SourceFile*)malloc(sizeof(SourceFile));
    sourceFiles->files[0].name = NULL;
}

void loadTestsAndSourceFiles(TestFileList* testFiles, SourceFileList* sourceFiles, char* basePath)
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
        addToListOrEnterSubDirectoryForRecursion(testFiles, sourceFiles, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
    }

    closedir(basePathDirectory);
    free(fileOrSubDirectoryFullPath);
}

void addToListOrEnterSubDirectoryForRecursion(TestFileList* testFiles, SourceFileList* sourceFiles, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath)
{
    if(isTestDir(basePath) && isTestFile(fileOrSubDirectory))
    {
        addTestFileToList(testFiles, fileOrSubDirectoryFullPath);
    }
    else if(!isVisibleDirectory(fileOrSubDirectory) && isSourceFile(fileOrSubDirectory))
    {
        addSourceFileToList(sourceFiles, fileOrSubDirectoryFullPath);
    }
    else if(isVisibleDirectory(fileOrSubDirectory))
    {
        loadTestsAndSourceFiles(testFiles, sourceFiles, fileOrSubDirectoryFullPath);
    }
}

bool isVisibleDirectory(struct dirent *fileOrSubDirectory)
{
    bool dirNameStartsWithDot = strncmp(fileOrSubDirectory->d_name, ".", 1) == 0;
    bool dirNameContainsSlashDot = strstr(fileOrSubDirectory->d_name, "/.") != NULL;
    bool isHiddenDirectory = dirNameStartsWithDot || dirNameContainsSlashDot;
    return !isHiddenDirectory && fileOrSubDirectory->d_type == DT_DIR;
}

void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName)
{
    strcpy(path, basePath);
    strcat(path, "/");
    strcat(path, fileOrSubDirectoryName);  
}

void freeTestFileList(TestFileList* testFileList)
{
    for(int fileIndex = 0; fileIndex < testFileList->size; fileIndex++)
    {
        TestFile* testFile = &testFileList->files[fileIndex];
        free(testFile->name);
        for(int testCaseIndex = 0; testCaseIndex < testFile->numTestCases; testCaseIndex++)
        {
            TestCase* testCase = &testFile->cases[testCaseIndex];
            free(testCase->testName);
        }
        free(testFile->cases);
    }
    free(testFileList->files);
    free(testFileList);
}

void freeSourceFileList(SourceFileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i].name);
    }
    free(list);
}


void addSourceFileToList(SourceFileList* list, const char* pathToSourceFile)
{
    list->files = (SourceFile*)realloc(list->files, ((list->size + 1) * sizeof(SourceFile)));
    list->files[list->size].name = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));; 
    strcpy(list->files[list->size].name, pathToSourceFile);
    list->size++; 
}

void addTestFileToList(TestFileList* testFileList, const char* pathToTestFile)
{
    testFileList->files = (TestFile*)realloc(testFileList->files, (testFileList->size + 1) * sizeof(TestFile));
    testFileList->files[testFileList->size].name = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    testFileList->files[testFileList->size].numTestCases = 0;
    testFileList->files[testFileList->size].cases = (TestCase*)malloc(sizeof(TestCase));
    strcpy(testFileList->files[testFileList->size].name, pathToTestFile);

    addTestCasesToList(testFileList, pathToTestFile);

    testFileList->size++;
}

void addTestCasesToList(TestFileList* testFileList, const char* pathToTestFile)
{
    FILE *testFilePtr;
    char* buffer = (char*)malloc(255*sizeof(char));
    testFilePtr = fopen(pathToTestFile, "r");

    while(fgets(buffer, 255, (FILE*)testFilePtr) != NULL)
    {
        if(isTestCaseDefinition(buffer))
        {
            addSingleTestCaseToList(testFileList, pathToTestFile, buffer);
        }  
    }

    free(buffer);
    fclose(testFilePtr);
}

void addSingleTestCaseToList(TestFileList* testFileList, const char* pathToTestFile, char* buffer)
{
    TestFile* testFile = &testFileList->files[testFileList->size];
    testFile->cases = (TestCase*)realloc(testFile->cases, ((testFile->numTestCases + 1) * sizeof(TestCase)));
    testFile->cases[testFile->numTestCases].testName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));

    trimTestName(buffer); 

    strcpy(testFile->cases[testFile->numTestCases].testName, buffer);
    testFile->numTestCases++;
    testFileList->totalNumTestCases++;
}