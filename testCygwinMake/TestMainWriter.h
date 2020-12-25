#ifndef TESTMAINWRITER_H
#define TESTMAINWRITER_H

#define TEST_MAIN_C_BASE_SIZE 201
#define TEST_MAIN_C_SIZE_INCREMENT_PER_TESTCASE 48
#define TEST_MAIN_H_BASE_SIZE 45
#define TEST_MAIN_H_SIZE_INCREMENT_PER_TESTCASE 20

#include "TestStructures.h"

void writeTestsToTestMain(TestCaseList* testCaseList);
void writeToTestMainC(int numTests, TestCase* cases);
void populateTestMainCContents(char* contents, int numTests, struct TestCase* cases);
void addTestMainCIncludes(char* main);
void addTestMainCFunctionPointerDefinitions(char* main, int numTests, TestCase* cases);
void addTestMainCFunctionPointerCalls(char* main, int numTests, TestCase* cases);
void addTestMainCResultsCheckAndExits(char* main);
int sizeOfTestMainC(int numTests);

void writeToTestMainH(int numTests, TestCase* cases);
void writeTestMainHGuardsAndDllDefine(char* contents);
void writeTestMainHGregTestDllImports(char* contents);
void writeTestMainHTestCaseDllImports(char* contents, int numTests, TestCase* cases);
void writeTestMainHEnd(char* contents);
int sizeOfTestMainH(int numTests);
void writeToFile(const char* filename, const char* contents);

#endif