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
    makeDir("temp");
    runTestGatherer();
    compileIntoTempObjectFiles();
    linkObjectFilesWithGregTestDllToMakeProjectTestDll();
    createTestMainExecutableFromProjectDllAndGregTestDll();
    int testResults = runTests();
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
    removeFolder("temp");
    return retval;
}

void makeDir(char* dirName)
{
    char* const argv[] = {"/usr/bin/mkdir.exe", dirName, NULL};
    forkAndRunChildProcess("/usr/bin/mkdir.exe", argv);
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

void compileIntoTempObjectFiles()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-c", "src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    "src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "src/HelloWorld/HelloWorld.c", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);

    char* const argv2[] = {"/usr/bin/mv.exe", "HelloWorld.o", "TestHelloWorld.o", "TestHelloWorld2.o", "temp", NULL};
    forkAndRunChildProcess("/usr/bin/mv.exe", argv2);
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-shared", "-o", "temp/TestHelloWorld.dll",
    "temp/TestHelloWorld.o", "temp/TestHelloWorld2.o", "temp/HelloWorld.o", "-L./", "lib/GregTest.dll", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void createTestMainExecutableFromProjectDllAndGregTestDll()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-o", "temp/TestMain",
    "temp/TestMain.c", "-L./", "temp/TestHelloWorld.dll", "lib/GregTest.dll", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv); 
}

int runTests()
{ 
    char * const argv[] = {"/usr/bin/cp.exe", "temp/TestHelloWorld.dll", ".", NULL};
    forkAndRunChildProcess("/usr/bin/cp.exe", argv);

    char * const argv1[] = {"/usr/bin/cp.exe", "lib/GregTest.dll", ".", NULL};
    forkAndRunChildProcess("/usr/bin/cp.exe", argv1);

    char * const argv2[] = {"temp/TestMain.exe", NULL};
    int testResult = forkAndRunChildProcess("temp/TestMain.exe", argv2); 

    char * const argv3[] = {"/usr/bin/rm.exe", "GregTest.dll", NULL};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv3); 

    char * const argv4[] = {"/usr/bin/rm.exe", "TestHelloWorld.dll", NULL};
    forkAndRunChildProcess("/usr/bin/rm.exe", argv4); 

    return testResult;
}

int compileObjectFilesIntoProjectExecutable()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "temp/HelloWorld.o", "-o", "dist/HelloWorld.exe", NULL};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);   
}

void removeFolder(char* folderName)
{
    char * const argv[] = {"/usr/bin/rm.exe", folderName, "-r", NULL};
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