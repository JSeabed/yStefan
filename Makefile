#use C compiler
CC=gcc

#Include dir
CFLAGS=-I

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
	$(CC) -o $(EXE) $(OBJ) $(LIBS)


$(OBJ): $(SOURCES)
	$(CC) -c $(SOURCES) $(LIBS) 


.PHONY: clean


all:
	$(EXE)


clean:
	$(RM) $(OBJ) *~ $(EXE)
