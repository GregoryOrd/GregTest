#ifndef TESTSTRUCTUREDEFS_H
#define TESTSTRUCTUREDEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

typedef struct TestCase
{
    char* testName;
} TestCase;

typedef struct TestFile
{
    char* name;
    int numTestCases;
    TestCase* cases;
} TestFile;

typedef struct TestFileList
{
    int size;
    int totalNumTestCases;
    TestFile* files;
} TestFileList;

#endif