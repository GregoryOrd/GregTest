distDirectory = ../dist
distSo = $(distDirectory)/GregTest.so
CC=gcc
CFLAGS=-fPIC
USR_LIB = /usr/lib
USR_INCLUDE = /usr/include

GREGTEST_H = GregTest.h

G_ASSERT_C = G_ASSERT.c
G_ASSERT_H = G_ASSERT.h
G_ASSERT_O = G_ASSERT.o

PASS_FAIL_C = pass_fail.c
PASS_FAIL_H = pass_fail.h
PASS_FAIL_O = pass_fail.o

RESULT_C = result.c
RESULT_H = result.h
RESULT_O = result.o

install: $(distSo)
	sudo cp $(distSo) $(USR_LIB)
	sudo cp $(GREGTEST_H) $(USR_INCLUDE)

$(distSo): $(G_ASSERT_O) $(PASS_FAIL_O) $(RESULT_O) $(distDirectory) 
	# Link into a so, then remove .o files
	$(CC) $(CFLAGS) -shared -o $@ $(G_ASSERT_O) $(PASS_FAIL_O) $(RESULT_O)
	rm *.o

$(distDirectory):
	mkdir -p $(distDirectory)

$(RESULT_O): $(RESULT_C) $(RESULT_H)
	$(CC) $(CFLAGS) -c $<

$(PASS_FAIL_O): $(PASS_FAIL_C) $(PASS_FAIL_H)
	$(CC) $(CFLAGS) -c $<

$(G_ASSERT_O): $(G_ASSERT_C) $(G_ASSERT_H) $(GREGTEST_H)
	$(CC) $(CFLAGS) -c $<
	
clean:
	sudo rm $(USR_LIB)/GregTest.so
	sudo rm $(USR_INCLUDE)/GregTest.h
	rm $(distSo)
	rm *.o