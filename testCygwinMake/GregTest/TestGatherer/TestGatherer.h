#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#include <dirent.h>
#include <stdbool.h>
#include "TestStructureDefs.h"

void initTestCases(TestCaseList* testCases);
void loadTests(TestCaseList* testCases, char* basePath);
void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath);
void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName);
void printTestCaseList(const TestCaseList list);
void addTestCasesToList(TestCaseList* list, const char* pathToTestFile);
void addSingleTestCaseToList(TestCaseList* list, const char* pathToTestFile, char* buffer);
void freeTestCasesList(TestCaseList* list);
bool isDirectory(struct dirent *fileOrSubDirectory);
void trimTestName(char* testName);

#endif