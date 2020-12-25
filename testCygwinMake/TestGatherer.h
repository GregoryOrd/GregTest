#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#include <stdbool.h>
#include "TestStructures.h"

void initTestCases(TestCaseList* testCases);
void loadTests(TestCaseList* testCases, char* basePath);
void printTestCaseList(const TestCaseList list);
void addTestCasesToList(TestCaseList* list, const char* path);
void freeTestCasesList(TestCaseList* list);
bool isTestDir(char* dirName);
bool isTestFile(char* dirName);
char* lowerString(char* str);
bool isTestCaseDefinition(char* line);
void trimTestName(char* testName);
bool isSpecialCharacter(char c);

#endif