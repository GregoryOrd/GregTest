#include "../HelloWorld.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool testHelloWorldString()
{
	const char* (*testHelloWorldStringFuncPtr)() = &helloWorldString;

	const char* expected = "Hello World!!!";
	const char* actual = (*testHelloWorldStringFuncPtr)();

	if(strcmp(expected, actual) == 0)
	{
		printf("[PASS]: testHelloWorldString\n");
		return true;
	}
	else
	{
		printf("[FAIL]: testHelloWorldString\n[Expected]: %s\n[Actual]: %s\n\n", expected, actual);
		return false;
	}
}
