#ifndef TESTGATHERER_H
#define TESTGATHERER_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct FileList
{
    int size;
    char** files;
} FileList;

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

void loadTests(FileList* fileList, TestCaseList* testCases, char* basePath);
void printFileList(const FileList fileList);
void addFileToList(FileList* list, const char* path);
void addTestCasesToList(TestCaseList* list, const char* path);
void freeFileListFiles(FileList* list);
bool isTestDir(char* dirName);
bool isTestFile(char* dirName);
char* lowerString(char* str);
bool isTestCaseDefinition(char* line);

#endif