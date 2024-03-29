#ifndef GREG_TEST_H
#define GREG_TEST_H

#ifdef __WINDOWS__
#define LibraryImport   __declspec(dllimport)
#else
#define LibraryImport   
#endif

#include <stdbool.h>

#define G_EXPECT_TRUE(actual) G_TRUE_EXPECT(actual, __func__)
#define G_EXPECT_FALSE(actual) G_FALSE_EXPECT(actual, __func__)
#define G_ASSERT_BOOL_EQ(expected, actual) G_ASSERT_EQ_BOOL(expected, actual, __func__)
#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)
#define G_ASSERT_INT_EQ(expected, actual) G_ASSERT_EQ_INT(expected, actual, __func__)
#define G_ASSERT_UNSIGNED_INT_EQ(expected, actual) G_ASSERT_EQ_UNSIGNED_INT(expected, actual, __func__)
#define G_ASSERT_LONG_EQ(expected, actual) G_ASSERT_EQ_LONG(expected, actual, __func__)
#define G_ASSERT_UNSIGNED_LONG_EQ(expected, actual) G_ASSERT_EQ_UNSIGNED_LONG(expected, actual, __func__)
#define G_ASSERT_CHAR_EQ(expected, actual) G_ASSERT_EQ_CHAR(expected, actual, __func__)
#define G_ASSERT_UNSIGNED_CHAR_EQ(expected, actual) G_ASSERT_EQ_UNSIGNED_CHAR(expected, actual, __func__)
#define G_ASSERT_SIGNED_CHAR_EQ(expected, actual) G_ASSERT_EQ_SIGNED_CHAR(expected, actual, __func__)
#define G_ASSERT_FLOAT_EQ(expected, actual) G_ASSERT_EQ_FLOAT(expected, actual, __func__)
#define G_ASSERT_DOUBLE_EQ(expected, actual) G_ASSERT_EQ_DOUBLE(expected, actual, __func__)
#define G_ASSERT_FLOAT_NEAR(expected, actual, epsilon) G_ASSERT_NEAR_FLOAT(expected, actual, epsilon, __func__)
#define G_ASSERT_DOUBLE_NEAR(expected, actual, epsilon) G_ASSERT_NEAR_DOUBLE(expected, actual, epsilon, __func__)
#define G_EXPECT_CALL(function) G_EXPECT_FUNCTION_CALLED(function, __func__)

#ifdef __cplusplus
extern "C" {
#endif  

LibraryImport void G_TRUE_EXPECT(const bool actual, const char* testName);
LibraryImport void G_FALSE_EXPECT(const bool actual, const char* testName);
LibraryImport void G_ASSERT_EQ_BOOL(const bool expected, const bool actual, const char* testName);
LibraryImport void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
LibraryImport void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);
LibraryImport void G_ASSERT_EQ_UNSIGNED_INT(const unsigned int expected, const unsigned int actual, const char* testName);
LibraryImport void G_ASSERT_EQ_LONG(const long expected, const long actual, const char* testName);
LibraryImport void G_ASSERT_EQ_UNSIGNED_LONG(const unsigned long expected, const unsigned long actual, const char* testName);
LibraryImport void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName);
LibraryImport void G_ASSERT_EQ_UNSIGNED_CHAR(const unsigned char expected, const unsigned char actual, const char* testName);
LibraryImport void G_ASSERT_EQ_SIGNED_CHAR(const signed char expected, const signed char actual, const char* testName);
LibraryImport void G_ASSERT_EQ_FLOAT(const float expected, const float actual, const char* testName);
LibraryImport void G_ASSERT_EQ_DOUBLE(const double expected, const double actual, const char* testName);
LibraryImport void G_ASSERT_NEAR_FLOAT(const float expected, const float actual, const float epsilon, const char* testName);
LibraryImport void G_ASSERT_NEAR_DOUBLE(const double expected, const double actual, const double epsilon, const char* testName);
LibraryImport void G_EXPECT_FUNCTION_CALLED(const void* functionPtr, const char* testName);

#ifdef __cplusplus
}
#endif  

#endif