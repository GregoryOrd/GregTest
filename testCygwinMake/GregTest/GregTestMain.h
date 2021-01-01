#ifndef GREG_TEST_MAIN
#define GREG_TEST_MAIN 

#include "TestGatherer/TestStructureDefs.h"
#include "TestGatherer/SourceFileStructureDefs.h"
#include "ObjectFileStructureDefs.h"
#include "ArgListDefs.h"

void makeDir(char* dirName);
void runTestGatherer(TestFileList* testFiles, SourceFileList* sourceFiles);
int runTestsAndCompileIfTheyPass();
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void createTestMainExecutable(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, TestFileList* testCases, SourceFileList* sourceFiles);
void populateArgsFor_compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, TestFileList* testCases, SourceFileList* sourceFiles);
void linkObjectFilesWithGregTestDllToMakeProjectTestDll(ObjectFileList* tempObjectFiles);
void createTestMainExecutableFromProjectDllAndGregTestDll();
void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs);
void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs);
int runTests();
void determineObjectFileName(char* objectFileName, const char* filePath);
int compileObjectFilesIntoProjectExecutable();
void addTempObjectFileToList(ObjectFileList* list, char* filename, bool isFromSource);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void changeDirectory(char* dirToMoveTo);
void copyTestProjectDllIntoTopLevelDir();
void copyGregTestDllIntoTopLevelDir();
void reverseFileName(char* dest, char* src);
void removeFolder(char* folderName);
void clearString(char* str);
void freeArgList(ArgList* argList);
int forkAndRunChildProcess(const char * pathToExecutable, char * const argv[]);

#endif