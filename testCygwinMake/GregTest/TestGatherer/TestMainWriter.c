#include "TestMainWriter.h"

#include "../GregTestConstants.h"

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
    char testMainC[WINDOWS_MAX_PATH_LENGTH] = TEMP_DIR;
    strcat(testMainC, "/TestMain.c");
    writeToFile(testMainC, contents);
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
    strcat(main, "\t\tprintf(\"All Tests Passed Successfully\\n\");\n");
    strcat(main, "\t\texit(0);\n");
    strcat(main, "\t}\n");
    strcat(main, "\telse\n");
    strcat(main, "\t{\n");
    strcat(main, "\t\tprintf(\"Tests Failed\\n\");\n");
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
    char testMainH[WINDOWS_MAX_PATH_LENGTH] = TEMP_DIR;
    strcat(testMainH, "/TestMain.h");
    writeToFile(testMainH, contents);
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
    return size;
}

int sizeOfTestMainH(int numTests)
{
    int size = TEST_MAIN_C_BASE_SIZE;
    size += (numTests * (TEST_MAIN_H_SIZE_INCREMENT_PER_TESTCASE + WINDOWS_MAX_PATH_LENGTH * sizeof(char*)));
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