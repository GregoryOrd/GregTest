distDirectory = ../dist

GregTestMain: $(distDirectory)
	gcc GregTestMain.c -o ../dist/GregTest.exe

GregTestDll: $(distDirectory)
	# Compile to .o files
	gcc -c GregTest.c

	# Link into a dll, then remove .o files
	# Make two copies of the dll. One for this folder, one for the dist folder
	gcc -shared -o ../dist/GregTest.dll GregTest.o
	rm *.o

$(distDirectory):
	mkdir ../dist

clean:
	rm ../dist/GregTest.exe
	rm ../dist/GregTest.dll
	rm ../dist/TestHelloWorld.dll
	rm *.o