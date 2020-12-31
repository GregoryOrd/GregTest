#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

#include "TestGatherer/TestStructureDefs.h"
#include "TestGatherer/SourceFileStructureDefs.h"

void makeDir(char* dirName);
void runTestGatherer(TestCaseList* testCases, SourceFileList* sourceFiles);
void printTestCaseList(const TestCaseList* list);
void printSourceFiles(const SourceFileList* list);
int numTestFiles(TestCaseList* testCases);
void compileIntoTempObjectFiles(TestCaseList* testCases, SourceFileList* sourceFiles);
void populateArgsFor_compileIntoTempObjectFiles(char** argv, TestCaseList* testCases, SourceFileList* sourceFiles, int numGccArgs);
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