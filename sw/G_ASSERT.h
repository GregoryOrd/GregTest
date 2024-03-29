#ifndef G_ASSERT_H
#define G_ASSERT_H

#include <stdbool.h>

void G_TRUE_EXPECT(const bool actual, const char* testName);
void G_FALSE_EXPECT(const bool actual, const char* testName);
void G_ASSERT_EQ_BOOL(const bool expected, const bool actual, const char* testName);
void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);
void G_ASSERT_EQ_UNSIGNED_INT(const unsigned int expected, const unsigned int actual, const char* testName);
void G_ASSERT_EQ_LONG(const long expected, const long actual, const char* testName);
void G_ASSERT_EQ_UNSIGNED_LONG(const unsigned long expected, const unsigned long actual, const char* testName);
void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName);
void G_ASSERT_EQ_UNSIGNED_CHAR(const unsigned char expected, const unsigned char actual, const char* testName);
void G_ASSERT_EQ_SIGNED_CHAR(const signed char expected, const signed char actual, const char* testName);
void G_ASSERT_EQ_FLOAT(const float expected, const float actual, const char* testName);
void G_ASSERT_EQ_DOUBLE(const double expected, const double actual, const char* testName);
void G_ASSERT_NEAR_DOUBLE(const double expected, const double actual, const double epsilon, const char* testName);

#endif