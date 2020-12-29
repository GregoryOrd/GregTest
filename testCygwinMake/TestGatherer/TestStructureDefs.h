#ifndef TESTSTRUCTUREDEFS_H
#define TESTSTRUCTUREDEFS_H

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

#endif