#ifndef TEST_DEFINTIONS
#define TEST_DEFINITIONS

#define TEST_NAME_TRIM_FRONT_OFFSET 5
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE 8
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE 9

#include <dirent.h>
#include <stdbool.h>

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
bool isSourceFile(struct dirent *fileOrSubDirectory);
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