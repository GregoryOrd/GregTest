#ifndef G_ASSERT_H
#define G_ASSERT_H

void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);
void G_ASSERT_EQ_LONG(const long expected, const long actual, const char* testName);
void G_ASSERT_EQ_CHAR(const char expected, const char actual, const char* testName);
void G_ASSERT_EQ_UNSIGNED_CHAR(const unsigned char expected, const unsigned char actual, const char* testName);
void G_ASSERT_EQ_SIGNED_CHAR(const signed char expected, const signed char actual, const char* testName);

#endif