#ifndef TESTMAINWRITER_H
#define TESTMAINWRITER_H

#define TEST_MAIN_C_BASE_SIZE 201
#define TEST_MAIN_C_SIZE_INCREMENT_PER_TESTCASE 48
#define TEST_MAIN_H_BASE_SIZE 45
#define TEST_MAIN_H_SIZE_INCREMENT_PER_TESTCASE 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TestStructureDefs.h"

void writeTestsToTestMain(TestFileList* testFiles);
void writeToTestMainC(TestFileList* testFiles);
void populateTestMainCContents(char* contents, TestFileList* testFiles);
void addTestMainCIncludes(char* main);
void addTestMainCFunctionPointerDefinitions(char* main, TestFileList* testFiles);
void addTestMainCFunctionPointerCalls(char* main, TestFileList* testFiles);
void addTestMainCFunctionPointerDefinitionsForSpecificFile(char* main, int numTests, TestCase* cases);
void addTestMainCFunctionPointerCallsForSpecificFile(char* main, int numTests, TestCase* cases);
void addTestMainCResultsCheckAndExits(char* main);
int sizeOfTestMainC(int numTests);

void writeToTestMainH(TestFileList* testFiles);
void writeTestMainHGuardsAndDllDefine(char* contents);
void writeTestMainHGregTestDllImports(char* contents);
void writeTestMainHTestCaseDllImports(char* contents, TestFileList* testFiles);
void writeTestMainHTestCaseDllImportsForSpecificFile(char* contents, int numTests, TestCase* cases);
void writeTestMainHEnd(char* contents);
int sizeOfTestMainH(int numTests);
void writeToFile(const char* filename, const char* contents);

#endif