# CC = gcc
# CFLAGS = -g

# all: helloworld

# test:
# 	# testCases.exe

# helloworld: helloworld.o
# 	$(CC)$(LDFLAGS) -o $@ $^

# helloworld.o: helloworld.c
# 	$(CC)$(CCFLAGS) -c -o $@ $<

clean: FRC
	rm *.o *.dll *.exe

FRC:

# To Create HelloWorld.dll
# Step 1) Use "gcc -c" to compile HelloWorld.c to object code
# Step 2) Tell gcc it is building a shared library: gcc - shared -o HelloWorld.dll HelloWorld.o
# Step 3) Link to the dll with: gcc -o TestMain TestMain.c -L./ -lHelloWorld.dll


testMain:
	# Compile to .o files
	gcc -c src/HelloWorld/testHelloWorld/TestHelloWorld.c src/HelloWorld/testHelloWorld/TestHelloWorld2.c src/HelloWorld/HelloWorld.c

	# Link .o files together into a dll, then remove .o files
	gcc -shared -o TestHelloWorld.dll TestHelloWorld.o TestHelloWorld2.o HelloWorld.o
	rm *.o

	# Link TestMain to the dll
	gcc -o TestMain TestMain.c -L./ TestHelloWorld.dll