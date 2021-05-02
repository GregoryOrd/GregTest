#ifndef G_EXPECT_CALL_H
#define G_EXPECT_CALL_H

#include <stdbool.h>

void __cyg_profile_func_enter (void *this_fn,
                               void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit  (void *this_fn,
                               void *call_site) __attribute__((no_instrument_function));

typedef struct
{
    const char* testName;
    bool result;
} executing_test;


typedef struct 
{
    void* origin;
    bool isValid;
    bool hasReturned;
} expect_call_origin;

void G_EXPECT_FUNCTION_CALLED(const void* functionPtr, const char* testName);

#endif