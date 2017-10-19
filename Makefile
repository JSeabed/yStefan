CC=gcc
CFLAGS=-I
LIBS = lgeniePi

genieInterface: genieInterface.o
	$(CC) -o genieInterface genieInterface.o -I -lgeniePi
