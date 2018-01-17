#makefile
#this makefile compiles the code test.c into
#a program called test
#which is the same as the command line:
# gcc -o test test.c
#this rule is to link the object code into an executable
test: test.o
	gcc -o test test.o
#this rule is to compile the source code
test.o: test.c
	gcc -c test.c
	gcc -c mini_filesystem.c