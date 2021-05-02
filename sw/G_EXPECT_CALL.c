#include "G_EXPECT_CALL.h"

#include <stdlib.h>

#include "../../GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "G_EXPECT_CALL_DEFS.h"
#include "pass_fail.h"

expect_call_origin origin = {0, false, true};
executing_test executingTest = {"", false};
LinkedList* expectedCalls;
LinkedList* actualCalls;
LinkedList* missedCalls;

void freeFunctionPointerData(void* functionPtr)
{
   // Do nothing. We don't want to free the memory address because it is pointing to code, not data.
}

void __cyg_profile_func_enter(void* this_fn, void* call_site)
{
   if (origin.hasReturned == true)
   {
      origin.origin = this_fn;
      origin.hasReturned = false;

      expectedCalls = (LinkedList*)malloc(sizeof(LinkedList));
      initEmptyLinkedList(expectedCalls, EXPECTED_CALLS_TYPE);

      actualCalls = (LinkedList*)malloc(sizeof(LinkedList));
      initEmptyLinkedList(actualCalls, ACTUAL_CALLS_TYPE);

      missedCalls = (LinkedList*)malloc(sizeof(LinkedList));
      initEmptyLinkedList(missedCalls, MISSED_CALLS_TYPE);
   }
   else
   {
      append_ll(actualCalls, this_fn, ACTUAL_CALLS_TYPE);
   }
}

void __cyg_profile_func_exit(void* this_fn, void* call_site)
{
   if (origin.isValid && this_fn == origin.origin)
   {
      origin.isValid = false;

      bool correctCalls = true;
      for (int i = 0; i < expectedCalls->size; i++)
      {
         void* expectedCall = (void*)at_ll(expectedCalls, EXPECTED_CALLS_TYPE, i);
         bool callFound = contains_ll(actualCalls, expectedCall, ACTUAL_CALLS_TYPE);
         correctCalls &= callFound;
         if (!callFound)
         {
            append_ll(missedCalls, expectedCall, MISSED_CALLS_TYPE);
         }
      }
      if (correctCalls)
      {
         pass(executingTest.testName);
      }
      else
      {
         fail_expect_call(executingTest.testName, missedCalls);
      }
   }

   if (this_fn == origin.origin)
   {
      origin.hasReturned = true;
      freeLinkedList(expectedCalls, freeFunctionPointerData);
      freeLinkedList(actualCalls, freeFunctionPointerData);
      freeLinkedList(missedCalls, freeFunctionPointerData);
   }
}

void G_EXPECT_FUNCTION_CALLED(const void* functionPtr, const char* testName)
{
   origin.isValid = true;
   append_ll(expectedCalls, (void*)functionPtr, EXPECTED_CALLS_TYPE);
   executingTest.testName = testName;
}