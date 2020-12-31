#include "GregTestMain.h"

#include "TestGatherer/TestGatherer.h"
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
    char* const argv[] = {mkdir, dirName, NULL};
    forkAndRunChildProcess(mkdir, argv);
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
    char * const argv[] = {gcc, "-c", "src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    "src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "src/HelloWorld/HelloWorld.c", NULL};
    forkAndRunChildProcess(gcc, argv);

    char* const argv2[] = {mv, "HelloWorld.o", "TestHelloWorld.o", "TestHelloWorld2.o", TEMP_DIR, NULL};
    forkAndRunChildProcess(mv, argv2);
}

void linkObjectFilesWithGregTestDllToMakeProjectTestDll()
{
    char * const argv[] = {gcc, "-shared", "-o", "temp/TestProject.dll",
    "temp/TestHelloWorld.o", "temp/TestHelloWorld2.o", "temp/HelloWorld.o", "-L./", "lib/GregTest.dll", NULL};
    forkAndRunChildProcess(gcc, argv);
}

void createTestMainExecutableFromProjectDllAndGregTestDll()
{
    char * const argv[] = {gcc, "-o", "temp/TestMain",
    "temp/TestMain.c", "-L./", "temp/TestProject.dll", "lib/GregTest.dll", NULL};
    forkAndRunChildProcess(gcc, argv); 
}

int runTests()
{ 
    char * const argv[] = {cp, "temp/TestProject.dll", ".", NULL};
    forkAndRunChildProcess(cp, argv);

    char * const argv1[] = {cp, "lib/GregTest.dll", ".", NULL};
    forkAndRunChildProcess(cp, argv1);

    char * const argv2[] = {"temp/TestMain.exe", NULL};
    int testResult = forkAndRunChildProcess("temp/TestMain.exe", argv2); 

    char * const argv3[] = {rm, "GregTest.dll", NULL};
    forkAndRunChildProcess(rm, argv3); 

    char * const argv4[] = {rm, "TestProject.dll", NULL};
    forkAndRunChildProcess(rm, argv4); 

    return testResult;
}

int compileObjectFilesIntoProjectExecutable()
{
    char * const argv[] = {gcc, "temp/HelloWorld.o", "-o", "dist/HelloWorld.exe", NULL};
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