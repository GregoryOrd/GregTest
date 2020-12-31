#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

void makeDir(char* dirName);
void runTestGatherer();
void compileIntoTempObjectFiles();
void linkObjectFilesWithGregTestDllToMakeProjectTestDll();
void createTestMainExecutableFromProjectDllAndGregTestDll();
int runTests();
int compileObjectFilesIntoProjectExecutable();
void changeDirectory(char* dirToMoveTo);
void copyTestProjectDllIntoTopLevelDir();
void copyGregTestDllIntoTopLevelDir();
void removeFolder(char* folderName);
int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif