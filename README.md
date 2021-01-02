# GregTest
GregTest will ship as a dll library and a header file. The header file can be included in a customer's test files to allow them
to link with the dll. The dll consists of a series of assertion functions, as well as a 'bool result()' function that returns true if all tests passed and false if any tests failed. Actually compiling and running the tests is the responsibility of the build system, but shipping as a dll and header should give some flexibility here. See the [GregBuild repo](https://github.com/GregoryOrd/GregBuild) for an example build system process that uses the GregTest dll.

## Adding dirent.h to MSVC include path
In this project dirent.h is used to recurse through the file system. At the time of writing the functions for this recursion I was developing with VS Code and compiling with gcc and Cygwin. By default the VS CODE and the MSVC compiler do not include dirent.h, but Cygwin does. This meant that I would get a file not found error inside of my IDE, but would not have the problem when compiling. To remove this error inside the IDE, I added dirent.h to the MSVC include path. In my case, the include path was

- C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/include

The dirent.h file that I placed here came from a github repo owned by Toni Ronkko:
- [https://github.com/tronkko/dirent/blob/master/include/dirent.h](https://github.com/tronkko/dirent/blob/master/include/dirent.h)
