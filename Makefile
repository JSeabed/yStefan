#use C compiler
CC=gcc

#Include dir
CFLAGS=-Wall -O2

#Rpi flags
RPFLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard

#??
DFLAGS=-D DEBUG

#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
LIBS = -lgeniePi

#the executable file that will be created
EXE = genieInterface

#include .c files
SOURCES = genieInterface.c

#obj file
OBJ = genieInterface.o
#OBJ = $(SOURCES:.c=.o)


$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(RPFLAGS) $(DEBUG) -o $(EXE) $(OBJ) $(LIBS)


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) $(RPFLAGS) $(DEBUG) -c $(SOURCES) $(LIBS) 


.PHONY: all debug clean


all:	$(EXE)


debug: DEBUG = -D DEBUG

debug: all

clean:
	$(RM) $(OBJ) *~ $(EXE)
