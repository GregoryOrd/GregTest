distDirectory = ../dist

GregTest: $(distDirectory)
	# Compile to .o files
	gcc -c GregTest.c

	# Link into a dll, then remove .o files
	# Make two copies of the dll. One for this folder, one for the dist folder
	gcc -shared -o GregTest.dll GregTest.o
	gcc -shared -o ../dist/GregTest.dll GregTest.o
	rm *.o

$(distDirectory):
	mkdir ../dist

clean:
	rm GregTest.dll
	rm ../dist/GregTest.dll