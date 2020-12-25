#include "TestGatherer.h"

#include <ctype.h>
#include "TestMainWriter.h"

int main(void) 
{ 
    #if defined(_WIN32) || defined(_WIN64)
        printf("TestGatherer does not support being built on Windows.\n");
        printf("If you have a Windows machine, please use Cygwin.\n");
        exit(1);
    #endif
    
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = ".";

    TestCaseList* testCases = (TestCaseList*)malloc(sizeof(TestCaseList));
    initTestCases(testCases);

    loadTests(testCases, startingDirectory);
    writeTestsToTestMain(testCases);

    freeTestCasesList(testCases);
    exit(0);
} 

void initTestCases(TestCaseList* testCases)
{
    testCases->size = 0;
    testCases->cases = (TestCase*)malloc(sizeof(TestCase));
    testCases->cases[0].testFile = NULL;
    testCases->cases[0].testName = NULL; 
}

void loadTests(TestCaseList* testCases, char* basePath)
{
    char* fileOrSubDirectoryFullPath = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    struct dirent *fileOrSubDirectory;

    DIR *basePathDirectory = opendir(basePath);
    if (!basePathDirectory)
    {
        return;
    }

    while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL)
    {
        copyFileOrSubDirectoryNameIntoPath(fileOrSubDirectoryFullPath, basePath, fileOrSubDirectory->d_name);
        addTestCasesOrEnterSubDirectoryForRecursion(testCases, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
    }

    closedir(basePathDirectory);
    free(fileOrSubDirectoryFullPath);
}

void addTestCasesOrEnterSubDirectoryForRecursion(TestCaseList* testCases, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath)
{
    if(isTestDir(basePath) && isTestFile(fileOrSubDirectory))
    {
        addTestCasesToList(testCases, fileOrSubDirectoryFullPath);
    }
    if (isDirectory(fileOrSubDirectory))
    {
        loadTests(testCases, fileOrSubDirectoryFullPath);
    }
}

bool isDirectory(struct dirent *fileOrSubDirectory)
{
    return strncmp(fileOrSubDirectory->d_name, ".", 1) != 0 && fileOrSubDirectory->d_type == DT_DIR;
}

void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName)
{
    strcpy(path, basePath);
    strcat(path, "/");
    strcat(path, fileOrSubDirectoryName);  
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

void addTestCasesToList(TestCaseList* list, const char* pathToTestFile)
{
    FILE *testFile;
    char* buffer = (char*)malloc(255*sizeof(char));
    testFile = fopen(pathToTestFile, "r");

    while(fgets(buffer, 255, (FILE*)testFile) != NULL)
    {
        if(isTestCaseDefinition(buffer))
        {
            addSingleTestCaseToList(list, pathToTestFile, buffer);
        }  
    }

    free(buffer);
    fclose(testFile);
}

void addSingleTestCaseToList(TestCaseList* list, const char* pathToTestFile, char* buffer)
{
    list->cases = (TestCase*)realloc(list->cases, ((list->size + 1) * sizeof(TestCase)));
    list->cases[list->size].testName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*));
    list->cases[list->size].testFile = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char*)); 

    trimTestName(buffer); 

    strcpy(list->cases[list->size].testName, buffer);
    strcpy(list->cases[list->size].testFile, pathToTestFile);
    list->size++;
}

bool isTestDir(char* dirName)
{
    char lower[WINDOWS_MAX_PATH_LENGTH];
    lowerString(lower, dirName);
    return strstr(lower, "/test") != NULL;
}

bool isTestFile(struct dirent *fileOrSubDirectory)
{
    char lower[WINDOWS_MAX_PATH_LENGTH];
    lowerString(lower, fileOrSubDirectory->d_name);
    bool result = (strncmp(lower, "test", 4) == 0 && (strstr(lower, ".c") != NULL || strstr(lower, ".cpp") != NULL));
    return result;
}

void lowerString(char* dest, char* src)
{
    char* srcItr = src;
    char* destItr = dest;
    while(*srcItr != '\0')
    {
        *destItr = tolower(*srcItr);
        destItr++;
        srcItr++;
    }
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
    int length = strlen(testName) - 1;
    int endOffset = testNameEndOffset(testName);
    for(int i = 0; i < strlen(testName) - 1; i++)
    {
        if(i < length - endOffset )
        {
            testName[i] = testName[i + TEST_NAME_TRIM_FRONT_OFFSET];
        }
        else{
            testName[i] = '\0';
        }
    }
}

int testNameEndOffset(char* testName)
{
    if(theCurlyBraceIsOnTheSameLineAsTheTestName(testName, strlen(testName) - 1))
    {
        return TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_SAME_LINE;
    }
    return TEST_NAME_TRIM_BACK_OFFSET_CURLY_BRACE_NEXT_LINE;
}

bool theCurlyBraceIsOnTheSameLineAsTheTestName(char* testName, int initialLength)
{
    return testName[initialLength-2] == '{';
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