#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

void runTestGatherer();
void compileIntoObjectFiles();
void linkObjectFilesWithGregTestDllToMakeProjectDll();
void createTestMainExecutableFromProjectDllAndGregTestDll();
void forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif