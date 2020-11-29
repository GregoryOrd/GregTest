#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "TestMain.h"

int main()
{
    bool (*testHelloWorldString_fun_ptr_)(void) = &testHelloWorldString;
	bool result = (*testHelloWorldString_fun_ptr_)();


	if(result)
	{
		printf("\nAll Tests Passed Successfully\n");
		exit(0);
	}
	exit(1);
}