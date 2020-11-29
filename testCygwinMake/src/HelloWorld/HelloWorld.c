#include <stdio.h>
#include "HelloWorld.h"

void printHelloWorld()
{
	const char* helloWorldStr = helloWorldString();
	printf(helloWorldStr);
}

const char* helloWorldString()
{
	return "Hello World";
}
