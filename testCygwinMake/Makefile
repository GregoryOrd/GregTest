distDirectory = ../dist
HelloWorld: testMain

	@echo
	# Run the tests (TestMain.exe)
	dist/TestMain.exe

	@echo
	# Remove TestMain.c and TestMain.exe
	rm TestMain.h TestMain.c
	rm dist/TestMain.exe

	@echo
	# Compile the actual program now that the tests have passed
	gcc HelloWorld.o -o dist/HelloWorld.exe

	# Remove .o files
	rm *.o

	@echo
	@echo
	@echo Build Successful!

testMain: $(distDirectory)

	@echo
	# Creating TestMain.c
	TestGatherer/TestGathererAndWriter.exe

	@echo
	# Compiling test and src files to .o files
	gcc -c src/HelloWorld/testHelloWorld/TestHelloWorld.c src/HelloWorld/testHelloWorld/TestHelloWorld2.c src/HelloWorld/HelloWorld.c

	@echo
	# Link .o files and GregTest.dll together into a project dll. Then remove test .o files
	gcc -shared -o dist/TestHelloWorld.dll TestHelloWorld.o TestHelloWorld2.o HelloWorld.o -L./ dist/GregTest.dll

	@echo
	# Create TestMain.exe with the project dll and GregTest.dll
	gcc -o dist/TestMain TestMain.c -L./ dist/TestHelloWorld.dll dist/GregTest.dll

$(distDirectory):
	mkdir ../dist

clean:
	rm *.o dist/TestHelloWorld.dll dist/HelloWorld.exe