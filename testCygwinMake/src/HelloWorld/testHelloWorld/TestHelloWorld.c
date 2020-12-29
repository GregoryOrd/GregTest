#include "../HelloWorld.h"
#include "../../GregTest.h"

void testHelloWorldString()
{
	const char* expected = "Hello World";
	const char* actual = helloWorldString();

	G_ASSERT_STR_EQ(expected, actual);
}
