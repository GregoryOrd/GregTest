#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "TestMain.h"

int main()
{
	void (*testHelloWorldString_fun_ptr_)(void) = &testHelloWorldString;
	void (*testMySt456uff_fun_ptr_)(void) = &testMySt456uff;

	(*testHelloWorldString_fun_ptr_)();
	(*testMySt456uff_fun_ptr_)();

	if(result())
	{
		printf("All Tests Passed Successfully");
		exit(0);
	}
	else
	{
		printf("Tests Failed");
	}
	exit(1);
}
