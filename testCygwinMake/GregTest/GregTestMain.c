#include "GregTestMain.h"

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
    linkObjectFilesWithGregTestDllToMakeProjectDll();
    createTestMainExecutableFromProjectDllAndGregTestDll();
    return 0;
}

void runTestGatherer()
{
    char * const argv[] = {"C:/GregTest/testCygwinMake/dist/TestGathererAndWriter.exe",NULL};
    forkAndRunChildProcess("C:/GregTest/testCygwinMake/dist/TestGathererAndWriter.exe", argv);
}

void compileIntoObjectFiles()
{
    char * const argv[] = {"/usr/bin/gcc.exe", "-c", "C:/GregTest/testCygwinMake/src/HelloWorld/testHelloWorld/TestHelloWorld.c",
    "C:/GregTest/testCygwinMake/src/HelloWorld/testHelloWorld/TestHelloWorld2.c", "C:/GregTest/testCygwinMake/src/HelloWorld/HelloWorld.c"};
    forkAndRunChildProcess("/usr/bin/gcc.exe", argv);
}

void linkObjectFilesWithGregTestDllToMakeProjectDll()
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

void forkAndRunChildProcess(const char * pathToExecutable, char * const argv[])
{
    #ifdef __unix__
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        execv(pathToExecutable, argv);
        exit(1); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */
    } 
    #endif

    #ifdef __WIN32__
    FILE *fp;
    int status;

    status = spawnv(P_WAIT, pathToExecutable, argv);

    #endif
}