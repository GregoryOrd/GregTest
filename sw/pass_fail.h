#ifndef PASS_FAIL_H
#define PASS_FIAL_H

void pass(const char* testName);

void fail_str(const char* testName, const char* expected, const char* actual);
void fail_int(const char* testName, const int expected, const int actual);
void fail_long(const char* testName, const long expected, const long actual);
void fail_char(const char* testName, const char expected, const char actual);
void fail_uchar(const char* testName, const unsigned char expected, const unsigned char actual);
void fail_schar(const char* testName, const signed char expected, const signed char actual);

#endif