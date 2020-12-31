#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#include <dirent.h>
#include <stdbool.h>
#include "TestStructureDefs.h"
#include "SourceFileStructureDefs.h"

void initTestCases(TestCaseList* testCases);
void initSourceFiles(SourceFileList* sourceFiles);
void loadTests(TestCaseList* testCases, SourceFileList* sourceFiles, char* basePath);
void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, SourceFileList* sourceFiles, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath);
void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName);
void addTestCasesToList(TestCaseList* list, const char* pathToTestFile);
void addSingleTestCaseToList(TestCaseList* list, const char* pathToTestFile, char* buffer);
void freeTestCasesList(TestCaseList* list);
void freeSourceFileList(SourceFileList* list);
bool isDirectory(struct dirent *fileOrSubDirectory);
void trimTestName(char* testName);

#endif