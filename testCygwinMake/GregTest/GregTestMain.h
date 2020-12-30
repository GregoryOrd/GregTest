#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

void runTestGatherer();
void compileIntoObjectFiles();
void linkObjectFilesWithGregTestDllToMakeProjectTestDll();
void createTestMainExecutableFromProjectDllAndGregTestDll();
int runTests();
void removeProjectTestDll();
void removeTestMainArtifacts();
int compileObjectFilesIntoProjectExecutable();
void removeObjectFiles();
int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif