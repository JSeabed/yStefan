#Shell
SHELL = /bin/sh
#use C compiler
CC=gcc

#Basic C flags; optimisation for RPi
CFLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard

#Macro Flag
#DFLAGS=-D DEBUG 
DEBUGFLAGS = O0 -D DEBUG

#RELEASE FLAG
RELEASEFLAGS = O3 -D NDEBUG -combine -fwhole-program

#DGENIE=-D GENIE

#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
#LIBS = -lgeniePi
GENIELIBS = -lgeniePi
DIABLOLIBS = -ldiabloSerial

#the executable file that will be created
EXE = genieInterface

#include .c files
SOURCES = $(shell echo src/*.c)

#obj file
#OBJ = genieInterface.o
OBJ = $(SOURCES:.c=.o)

#Install dir
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin


$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(RPFLAGS)  $(DEBUG) -o $(EXE) $(OBJ) $(LIBS)


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) $(RPFLAGS)  $(DEBUG) -c $(SOURCES) $(LIBS) 


.PHONY: all debug clean release
.DEFAULT: all

release: $(SOURCES)
	$(CC) $(CFLAGS) $(RELEASEFLAGS) -o $(EXE) $(SOURCES) $(DIABLOLIBS)

all:	$(EXE) 

genie: $(SOURCES)
	$(CC) $(CFLAGS) $(RELEASEFLAGS) -o $(EXE) $(SOURCES) $(GENIELIBS)


dg: genie
	genie $(DEBUGFLAGS)

#debug: all

clean:
	$(RM) $(OBJ) *~ $(EXE)


#use to choose library

#genie: all

diablo: LIBS = -ldiabloSerial -lm

#diablo: all

dg:
	debug genie all
dd:	debug diablo all
