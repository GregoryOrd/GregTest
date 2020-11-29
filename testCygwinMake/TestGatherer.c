#include <stdio.h> 
#include <dirent.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "TestGatherer.h"


int main(void) 
{ 
    char dir[WINDOWS_MAX_PATH_LENGTH] = ".";

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    testCases->size = 0;
    testCases->cases = (TestCase*)malloc(sizeof(TestCase));
    testCases->cases[0].testFile = NULL;
    testCases->cases[0].testName = NULL;

    loadTests(testCases, dir);
    printf("\n\nFINAL PRINT:\n");
    printTestCaseList(*testCases);   

    writeTestsToTestMain(testCases);

    freeTestCasesList(testCases);
    exit(0);
} 

void loadTests(TestCaseList* testCases, char* basePath)
{
    char* path = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
    {
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        strcpy(path, basePath);
        strcat(path, "/");
        strcat(path, dp->d_name);
  
        if(isTestDir(basePath) && isTestFile(dp->d_name))
        {
            addTestCasesToList(testCases, path);
        }
        if (strncmp(dp->d_name, ".", 1) != 0 && dp->d_type == DT_DIR)
        {
            loadTests(testCases, path);
        }
    }

    closedir(dir);
    free(path);
}

void printTestCaseList(const TestCaseList list)
{
    printf("====================================\n");
    for(int i = 0; i < list.size; i++)
    {
        printf("file: %s || testCase: %s\n", list.cases[i].testFile, list.cases[i].testName);
    }  
    printf("====================================\n");
}


void freeTestCasesList(TestCaseList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->cases[i].testFile);
        free(list->cases[i].testName);
    }
    free(list);
}

void addTestCasesToList(TestCaseList* list, const char* path)
{
    FILE *fp;
    char* buff = (char*)malloc(255*sizeof(char));

    fp = fopen(path, "r");

    while(fgets(buff, 255, (FILE*)fp) != NULL)
    {
        if(isTestCaseDefinition(buff))
        {
            list->cases = (TestCase*)realloc(list->cases, ((list->size + 1) * sizeof(TestCase)));
            list->cases[list->size].testName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
            list->cases[list->size].testFile = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 

            trimTestName(buff); 

            strcpy(list->cases[list->size].testName, buff);
            strcpy(list->cases[list->size].testFile, path);
            list->size++;
        }  
    }

    free(buff);
    fclose(fp);
}

bool isTestDir(char* dirName)
{
    bool result = false;
    char* lower = lowerString(dirName);
    if(strstr(lower, "/test") != NULL)
    {
        result = true;
    }
    free(lower);
    return result;
}

bool isTestFile(char* dirName)
{
    bool result = false;
    char* lower = lowerString(dirName);
    if(strncmp(lower, "test", 4) == 0 && (strstr(lower, ".c") != NULL || strstr(lower, ".cpp") != NULL))
    {
        result = true;
    }
    free(lower);
    return result;
}

char* lowerString(char* str)
{
    int size = 0;

    char* lower = (char*)malloc(sizeof(char));
    lower[0] = '\0';

    char* itr = str;
    while(*itr != '\0')
    {
        char l = tolower(*itr);
        lower = (char*)realloc(lower, size+2);
        lower[size] = l;
        lower[size+1] = '\0';
        size++;
        itr++;
    }
    return lower;
}

bool isTestCaseDefinition(char* line)
{
    bool correctStartOfLine = strncmp(line, "bool test", 9) == 0;
    bool singleSpaceBetweenBoolAndTestName = (strstr(line, " ") == &line[4]);

    int numSpaces = 0;
    int numLeftBrackets = 0;
    int numRightBrackets = 0;
    bool hasSpecialCharacters = false;
    int leftBracketIndex = 0;
    int rightBracketIndex = 0;

    int length = 0;
    char* currentPtr = line;
    while(*currentPtr != '\0' && *currentPtr != '\n')
    {
        if(*currentPtr == ' ')
        {
            numSpaces++;
        }
        else if(*currentPtr == '(')
        {
            numLeftBrackets++;
            leftBracketIndex = length;
        }
        else if(*currentPtr == ')')
        {
            numRightBrackets++;
            rightBracketIndex = length;
        }
        length++;
        currentPtr++;
    }


    bool correctSpaces = singleSpaceBetweenBoolAndTestName && numSpaces == 1;

    int expectedLeftBracketIndex = length - 3;
    if(line[length-2] == '{')
    {
        expectedLeftBracketIndex--;
    }
    int expectedRightBracketIndex = expectedLeftBracketIndex + 1; 

    bool correctBracketCount = numLeftBrackets == 1 && numRightBrackets == 1; 
    bool correctBracketPosition = leftBracketIndex == expectedLeftBracketIndex && rightBracketIndex == expectedRightBracketIndex;
    bool correctBrackets = correctBracketCount && correctBracketPosition;

    if(correctStartOfLine && correctSpaces && correctBrackets)
    {
        for(int i = 0; i < length; i++)
        {
            if(isSpecialCharacter(line[i]))
            {
                hasSpecialCharacters = true;
            }
        }

        if(!hasSpecialCharacters)
        {
            return true;
        }
        else
        {
            printf("\nGregTest does not accept test cases with special characters in the name of the test.\n");
            printf("%s\n\n", line);
            return false;
        }
        
    }
    return false;
}

void trimTestName(char* testName)
{
    //testName will come in looking like:
    //  bool testExampleName()
    //This function will trim of the bool and the brackets
    int offset = 8;


    int count = 0;
    char* currentPtr = testName;
    while(*currentPtr != '\0' && *currentPtr != '\n')
    {
        count++;   
        currentPtr++;
    }

    if(testName[count-2] == '{')
    {
        offset++;
    }

    char temp[count];
    strcpy(temp, testName);

    for(int i = 0; i < count; i++)
    {
        if(i < count - offset)
        {
            testName[i] = testName[i+5];
        }
        else{
            testName[i] = '\0';
        }
    }
}

bool isSpecialCharacter(char c)
{
    bool isCarriageReturn = c == 13;
    bool isNewLine = c == '\n';
    bool isNumber = c >= '0' && c <= '9';
    bool isUpperCaseLetter = c >= 'A' && c <= 'Z';
    bool isLowerCaseLetter = c >= 'a' && c <= 'z';
    bool isSpace = c == ' ';
    bool isLeftCurrlyBracket = c == '{';
    bool isParenthesis = (c == '(') || (c == ')');

    if(isCarriageReturn || isNewLine || isNumber || isUpperCaseLetter || isLowerCaseLetter || isSpace || isLeftCurrlyBracket || isParenthesis)
    {
        return false;
    }
    return true;
}

void writeTestsToTestMain(TestCaseList* testCaseList)
{
    int numTests = testCaseList->size;
    int sizeOfTestMainCxx = 220;
    int sizeOfTestMainH = 45;

    if(numTests >= 1)
    {
        sizeOfTestMainCxx += 24;
    }
    sizeOfTestMainCxx += (numTests * (67 + WINDOWS_MAX_PATH_LENGTH * sizeof(char*)));
    sizeOfTestMainH += (numTests * (20 + WINDOWS_MAX_PATH_LENGTH * sizeof(char*)));

    char main[sizeOfTestMainCxx];
    char mainH[sizeOfTestMainH];
    main[0] = '\0';
    mainH[0] = '\0';

    strcat(mainH, "#define DllImport   __declspec(dllimport)\n\n");

    strcat(main, "#include <stdbool.h>\n");
    strcat(main, "#include <stdio.h>\n");
    strcat(main, "#include <stdlib.h>\n");
    strcat(main, "#include \"TestMain.h\"\n\n");

    strcat(main, "int main()\n{\n");
    strcat(main, "\tbool result = true;\n");

    //
    //Section for generated code:
    //For TestMain.cxx
    //  bool (*testHelloWorldString_fun_ptr_)(void) = &testHelloWorldString;
    //
    //For TestMain.h
    //  DllImport bool testHelloWorldString();
    //
    for(int i = 0; i < numTests; i++)
    {
        TestCase* cases = testCaseList->cases;
        char* testName = cases[i].testName;
        strcat(main, "\tbool (*");
        strcat(main, testName);
        strcat(main, "_fun_ptr_)(void) = &");
        strcat(main, testName);
        strcat(main, ";\n");

        strcat(mainH, "DllImport bool ");
        strcat(mainH, testName);
        strcat(mainH, "();\n");
    }

    strcat(main, "\n");

    //
    //Section for generated code:
    //  result = result && (*testHelloWorldString_fun_ptr_)();
    //
    for(int i = 0; i < numTests; i++)
    {
        TestCase* cases = testCaseList->cases;
        char* testName = cases[i].testName;
        strcat(main, "\tresult = result && (*");
        strcat(main, testName);
        strcat(main, "_fun_ptr_)();\n");
    }

    strcat(main, "\n");

    strcat(main, "\tif(result)\n");
    strcat(main, "\t{\n");
    strcat(main, "\t\tprintf(\"All Tests Passed Successfully\");\n");
    strcat(main, "\t\texit(0);\n");
    strcat(main, "\t}\n");
    strcat(main, "\texit(1);\n");
    strcat(main, "}\n");

    printf("MAIN:\n\n\n");
    printf("%s", main);

    FILE *fp;
    fp = fopen("TestMain.c", "w");
    int writeResult =  fputs(main, fp);
    if(writeResult == EOF)
    {
        printf("Error writing test cases to TestMain.c\n");
        exit(1);
    }
    fclose(fp);

    FILE *fpH;
    fpH = fopen("TestMain.h", "w");
    int writeResultH =  fputs(mainH, fpH);
    if(writeResultH == EOF)
    {
        printf("Error writing test cases to TestMain.h\n");
        exit(1);
    }
    fclose(fpH);
}