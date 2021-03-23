#ifndef GREG_TEST_H
#define GREG_TEST_H

#ifdef __WINDOWS__
#define LibraryImport   __declspec(dllimport)
#else
#define LibraryImport   
#endif

#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)
#define G_ASSERT_INT_EQ(expected, actual) G_ASSERT_EQ_INT(expected, actual, __func__)
#define G_ASSERT_LONG_EQ(expected, actual) G_ASSERT_EQ_LONG(expected, actual, __func__)
#define G_ASSERT_CHAR_EQ(expected, actual) G_ASSERT_EQ_CHAR(expected, actual, __func__)
#define G_ASSERT_UNSIGNED_CHAR_EQ(expected, actual) G_ASSERT_EQ_UNSIGNED_CHAR(expected, actual, __func__)
#define G_ASSERT_SIGNED_CHAR_EQ(expected, actual) G_ASSERT_EQ_SIGNED_CHAR(expected, actual, __func__)

#ifdef __cplusplus
extern "C" {
#endif  

LibraryImport void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
LibraryImport void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);
LibraryImport void G_ASSERT_EQ_LONG(const long expected, const long actual, const char* testName);
LibraryImport void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName);
LibraryImport void G_ASSERT_EQ_UNSIGNED_CHAR(const unsigned char expected, const unsigned char actual, const char* testName);
LibraryImport void G_ASSERT_EQ_SIGNED_CHAR(const signed char expected, const signed char actual, const char* testName);

#ifdef __cplusplus
}
#endif  

#endif