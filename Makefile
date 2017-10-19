CC=gcc
CFLAGS=-I

genieInterface: genieInterface.o
	$(CC) -o genieInterface genieInterface.o -I
