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
    initTestFiles(testFiles);

    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    initSourceFiles(sourceFiles);

    makeDir(TEMP_DIR);
    runTestGatherer(testFiles, sourceFiles);
    compileIntoTempObjectFiles(testFiles, sourceFiles);
    linkObjectFilesWithGregTestDllToMakeProjectTestDll();
    createTestMainExecutableFromProjectDllAndGregTestDll();
    int testResults = runTests();
    if(!testResults)
    {
        if(!compileObjectFilesIntoProjectExecutable())
        {
            printf("Build Successful!\n");
        }
        else
        {
            printf("Error Compiling the Code After Tests Completed\n");
        }
        
        retval = 0;
    }
    removeFolder(TEMP_DIR);
    freeTestFileList(testFiles);
    freeSourceFileList(sourceFiles);
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

    for(int testFileIndex = 0; testFileIndex < testFiles->size; testFileIndex++)
    {
        printTestCases(&testFiles->files[testFileIndex]);
    }
    printSourceFiles(sourceFiles);
    printTestFiles(testFiles);
}

void printSourceFiles(const SourceFileList* list)
{
    printf("====================================\n");
    for(int i = 0; i < list->size; i++)
    {
        printf("Source File: %s\n", list->files[i].name);
    }  
    printf("====================================\n");  
}

void printTestCases(const TestFile* file)
{
    printf("====================================\n");
    for(int i = 0; i < file->numTestCases; i++)
    {
        printf("file: %s || testCase: %s\n", file->name, file->cases[i].testName);
    }  
    printf("====================================\n");
}

void printTestFiles(const TestFileList* list)
{
    printf("====================================\n");
    for(int i = 0; i < list->size; i++)
    {
        printf("Test File: %s\n", list->files[i].name);
    }  
    printf("====================================\n");    
}

void compileIntoTempObjectFiles(TestFileList* testFiles, SourceFileList* sourceFiles)
{
    // char * const argv[] = {gcc, "-c", "src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    // "src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "src/HelloWorld/HelloWorld.c", NULL};

    int numGccArgs = testFiles->size + sourceFiles->size + 3;
    int numMvArgs = numGccArgs - 1;

    char ** argv = malloc(numGccArgs * sizeof(char*));
    populateArgsFor_compileIntoTempObjectFiles(argv, testFiles, sourceFiles, numGccArgs);
    forkAndRunChildProcess(gcc, argv);

    char* const argv2[] = {mv, "HelloWorld.o", "TestHelloWorld.o", "TestHelloWorld2.o", TEMP_DIR, NULL};
    forkAndRunChildProcess(mv, argv2);

    free(argv);
}

void populateArgsFor_compileIntoTempObjectFiles(char** argv, TestFileList* testFiles, SourceFileList* sourceFiles, int numGccArgs)
{
    argv[0] = gcc;
    argv[1] = "-c";

    int argIndex = 2;
    int testFileIndex = 0;
    int sourceFileIndex = 0;
    while(testFileIndex < testFiles->size)
    {
        argv[argIndex] = testFiles->files[testFileIndex].name;
        argIndex++;
        testFileIndex++;
    }

    while(sourceFileIndex < sourceFiles->size)
    {
        argv[argIndex] = sourceFiles->files[sourceFileIndex].name;
        argIndex++;
        sourceFileIndex++;
    }
    argv[numGccArgs-1] = NULL;
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll()
{
    char * const argv[] = {gcc, "-shared", "-o", TEMP_TEST_PROJECT_DLL, 
    "temp/TestHelloWorld.o", "temp/TestHelloWorld2.o", "temp/HelloWorld.o", "-L./", LIB_GREG_TEST_DLL, NULL};
    forkAndRunChildProcess(gcc, argv);
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

int compileObjectFilesIntoProjectExecutable()
{
    char * const argv[] = {gcc, "temp/HelloWorld.o", "-o", PROJECT_EXE, NULL};
    forkAndRunChildProcess(gcc, argv);   
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
        return = WEXITSTATUS(status);         
    } 
    #endif
}