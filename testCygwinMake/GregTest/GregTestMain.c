#include "GregTestMain.h"

#include "TestGatherer/TestGatherer.h"
#include "TestGatherer/TestMainWriter.h"
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
    runTestGatherer();
    compileIntoObjectFiles();
    linkObjectFilesWithGregTestDllToMakeProjectTestDll();
    createTestMainExecutableFromProjectDllAndGregTestDll();
    int testResults = runTests();
    removeProjectTestDll();
    removeTestMainArtifacts();
    if(!testResults)
    {
        //Tests Passed
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
    removeObjectFiles();
    return retval;
}

void runTestGatherer()
{
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = "src";

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    initTestCases(testCases);

    loadTests(testCases, startingDirectory);
    writeTestsToTestMain(testCases);

    freeTestCasesList(testCases);
}

void compileIntoObjectFiles()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-c", "src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    "src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "src/HelloWorld/HelloWorld.c", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-shared", "-o", "dist/TestHelloWorld.dll",
    "TestHelloWorld.o", "TestHelloWorld2.o", "HelloWorld.o", "-L./", "dist/GregTest.dll", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void createTestMainExecutableFromProjectDllAndGregTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-o", "dist/TestMain",
    "TestMain.c", "-L./", "dist/TestHelloWorld.dll", "dist/GregTest.dll", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv); 
}

int runTests()
{
    char * const argv[] = {"dist/TestMain.exe", NULL};
    return forkAndRunChildProcess("dist/TestMain.exe", argv); 
}

void removeProjectTestDll()
{
    char * const argv[] = {"/usr/bin/rm.exe", "dist/TestHelloWorld.dll", NULL};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv);     
}

void removeTestMainArtifacts()
{
    char * const argv[] = {"/usr/bin/rm.exe", "dist/TestMain.exe", "TestMain.h", "TestMain.c", NULL};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv);    
}

int compileObjectFilesIntoProjectExecutable()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "HelloWorld.o", "-o", "dist/HelloWorld.exe", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);   
}

void removeObjectFiles()
{
    char * const argv[] = {"/usr/bin/rm.exe", "HelloWorld.o", "TestHelloWorld.o", "TestHelloWorld2.o", NULL};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv);  
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