#ifndef GREG_TEST_H
#define GREG_TEST_H

#define DllImport   __declspec(dllimport)

#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)

DllImport void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);

#endif