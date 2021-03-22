#include "GregTest_Private.h"

void pass(const char* testName) { printf("[PASS]: %s\n", testName); }

void fail_str(const char* testName, const char* expected, const char* actual)
{
   printf("[FAIL]: %s\n[Expected]: %s\n[Actual]: %s\n\n", testName, expected, actual);
   result_ = result_ & false;
}

void fail_int(const char* testName, const int expected, const int actual)
{
   printf("[FAIL]: %s\n[Expected]: %d\n[Actual]: %d\n\n", testName, expected, actual);
   result_ = result_ & false;
}

bool result() { return result_; }

void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName)
{
   if (strcmp(expected, actual) == 0)
   {
      pass(testName);
   }
   else
   {
      fail_str(testName, expected, actual);
   }
}

void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_int(testName, expected, actual);
   }
}