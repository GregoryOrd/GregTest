#ifndef GREG_TEST_H
#define GREG_TEST_H

#ifdef __WINDOWS__
#define LibraryImport   __declspec(dllimport)
#else
#define LibraryImport   
#endif

#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)
#define G_ASSERT_INT_EQ(expected, actual) G_ASSERT_EQ_INT(expected, actual, __func__)

#ifdef __cplusplus
extern "C" {
#endif  

LibraryImport void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
LibraryImport void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);

#ifdef __cplusplus
}
#endif  

#endif