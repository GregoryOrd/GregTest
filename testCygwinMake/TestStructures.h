#ifndef TESTSTRUCTURES_H
#define TESTSTRUCTURES_H

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
#else 
    #include <dirent.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOWS_MAX_PATH_LENGTH 260
#define TEST_NAME_TRIM_FRONT_OFFSET 5
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE 8
#define TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE 9

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