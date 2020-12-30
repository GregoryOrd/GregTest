distDirectory = ../dist
HelloWorld:
	dist/GregTest.exe


	@echo
	# Compile the actual program now that the tests have passed
	gcc HelloWorld.o -o dist/HelloWorld.exe

	# Remove .o files
	rm *.o

	@echo
	@echo
	@echo Build Successful!

$(distDirectory):
	mkdir ../dist

clean:
	rm *.o dist/TestHelloWorld.dll dist/HelloWorld.exe dist/TestMain.exe HelloWorld.exe