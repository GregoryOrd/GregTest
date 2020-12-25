#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#include <stdbool.h>
#include "TestStructures.h"

void initTestCases(TestCaseList* testCases);
void loadTests(TestCaseList* testCases, char* basePath);
void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath);
bool isDirectory(struct dirent *fileOrSubDirectory);
void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName);
void printTestCaseList(const TestCaseList list);
void addTestCasesToList(TestCaseList* list, const char* pathToTestFile);
void addSingleTestCaseToList(TestCaseList* list, const char* pathToTestFile, char* buffer);
void freeTestCasesList(TestCaseList* list);
bool isTestDir(char* dirName);
bool isTestFile(struct dirent *fileOrSubDirectory);
void lowerString(char* dest, char* src);
bool isTestCaseDefinition(char* line);
int testNameEndOffset(char* testName);
bool theCurlyBraceIsOnTheSameLineAsTheTestName(char* testName, int initialLength);
bool isSpecialCharacter(char c);

//testName will come in looking like:
//void testExampleName()
//This function will trim of the void and the brackets
void trimTestName(char* testName);

#endif