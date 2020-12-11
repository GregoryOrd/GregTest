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
    bool correctStartOfLine = strncmp(line, "void test", 9) == 0;
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
    //  void testExampleName()
    //This function will trim of the void and the brackets
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
    TestCase* cases = testCaseList->cases;
    writeToTestMainC(numTests, cases);
    writeToTestMainH(numTests, cases);
}

void writeToTestMainC(int numTests, TestCase* cases)
{
    int size = sizeOfTestMainC(numTests);
    char contents[size];
    contents[0] = '\0';
    populateTestMainCContents(contents, numTests, cases);
    writeToFile("TestMain.c", contents);
}

void populateTestMainCContents(char* contents, int numTests, TestCase* cases)
{
    addTestMainCIncludes(contents);
    strcat(contents, "int main()\n{\n");
    addTestMainCFunctionPointerDefinitions(contents, numTests, cases);
    addTestMainCFunctionPointerCalls(contents, numTests, cases);
    addTestMainCResultsCheckAndExits(contents);
}

void addTestMainCIncludes(char* main)
{
    strcat(main, "#include <stdbool.h>\n");
    strcat(main, "#include <stdio.h>\n");
    strcat(main, "#include <stdlib.h>\n");
    strcat(main, "#include \"TestMain.h\"\n\n");
}

void addTestMainCFunctionPointerDefinitions(char* main, int numTests, TestCase* cases)
{
    for(int i = 0; i < numTests; i++)
    {
        char* testName = cases[i].testName;
        strcat(main, "\tvoid (*");
        strcat(main, testName);
        strcat(main, "_fun_ptr_)(void) = &");
        strcat(main, testName);
        strcat(main, ";\n");
    }
    strcat(main, "\n");
}

void addTestMainCFunctionPointerCalls(char* main, int numTests, TestCase* cases)
{
    for(int i = 0; i < numTests; i++)
    {
        char* testName = cases[i].testName;
        strcat(main, "\t(*");
        strcat(main, testName);
        strcat(main, "_fun_ptr_)();\n");
    }
    strcat(main, "\n");
}

void addTestMainCResultsCheckAndExits(char* main)
{
    strcat(main, "\tif(result())\n");
    strcat(main, "\t{\n");
    strcat(main, "\t\tprintf(\"All Tests Passed Successfully\");\n");
    strcat(main, "\t\texit(0);\n");
    strcat(main, "\t}\n");
    strcat(main, "\telse\n");
    strcat(main, "\t{\n");
    strcat(main, "\t\tprintf(\"Tests Failed\");\n");
    strcat(main, "\t}\n");
    strcat(main, "\texit(1);\n");
    strcat(main, "}\n");
}

void writeToTestMainH(int numTests, TestCase* cases)
{
    int size = sizeOfTestMainH(numTests);
    char contents[size];
    contents[0] = '\0';
    writeTestMainHGuardsAndDllDefine(contents);
    writeTestMainHGregTestDllImports(contents);
    writeTestMainHTestCaseDllImports(contents, numTests, cases);
    writeTestMainHEnd(contents);
    writeToFile("TestMain.h", contents);
}

void writeTestMainHGuardsAndDllDefine(char* contents)
{
    strcat(contents, "#ifndef TEST_MAIN_H\n");
    strcat(contents, "#define TEST_MAIN_H\n\n");
    strcat(contents, "#define DllImport __declspec(dllimport)\n\n");
}

void writeTestMainHGregTestDllImports(char* contents)
{
    strcat(contents, "//For GregTest\n");
    strcat(contents, "DllImport bool result();\n\n");
}

void writeTestMainHTestCaseDllImports(char* contents, int numTests, TestCase* cases)
{
    strcat(contents, "//From Tests Written Throughout the Repo\n");
    for(int i = 0; i < numTests; i++)
    {
        char* testName = cases[i].testName;

        strcat(contents, "DllImport void ");
        strcat(contents, testName);
        strcat(contents, "();\n");
    }
}

void writeTestMainHEnd(char* contents)
{
    strcat(contents, "\n#endif\n");
}

int sizeOfTestMainC(int numTests)
{
    int size = TEST_MAIN_C_BASE_SIZE;
    size += (numTests * (TEST_MAIN_C_SIZE_INCREMENT_PER_TESTCASE + WINDOWS_MAX_PATH_LENGTH * sizeof(char*)));
    printf("SIZE of TestMain.c: %d\n", size);
    return size;
}

int sizeOfTestMainH(int numTests)
{
    int size = TEST_MAIN_C_BASE_SIZE;
    size += (numTests * (TEST_MAIN_H_SIZE_INCREMENT_PER_TESTCASE + WINDOWS_MAX_PATH_LENGTH * sizeof(char*)));
    printf("SIZE of TestMain.h: %d\n", size);
    return size;
}

void writeToFile(const char* filename, const char* contents)
{
    FILE *file;
    file = fopen(filename, "w");
    int writeResult =  fputs(contents, file);
    if(writeResult == EOF)
    {
        printf("Error writing test cases to %s\n", filename);
        exit(1);
    }
    fclose(file);
}