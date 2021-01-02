GregTestDll: $(distDirectory)
	# Compile to .o files
	gcc -c GregTest.c

	# Link into a dll, then remove .o files
	gcc -shared -o ../dist/GregTest.dll GregTest.o
	rm *.o
	
clean:
	rm ../dist/GregTest.dll