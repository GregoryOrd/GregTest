#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

void runTestGatherer();
void compileIntoObjectFiles();
void linkObjectFilesWithGregTestDllToMakeProjectDll();
void createTestMainExecutableFromProjectDllAndGregTestDll();
int runTests();
int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif