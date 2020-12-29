#include <stdio.h>
#include "HelloWorld.h"

void main()
{
	printHelloWorld();
}

void printHelloWorld()
{
	const char* helloWorldStr = helloWorldString();
	printf(helloWorldStr);
}

const char* helloWorldString()
{
	return "Hello World";
}
