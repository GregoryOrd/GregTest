#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

#include "TestGatherer/TestStructureDefs.h"
#include "TestGatherer/SourceFileStructureDefs.h"

void makeDir(char* dirName);
void runTestGatherer(TestFileList* testFiles, SourceFileList* sourceFiles);
int runTestsAndCompileIfTheyPass();
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles);
void createTestMainExecutable(TestFileList* testFiles, SourceFileList* sourceFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles);
void printTestCases(const TestFile* list);
void printSourceFiles(const SourceFileList* list);
void printTestFiles(const TestFileList* list);
void compileIntoTempObjectFiles(TestFileList* testCases, SourceFileList* sourceFiles);
void populateArgsFor_compileIntoTempObjectFiles(char** argv, TestFileList* testCases, SourceFileList* sourceFiles, int numGccArgs);
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