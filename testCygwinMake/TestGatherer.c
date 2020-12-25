#include "TestGatherer.h"

#include <ctype.h>
#include "TestMainWriter.h"

int main(void) 
{     
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
    LineMetrics* metrics = (LineMetrics*)malloc(sizeof(LineMetrics));
    LineAnalysisResults* results = (LineAnalysisResults*)malloc(sizeof(LineAnalysisResults));
    initLineMetrics(metrics);
    initLineAnalysisResults(results);
    
    gatherLineMetrics(metrics, line);
    analyzeLineMetrics(metrics, line);
    determineResults(results, metrics, line);

    bool isTestCase = results->correctStartOfLine && results->correctSpaces && results->correctBrackets && !results->hasSpecialCharacters;

    free(metrics);
    free(results);
    return isTestCase;
}

void gatherLineMetrics(LineMetrics* metrics, char* line)
{
    char* currentPtr = line;
    while(*currentPtr != '\0' && *currentPtr != '\n')
    {
        if(*currentPtr == ' ')
        {
            metrics->numSpaces++;
        }
        else if(*currentPtr == '(')
        {
            metrics->numLeftBrackets++;
            metrics->leftBracketIndex = metrics->length;
        }
        else if(*currentPtr == ')')
        {
            metrics->numRightBrackets++;
            metrics->rightBracketIndex = metrics->length;
        }
        metrics->length++;
        currentPtr++;
    }
}

void analyzeLineMetrics(LineMetrics* metrics, char* line)
{
    //Line will have form:
    //testSomething() or testSomthing(){
    metrics->expectedLeftBracketIndex = metrics->length - 3;
    if(theCurlyBraceIsOnTheSameLineAsTheTestName(line, metrics->length))
    {
        metrics->expectedLeftBracketIndex--;
    }
    metrics->expectedRightBracketIndex = metrics->expectedLeftBracketIndex + 1; 
}

void determineResults(LineAnalysisResults* results, LineMetrics* metrics, char* line)
{
    results->correctStartOfLine = strncmp(line, "void test", 9) == 0;
    results->singleSpaceBetweenBoolAndTestName = (strstr(line, " ") == &line[4]);
    results->correctSpaces = results->singleSpaceBetweenBoolAndTestName && metrics->numSpaces == 1;

    results->correctBracketCount = metrics->numLeftBrackets == 1 && metrics->numRightBrackets == 1; 
    results->correctBracketPosition = metrics->leftBracketIndex == metrics->expectedLeftBracketIndex && metrics->rightBracketIndex == metrics->expectedRightBracketIndex;
    results->correctBrackets = results->correctBracketCount && results->correctBracketPosition;

    if(results->correctStartOfLine && results->correctSpaces && results->correctBrackets)
    {
        results->hasSpecialCharacters = lineHasSpecialCharacters(metrics, line);
    }
}

bool lineHasSpecialCharacters(LineMetrics* metrics, char* line)
{
    for(int i = 0; i < metrics->length; i++)
    {
        if(isSpecialCharacter(line[i]))
        {
            printf("\nGregTest does not accept test cases with special characters in the name of the test.\n");
            printf("%s\n\n", line);
            return true;
        }
    }

    return false;
}

void initLineMetrics(LineMetrics* metrics)
{
    metrics->numSpaces = 0;
    metrics->numLeftBrackets = 0;
    metrics->numRightBrackets = 0;
    metrics->leftBracketIndex = 0;
    metrics->rightBracketIndex = 0; 
    metrics->length = 0;
    metrics->expectedLeftBracketIndex = 0;
    metrics->expectedRightBracketIndex = 0;
}

void initLineAnalysisResults(LineAnalysisResults* results)
{
    results->correctStartOfLine = false;
    results->singleSpaceBetweenBoolAndTestName = false;
    results->hasSpecialCharacters = false;
    results->correctSpaces = false;
    results->correctBracketCount = false;
    results->correctBracketPosition = false;
    results->correctBrackets = false;
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