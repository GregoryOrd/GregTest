distDirectory = ../dist

HelloWorld: testMain
	dist/TestMain.exe

testMain: $(distDirectory)

	# Run the Test Gatherer to write the test cases into the TestMain.h and TestMain.c files
	TestGatherer/TestGathererAndWriter.exe

	# Compile to .o files
	gcc -c src/HelloWorld/testHelloWorld/TestHelloWorld.c src/HelloWorld/testHelloWorld/TestHelloWorld2.c src/HelloWorld/HelloWorld.c

	# Link .o files and GregTest.dll together into a TestHelloWorld.dll, then remove .o files
	gcc -shared -o dist/TestHelloWorld.dll TestHelloWorld.o TestHelloWorld2.o HelloWorld.o -L./ dist/GregTest.dll
	rm *.o

	# Link TestMain to the dll
	gcc -o dist/TestMain TestMain.c -L./ dist/TestHelloWorld.dll dist/GregTest.dll

$(distDirectory):
	mkdir ../dist

clean:
	rm *.o dist/TestHelloWorld.dll