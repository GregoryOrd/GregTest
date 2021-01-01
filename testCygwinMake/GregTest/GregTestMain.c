#include "GregTestMain.h"

#include "TestGatherer/TestGatherer.h"
#include "GregTestConstants.h"
#include "TestGatherer/TestMainWriter.h"
#include "CommandLineExecutables.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* for pid_t */
#include <process.h>

#ifdef __unix__
#include <unistd.h> /* for fork */
#include <sys/wait.h> /* for wait */
#endif

#ifdef __WIN32__
#include <process.h>
#endif

int main()
{
    int retval = 1;

    TestFileList* testFiles = (TestFileList*)malloc(sizeof(TestFileList));
    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    ObjectFileList* tempObjectFiles = (ObjectFileList*)malloc(sizeof(ObjectFileList));
    initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

    runTestGatherer(testFiles, sourceFiles);
    createTestMainExecutable(testFiles, sourceFiles, tempObjectFiles);
    retval = runTestsAndCompileIfTheyPass(tempObjectFiles);

    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return retval;
}

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    initTestFiles(testFiles);
    initSourceFiles(sourceFiles);
    initObjectFileList(tempObjectFiles);
    makeDir(TEMP_DIR);
}

void createTestMainExecutable(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    compileIntoTempObjectFiles(tempObjectFiles, testFiles, sourceFiles);
    linkObjectFilesWithGregTestDllToMakeProjectTestDll(tempObjectFiles);
    createTestMainExecutableFromProjectDllAndGregTestDll();
}

void initObjectFileList(ObjectFileList* objectFiles)
{
    objectFiles->size = 0;
    objectFiles->files = (ObjectFile*)malloc(sizeof(ObjectFile));
    objectFiles->files[0].name = NULL;
}

void freeObjectFileList(ObjectFileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i].name);
    }
    free(list);
}

void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    removeFolder(TEMP_DIR);
    freeTestFileList(testFiles);
    freeSourceFileList(sourceFiles);
    freeObjectFileList(tempObjectFiles);
}

int runTestsAndCompileIfTheyPass(ObjectFileList* tempObjectFiles)
{
    int retval = 1;
    int testResults = runTests();
    if(!testResults)
    {
        if(!compileObjectFilesIntoProjectExecutable(tempObjectFiles))
        {
            printf("\nBuild Successful!\n");
        }
        else
        {
            printf("Error Compiling the Code After Tests Completed\n");
        }
        
        retval = 0;
    }
    else if(testResults == 139)
    {
        printf("\nSegmentation Fault While Running the Tests\n");
        printf("Build Failed");
    }
    return retval;
}

void makeDir(char* dirName)
{
    char* const argv[] = {mkdir, dirName, NULL};
    forkAndRunChildProcess(mkdir, argv);
}

void runTestGatherer(TestFileList* testFiles, SourceFileList* sourceFiles)
{
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

    loadTestsAndSourceFiles(testFiles, sourceFiles, startingDirectory);
    writeTestsToTestMain(testFiles);
}

void compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, TestFileList* testFiles, SourceFileList* sourceFiles)
{
    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = testFiles->size + sourceFiles->size + 3;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    ArgList* mvArgs = (ArgList*)malloc(sizeof(ArgList));
    mvArgs->size = testFiles->size + sourceFiles->size + 3;
    mvArgs->args = (char**)malloc(mvArgs->size * sizeof(char*));

    populateArgsFor_compileIntoTempObjectFiles(tempObjectFiles, gccArgs, mvArgs, testFiles, sourceFiles);
    forkAndRunChildProcess(gcc, gccArgs->args);
    forkAndRunChildProcess(mv, mvArgs->args);

    freeArgList(gccArgs);
    free(mvArgs);
}

void printArgList(ArgList* argList, char* description)
{
    printf("%s\n");
    printf("===================================================\n");
    for(int i = 0; i < argList->size; i++)
    {
        printf("args[%d]: %s\n", i, argList->args[i]);
    }
    printf("===================================================\n\n");
}

void freeArgList(ArgList* argList)
{
    free(argList->args);
    free(argList);
}

void populateArgsFor_compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, TestFileList* testFiles, SourceFileList* sourceFiles)
{
    gccArgs->args[0] = gcc;
    gccArgs->args[1] = "-c";
    gccArgs->args[gccArgs->size-1] = NULL;

    mvArgs->args[0] = mv;
    mvArgs->args[mvArgs->size-2] = TEMP_DIR;
    mvArgs->args[mvArgs->size-1] = NULL;

    int argIndex = 0;
    getArgsForTestFiles(tempObjectFiles, &argIndex, testFiles, gccArgs, mvArgs);
    getArgsForSourceFiles(tempObjectFiles, &argIndex, sourceFiles, gccArgs, mvArgs);
}

void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs)
{
    char* objectFileName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
    int gccFileArgOffset = 2;
    int mvFileArgOffset = 1;
    int testFileIndex = 0;
    while(testFileIndex < testFiles->size)
    {
        clearString(objectFileName);
        determineObjectFileName(objectFileName, testFiles->files[testFileIndex].name);
        addTempObjectFileToList(tempObjectFiles, objectFileName, false);
        gccArgs->args[*argIndex + gccFileArgOffset] = testFiles->files[testFileIndex].name;
        mvArgs->args[*argIndex + mvFileArgOffset] = (char*)malloc(strlen(objectFileName) * sizeof(char));
        strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
        (*argIndex)++;
        testFileIndex++;
    } 
    free(objectFileName);
}

void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs)
{
    char* objectFileName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
    int gccFileArgOffset = 2;
    int mvFileArgOffset = 1;
    int sourceFileIndex = 0;
    while(sourceFileIndex < sourceFiles->size)
    {
        clearString(objectFileName);
        determineObjectFileName(objectFileName, sourceFiles->files[sourceFileIndex].name);
        addTempObjectFileToList(tempObjectFiles, objectFileName, true);
        gccArgs->args[*argIndex + gccFileArgOffset] = sourceFiles->files[sourceFileIndex].name;
        mvArgs->args[*argIndex + mvFileArgOffset] = (char*)malloc(strlen(objectFileName) * sizeof(char));
        strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
        (*argIndex)++;
        sourceFileIndex++;
    }
    free(objectFileName);
}

void addTempObjectFileToList(ObjectFileList* list, char* filename, bool isFromSource)
{
    int tempObjectFileLength = strlen(TEMP_DIR) + strlen("\\") + strlen(filename);
    char* tempObjectFile = (char*)malloc(tempObjectFileLength * sizeof(char));
    clearString(tempObjectFile);
    strcat(tempObjectFile, TEMP_DIR);
    strcat(tempObjectFile, "\\");
    strcat(tempObjectFile, filename);

    list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
    list->files[list->size].name = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 
    strcpy(list->files[list->size].name, tempObjectFile);
    list->files[list->size].isFromSource = isFromSource;
    list->size++; 

    free(tempObjectFile);
}

void clearString(char* str)
{
    int length = strlen(str);
    for(int i = 0; i < length - 1; i++)
    {
        str[i] = '\0';
    }
}

void determineObjectFileName(char* objectFileName, const char* filePath)
{
    int length = strlen(filePath) - 1;
    int offset = 2;
    char* reversedObjectFileName = (char*)malloc(length * sizeof(char));
    reversedObjectFileName[0] = 'o';
    reversedObjectFileName[1] = '.';
    bool pastExtension = false;

    for(int i = length - 1; i > 0; i--)
    {
        if(filePath[i] == '\\' || filePath[i] == '/')
        {
            break;
        }
        else if(filePath[i] != '.')
        {
            reversedObjectFileName[offset] = filePath[i];
            reversedObjectFileName[offset + 1] = '\0';
            offset++;
        }
        else if(filePath[i] == '.')
        {
            pastExtension = true;
        }
    }
    reverseFileName(objectFileName, reversedObjectFileName);
    free(reversedObjectFileName);
}

void reverseFileName(char* dest, char* src)
{
    int srcLength = strlen(src);
    int countUp = 0;
    for(int i = srcLength - 1; i >= 0; i--)
    {
        if(src[i] != '\0')
        {
            dest[countUp] = src[i];
            countUp++;
        }
    }
    dest[countUp] = '\0';
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll(ObjectFileList* tempObjectFiles)
{
    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = tempObjectFiles->size + 7;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    gccArgs->args[0] = gcc;
    gccArgs->args[1] = "-shared";
    gccArgs->args[2] = "-o";
    gccArgs->args[3] = TEMP_TEST_PROJECT_DLL;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        gccArgs->args[i + 4] = (&tempObjectFiles->files[i])->name;
    }
    gccArgs->args[gccArgs->size-3] = "-L./";
    gccArgs->args[gccArgs->size-2] = LIB_GREG_TEST_DLL;
    gccArgs->args[gccArgs->size-1] = NULL;

    forkAndRunChildProcess(gcc, gccArgs->args);

    freeArgList(gccArgs);
}

void createTestMainExecutableFromProjectDllAndGregTestDll()
{
    char * const argv[] = {gcc, "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_DLL, LIB_GREG_TEST_DLL, NULL};
    forkAndRunChildProcess(gcc, argv); 
}

int runTests()
{ 
    char * const argv[] = {cp, TEMP_TEST_PROJECT_DLL, CURRENT_DIR, NULL};
    forkAndRunChildProcess(cp, argv);

    char * const argv1[] = {cp, LIB_GREG_TEST_DLL, CURRENT_DIR, NULL};
    forkAndRunChildProcess(cp, argv1);

    char * const argv2[] = {TEMP_TEST_MAIN_EXE, NULL};
    int testResult = forkAndRunChildProcess(TEMP_TEST_MAIN_EXE, argv2); 

    char * const argv3[] = {rm, GREG_TEST_DLL, NULL};
    forkAndRunChildProcess(rm, argv3); 

    char * const argv4[] = {rm, TEST_PROJECT_DLL, NULL};
    forkAndRunChildProcess(rm, argv4); 

    return testResult;
}

int compileObjectFilesIntoProjectExecutable(ObjectFileList* tempObjectFiles)
{
    ArgList* gccArgs = (ArgList*)malloc(sizeof(ArgList));
    gccArgs->size = numObjectFilesFromSource(tempObjectFiles) + 4;
    gccArgs->args = (char**)malloc(gccArgs->size * sizeof(char*));

    gccArgs->args[0] = gcc;
    int numObjectFilesFromSourceAddedToArgsList = 0;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        ObjectFile* file = &tempObjectFiles->files[i];
        if(file->isFromSource)
        {
            gccArgs->args[numObjectFilesFromSourceAddedToArgsList + 1] = file->name;
            numObjectFilesFromSourceAddedToArgsList++;
        }
    }
    gccArgs->args[gccArgs->size-3] = "-o";
    gccArgs->args[gccArgs->size-2] = PROJECT_EXE;
    gccArgs->args[gccArgs->size-1] = NULL;

    int retval = forkAndRunChildProcess(gcc, gccArgs->args);   
    freeArgList(gccArgs);
    return retval;
}

int numObjectFilesFromSource(ObjectFileList* tempObjectFiles)
{
    int count = 0;
    for(int i = 0; i < tempObjectFiles->size; i++)
    {
        if((&tempObjectFiles->files[i])->isFromSource)
        {
            count++;
        }
    }
    return count;
}

void removeFolder(char* folderName)
{
    char * const argv[] = {rm, folderName, "-r", NULL};
    forkAndRunChildProcess(rm, argv);  
}

int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[])
{
    int status;
    #ifdef __unix__
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        execv(pathToExecutable, argv);
        exit(1); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        int status;
        waitpid(pid, &status, 0); /* wait for child to exit */
        if (WIFEXITED(status)) 
        { 
            return WEXITSTATUS(status);    
        } 
    } 
    #endif

    #ifdef __WIN32__
    status = spawnv(P_WAIT, pathToExecutable, argv);
    if(WIFEXITED(status)) 
    { 
        return WEXITSTATUS(status);   
    } 
    #endif
}