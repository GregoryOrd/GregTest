#ifndef TESTSTRUCTURES_H
#define TESTSTRUCTURES_H

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
#else 
    #include <dirent.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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