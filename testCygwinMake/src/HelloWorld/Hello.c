#include "Hello.h"
#include <string.h>

void hello(char* str)
{
    int len = strlen(str);
    for(int i = 0; i < len; i++)
    {
        str[i] = '\0';
    }
    strcpy(str, "Hello ");
}