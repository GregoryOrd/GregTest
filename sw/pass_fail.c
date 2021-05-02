#include "pass_fail.h"

#include <stdio.h>

#include "G_EXPECT_CALL_DEFS.h"
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

void fail_uint(const char* testName, const unsigned int expected, const unsigned int actual)
{
   printf("[FAIL]: %s\n[Expected]: %u\n[Actual]: %u\n\n", testName, expected, actual);
   andResult(false);
}

void fail_char(const char* testName, const char expected, const char actual)
{
   printf("[FAIL]: %s\n[Expected]: %c\n[Actual]: %c\n\n", testName, expected, actual);
   andResult(false);
}

void fail_uchar(const char* testName, const unsigned char expected, const unsigned char actual)
{
   printf("[FAIL]: %s\n[Expected]: %d\n[Actual]: %d\n\n", testName, expected, actual);
   andResult(false);
}

void fail_schar(const char* testName, const signed char expected, const signed char actual)
{
   printf("[FAIL]: %s\n[Expected]: %d\n[Actual]: %d\n\n", testName, expected, actual);
   andResult(false);
}

void fail_long(const char* testName, const long expected, const long actual)
{
   printf("[FAIL]: %s\n[Expected]: %ld\n[Actual]: %ld\n\n", testName, expected, actual);
   andResult(false);
}

void fail_ulong(const char* testName, const unsigned long expected, const unsigned long actual)
{
   printf("[FAIL]: %s\n[Expected]: %lu\n[Actual]: %lu\n\n", testName, expected, actual);
   andResult(false);
}

void fail_double(const char* testName, const double expected, const double actual)
{
   printf("[FAIL]: %s\n[Expected]: %f\n[Actual]: %f\n\n", testName, expected, actual);
   andResult(false);
}

void fail_double_epsilon(const char* testName, const double expected, const double actual, const double epsilon)
{
   printf("[FAIL]: %s\n[Expected]: %f\n[Actual]: %f\n[EPSILON]: %f\n\n", testName, expected, actual, epsilon);
   andResult(false);
}

void fail_expect_call(const char* testName, LinkedList* missedCalls)
{
   for (int i = 0; i < missedCalls->size; i++)
   {
      const void* functionPtr = at_ll(missedCalls, MISSED_CALLS_TYPE, i);
      printf("[FAIL]: %s\nExpected %p to be called, but it never was.\n\n", testName, functionPtr);
   }
   andResult(false);
}