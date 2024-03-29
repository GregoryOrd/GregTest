#ifndef PASS_FAIL_H
#define PASS_FIAL_H

#include "../../GregCToolkit/sw/Collections/LinkedList/LinkedList.h"

void pass(const char* testName);

void fail_bool(const char* testName, const bool expected, const bool actual);
void fail_str(const char* testName, const char* expected, const char* actual);
void fail_int(const char* testName, const int expected, const int actual);
void fail_uint(const char* testName, const unsigned int expected, const unsigned actual);
void fail_long(const char* testName, const long expected, const long actual);
void fail_ulong(const char* testName, const unsigned long expected, const unsigned long actual);
void fail_char(const char* testName, const char expected, const char actual);
void fail_uchar(const char* testName, const unsigned char expected, const unsigned char actual);
void fail_schar(const char* testName, const signed char expected, const signed char actual);
void fail_double(const char* testName, const double expected, const double actual);
void fail_double_epsilon(const char* testName, const double expected, const double actual, const double epsilon);
void fail_expect_call(const char* testName, LinkedList* missedCalls);
void fail_expect_true(const char* testName);
void fail_expect_false(const char* testName);

#endif