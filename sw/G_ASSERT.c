#include "G_ASSERT.h"

#include <string.h>

#include "pass_fail.h"

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

void G_ASSERT_EQ_UNSIGNED_INT(const unsigned int expected, const unsigned int actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_uint(testName, expected, actual);
   }
}

void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_char(testName, expected, actual);
   }
}

void G_ASSERT_EQ_UNSIGNED_CHAR(const unsigned char expected, const unsigned char actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_uchar(testName, expected, actual);
   }
}

void G_ASSERT_EQ_SIGNED_CHAR(const signed char expected, const signed char actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_schar(testName, expected, actual);
   }
}

void G_ASSERT_EQ_LONG(const long expected, const long actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_long(testName, expected, actual);
   }
}

void G_ASSERT_EQ_UNSIGNED_LONG(const unsigned long expected, const unsigned long actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_ulong(testName, expected, actual);
   }
}

void G_ASSERT_EQ_FLOAT(const float expected, const float actual, const char* testName) { G_ASSERT_EQ_DOUBLE(expected, actual, testName); }

void G_ASSERT_EQ_DOUBLE(const double expected, const double actual, const char* testName)
{
   if (expected == actual)
   {
      pass(testName);
   }
   else
   {
      fail_double(testName, expected, actual);
   }
}