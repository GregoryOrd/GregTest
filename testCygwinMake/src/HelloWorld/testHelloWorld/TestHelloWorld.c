#include "../HelloWorld.h"
#include "../../../GregTest.h"

void testHelloWorldString()
{
	const char* (*testHelloWorldStringFuncPtr)() = &helloWorldString;

	const char* expected = "Hello World";
	const char* actual = (*testHelloWorldStringFuncPtr)();

	G_ASSERT_STR_EQ(expected, actual);
}
