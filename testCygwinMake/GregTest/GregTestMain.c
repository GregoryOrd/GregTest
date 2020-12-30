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
        return 0;
    }
    return 1;
}

void runTestGatherer()
{
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = "C:/GregTest/testCygwinMake/src";

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    initTestCases(testCases);

    loadTests(testCases, startingDirectory);
    writeTestsToTestMain(testCases);

    freeTestCasesList(testCases);
}

void compileIntoObjectFiles()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-c", "C:/GregTest/testCygwinMake/src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    "C:/GregTest/testCygwinMake/src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "C:/GregTest/testCygwinMake/src/HelloWorld/HelloWorld.c"};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-shared", "-o", "C:/GregTest/testCygwinMake/dist/TestHelloWorld.dll",
    "TestHelloWorld.o", "TestHelloWorld2.o", "HelloWorld.o", "-L./", "C:/GregTest/testCygwinMake/dist/GregTest.dll"};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void createTestMainExecutableFromProjectDllAndGregTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-o", "C:/GregTest/testCygwinMake/dist/TestMain",
    "C:/GregTest/testCygwinMake/TestMain.c", "-L./", "C:/GregTest/testCygwinMake/dist/TestHelloWorld.dll", "C:/GregTest/testCygwinMake/dist/GregTest.dll"};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv); 
}

int runTests()
{
    char * const argv[] = {"C:/GregTest/testCygwinMake/dist/TestMain.exe"};
    return forkAndRunChildProcess("C:/GregTest/testCygwinMake/dist/TestMain.exe", argv); 
}

void removeProjectTestDll()
{
    char * const argv[] = {"/usr/bin/rm.exe", "C:/GregTest/testCygwinMake/dist/TestHelloWorld.dll"};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv);     
}

void removeTestMainArtifacts()
{
    char * const argv[] = {"/usr/bin/rm.exe", "C:/GregTest/testCygwinMake/dist/TestMain.exe", "C:/GregTest/testCygwinMake/TestMain.h", "C:/GregTest/testCygwinMake/TestMain.c"};
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