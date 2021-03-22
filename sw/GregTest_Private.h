#ifndef GREG_TEST_PRIVATE_H
#define GREG_TEST_PRIVATE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define G_ASSERT_STR_EQ(expected, actual) G_ASSERT_EQ_STR(expected, actual, __func__)
#define G_ASSERT_INT_EQ(expected, actual) G_ASSERT_EQ_INT(expected, actual, __func__)

bool result_ = true;

void pass(const char* testName);
void fail_str(const char* testName, const char* expected, const char* actual);
void fail_int(const char* testName, const int expected, const int actual);
bool result();

void G_ASSERT_EQ_STR(const char* expected, const char* actual, const char* testName);
void G_ASSERT_EQ_INT(const int expected, const int actual, const char* testName);

#endif