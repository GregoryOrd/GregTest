#include <stdio.h>
#include "HelloWorld.h"
#include "Hello.h"
#include "World.h"

void main()
{
	printHelloWorld();
}

void printHelloWorld()
{
	char* helloWorldStr;
	helloWorldString(helloWorldStr);
	printf(helloWorldStr);
}

void helloWorldString(char* helloWorldStr)
{
	hello(helloWorldStr);
	world(helloWorldStr);
}
