CC=gcc
CFLAGS=-I
LDFLAGS = -L/usr/local/lib
LDLIBS = -lgeniePi

genieInterface: genieInterface.o
	$(CC) -o genieInterface genieInterface.o -I $(LDFLAGS) $(LDLIBS)
