distDirectory = ../dist

GregTestMain: $(distDirectory)
	gcc GregTestMain.c TestGatherer/TestGatherer.c TestGatherer/TestMainWriter.c TestGatherer/TestDefinitions.c -o ../GregTest.exe

GregTestDll: $(distDirectory)
	# Compile to .o files
	gcc -c GregTest.c

	# Link into a dll, then remove .o files
	# Make two copies of the dll. One for this folder, one for the dist folder
	gcc -shared -o ../lib/GregTest.dll GregTest.o
	rm *.o

$(distDirectory):
	mkdir ../dist

clean:
	rm ../GregTest.exe
	rm ../lib/GregTest.dll
	rm ../lib/TestHelloWorld.dll
	rm *.o