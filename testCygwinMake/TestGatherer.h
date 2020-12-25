#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#include <stdbool.h>
#include "TestStructures.h"

typedef struct LineMetrics
{
    int numSpaces;
    int numLeftBrackets;
    int numRightBrackets;
    int leftBracketIndex;
    int rightBracketIndex;
    int length;
    int expectedLeftBracketIndex;
    int expectedRightBracketIndex;
} LineMetrics;

typedef struct LineAnalysisResults
{
    bool correctStartOfLine;
    bool singleSpaceBetweenBoolAndTestName;
    bool hasSpecialCharacters;
    bool correctSpaces;
    bool correctBracketCount;
    bool correctBracketPosition;
    bool correctBrackets;
} LineAnalysisResults;

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
void gatherLineMetrics(LineMetrics* metrics, char* line);
void analyzeLineMetrics(LineMetrics* metrics, char* line);
void determineResults(LineAnalysisResults* results, LineMetrics* metrics, char* line);
bool lineHasSpecialCharacters(LineMetrics* metrics, char* line);
void initLineMetrics(LineMetrics* metrics);
void initLineAnalysisResults(LineAnalysisResults* results);
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