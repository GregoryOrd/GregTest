#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct TestFileList
{
    int size;
    char** files;
} TestFileList;

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

void loadTests(TestFileList* TestFileList, TestCaseList* testCases, char* basePath);
void printTestFileList(const TestFileList TestFileList);
void printTestCaseList(const TestCaseList list);
void addFileToList(TestFileList* list, const char* path);
void addTestCasesToList(TestCaseList* list, const char* path);
void freeTestFilesList(TestFileList* list);
void freeTestCasesList(TestCaseList* list);
bool isTestDir(char* dirName);
bool isTestFile(char* dirName);
char* lowerString(char* str);
bool isTestCaseDefinition(char* line);
void trimTestName(char* testName);

#endif