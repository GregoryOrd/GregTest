distDirectory = ../dist
distSo = $(distDirectory)/GregTest.so
CC=gcc
CFLAGS=-fPIC
USR_LIB = /usr/lib
USR_INCLUDE = /usr/include
GREGTEST_C = GregTest.c
GREGTEST_O = GregTest.o
GREGTEST_H = GregTest.h
GREGTEST_H_PRIVATE = GregTest_Private.h

install: $(distSo)
	sudo cp $(distSo) $(USR_LIB)
	sudo cp $(GREGTEST_H) $(USR_INCLUDE)

$(distSo): $(GREGTEST_O) $(distDirectory) 
	# Link into a so, then remove .o files
	$(CC) $(CFLAGS) -shared -o $@ $<
	rm *.o

$(distDirectory):
	mkdir -p $(distDirectory)

$(GREGTEST_O): $(GREGTEST_C) $(GREGTEST_H) $(GREGTEST_H_PRIVATE)
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm $(USR_LIB)/GregTest.so
	rm $(USR_INCLUDE)/GregTest.h
	rm $(distSo)
	rm *.o