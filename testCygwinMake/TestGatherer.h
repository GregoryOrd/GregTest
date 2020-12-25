#ifndef TESTGATHERER_H
#define TESTGATHERER_H

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

//testName will come in looking like:
//void testExampleName()
//This function will trim off the void and the brackets
void trimTestName(char* testName);


// Preparing to split out a TestFileDefinitions.h/.c and TestCaseDefinitions.h/.c
// Above this comment will stay in the TestGatherer, the function below will be split out.
// This refactor should help keep things modular, reduce file size, and hopefully represent logical
// seam where we might be able to bring in an XML reader module to allow these definitions to be
// resource configuarable and read in at runtime. 


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


#endif