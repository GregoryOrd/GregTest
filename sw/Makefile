distDirectory = ../dist
CC=gcc
CFLAGS=-fPIC

GregTestDll: $(distDirectory)
	# Compile to .o files
	$(CC) $(CFLAGS) -c GregTest.c

	# Link into a so, then remove .o files
	$(CC) $(CFLAGS) -shared -o ../dist/GregTest.so GregTest.o
	rm *.o

$(distDirectory):
	mkdir ../dist
	
clean:
	rm ../dist/GregTest.so