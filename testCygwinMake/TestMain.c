#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "TestMain.h"

int main()
{
	bool result = true;
	bool (*testHelloWorldString_fun_ptr_)(void) = &testHelloWorldString;
	bool (*testMySt456uff_fun_ptr_)(void) = &testMySt456uff;

	result = result && (*testHelloWorldString_fun_ptr_)();
	result = result && (*testMySt456uff_fun_ptr_)();

	if(result)
	{
		printf("All Tests Passed Successfully");
		exit(0);
	}
	else
	{
		printf("Tests Failed\n");
	}
	
	exit(1);
}
