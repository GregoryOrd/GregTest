#include "../../GregTest.h"

void testMySt456uff(){
	const char* expected = "Hello World";
	const char* actual = "Hello World";

	G_ASSERT_STR_EQ(expected, actual);
}