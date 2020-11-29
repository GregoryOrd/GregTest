#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct TestCase
{
    char* testName;
    char* testFile;
} TestCase;

typedef struct TestCaseList
{
    int size;
    TestCase* cases;
} TestCaseList;

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
void writeTestsToTestMain(TestCaseList* testCaseList);

#endif