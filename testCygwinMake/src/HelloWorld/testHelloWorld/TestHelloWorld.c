#include "../HelloWorld.h"
#include "../../GregTest.h"
#include <stdio.h>

void testHelloWorldString()
{
	const char* expected = "Hello World";
	char actual[255];
	helloWorldString(actual);

	G_ASSERT_STR_EQ(expected, actual);
}
