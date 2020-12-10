#ifndef GREG_TEST_H
#define GREG_TEST_H

#include <string.h>
#include <stdio.h>

#define G_ASSERT_STR_EQ(expected, actual) assertStrEquals(expected, actual, __func__)

bool assertStrEquals(const char* expected, const char* actual, const char* callerName) {
    if(strcmp(expected, actual) == 0)
    {
        printf("[PASS]: %s\n", callerName);
        return true;
    }
    else
    {
        printf("[FAIL]: %s\n[Expected]: %s\n[Actual]: %s\n\n", callerName, expected, actual);
        return false;
    }
}

#endif