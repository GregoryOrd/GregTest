#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

#include "TestGatherer/TestStructureDefs.h"
#include "TestGatherer/SourceFileStructureDefs.h"
#include "ArgListDefs.h"

void makeDir(char* dirName);
void runTestGatherer(TestFileList* testFiles, SourceFileList* sourceFiles);
int runTestsAndCompileIfTheyPass();
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles);
void createTestMainExecutable(TestFileList* testFiles, SourceFileList* sourceFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles);
void compileIntoTempObjectFiles(TestFileList* testCases, SourceFileList* sourceFiles);
void populateArgsFor_compileIntoTempObjectFiles(ArgList* gccArgs, ArgList* mvArgs, TestFileList* testCases, SourceFileList* sourceFiles);
void linkObjectFilesWithGregTestDllToMakeProjectTestDll();
void createTestMainExecutableFromProjectDllAndGregTestDll();
void getArgsForTestFiles(int* argIndex, TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs);
void getArgsForSourceFiles(int* argIndex, SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs);
int runTests();
void determineObjectFileName(char* objectFileName, const char* filePath);
int compileObjectFilesIntoProjectExecutable();
void changeDirectory(char* dirToMoveTo);
void copyTestProjectDllIntoTopLevelDir();
void copyGregTestDllIntoTopLevelDir();
void reverseFileName(char* dest, char* src);
void removeFolder(char* folderName);
void clearString(char* str);
void freeArgList(ArgList* argList);
int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif