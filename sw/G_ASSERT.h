#ifndef G_ASSERT_H
#define G_ASSERT_H

#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)
#define G_ASSERT_INT_EQ(expected, actual) G_ASSERT_EQ_INT(expected, actual, __func__)
#define G_ASSERT_CHAR_EQ(expected, actual) G_ASSERT_EQ_CHAR(expected, actual, __func__)

void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);
void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName);

#endif