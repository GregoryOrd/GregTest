distDirectory = ../dist
distSo = $(distDirectory)/GregTest.so
CC=gcc
CFLAGS=-fPIC
USR_LIB = /usr/lib
USR_INCLUDE = /usr/include
BUILD = ../build

GREGTEST_H = GregTest.h

G_ASSERT_C = G_ASSERT.c
G_ASSERT_H = G_ASSERT.h
G_ASSERT_O = $(BUILD)/G_ASSERT.o

G_EXPECT_CALL_C = G_EXPECT_CALL.c
G_EXPECT_CALL_H = G_EXPECT_CALL.h
G_EXPECT_CALL_O = $(BUILD)/G_EXPECT_CALL.o

PASS_FAIL_C = pass_fail.c
PASS_FAIL_H = pass_fail.h
PASS_FAIL_O = $(BUILD)/pass_fail.o

RESULT_C = result.c
RESULT_H = result.h
RESULT_O = $(BUILD)/result.o

LINKEDLIST_O = ../../../../build/GregCToolkit/Collections/LinkedList/LinkedList.o
STRINGUTILS_O = ../../../../build/GregCToolkit/String/StringUtils.o

install: $(distSo)
	sudo cp $(distSo) $(USR_LIB)
	sudo cp $(GREGTEST_H) $(USR_INCLUDE)

$(distSo): $(G_ASSERT_O) $(G_EXPECT_CALL_O) $(PASS_FAIL_O) $(RESULT_O) $(distDirectory)
	$(CC) $(CFLAGS) -shared -o $@ $(G_ASSERT_O) $(G_EXPECT_CALL_O) $(PASS_FAIL_O) $(RESULT_O) $(LINKEDLIST_O) $(STRINGUTILS_O)

$(distDirectory):
	mkdir -p $(distDirectory)

$(RESULT_O): $(RESULT_C) $(RESULT_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(PASS_FAIL_O): $(PASS_FAIL_C) $(PASS_FAIL_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(G_ASSERT_O): $(G_ASSERT_C) $(G_ASSERT_H) $(GREGTEST_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(G_EXPECT_CALL_O): $(G_EXPECT_CALL_C) $(G_EXPECT_CALL_H) $(GREGTEST_H)
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	sudo rm $(USR_LIB)/GregTest.so
	sudo rm $(USR_INCLUDE)/GregTest.h
	rm $(distSo)
	rm $(BUILD)/*.o