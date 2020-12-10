#include "../HelloWorld.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../GregTest.h"

bool testHelloWorldString()
{
	const char* (*testHelloWorldStringFuncPtr)() = &helloWorldString;

	const char* expected = "Hello World";
	const char* actual = (*testHelloWorldStringFuncPtr)();

	return G_ASSERT_STR_EQ(expected, actual);
}
