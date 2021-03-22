#include "pass_fail.h"

#include <stdio.h>

#include "result.h"

void pass(const char* testName) { printf("[PASS]: %s\n", testName); }

void fail_str(const char* testName, const char* expected, const char* actual)
{
   printf("[FAIL]: %s\n[Expected]: %s\n[Actual]: %s\n\n", testName, expected, actual);
   andResult(false);
}

void fail_int(const char* testName, const int expected, const int actual)
{
   printf("[FAIL]: %s\n[Expected]: %d\n[Actual]: %d\n\n", testName, expected, actual);
   andResult(false);
}

void fail_char(const char* testName, const char expected, const char actual)
{
   printf("[FAIL]: %s\n[Expected]: %c\n[Actual]: %c\n\n", testName, expected, actual);
   andResult(false);
}