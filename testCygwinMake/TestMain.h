#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#define DllImport __declspec(dllimport)

//For GregTest
DllImport bool result();

//From Tests Written Throughout the Repo
DllImport void testHelloWorldString();
DllImport void testMySt456uff();

#endif
