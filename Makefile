#Shell
SHELL = /bin/sh
#use C compiler
CC=gcc

#Basic C flags; optimisation for RPi
CFLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard

#Macro Flag
#DFLAGS=-D DEBUG 
DEBUGFLAGS = -O -D DEBUG

#RELEASE FLAG
RELEASEFLAGS = -O3 -D NDEBUG -fwhole-program

DGENIE=-D GENIE

#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
#LIBS = -lgeniePi
GENIELIBS = -lgeniePi
DIABLOLIBS = -ldiabloSerial
LIBS = 

#the executable file that will be created
EXE = genieInterface

#include .c files
SOURCES = $(shell echo src/*.c)

#obj file
OBJ = genieInterface.o
#OBJ = $(SOURCES:.c=.o)

#Install dir
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin


$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS)


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)


.PHONY: all debug clean release
.DEFAULT: all

release: $(SOURCES)
	$(CC) $(RELEASEFLAGS) $(CFLAGS)  -o $(EXE) $(SOURCES) $(DIABLOLIBS)

all:	$(EXE) 

genie: $(SOURCES)
	$(CC) $(CFLAGS) $(RELEASEFLAGS) -o $(EXE) $(SOURCES) $(GENIELIBS)


dg: CFLAGS += $(DEBUGFLAGS)
dg: CFLAGS += $(DGENIE)
dg: LIBS += $(GENIELIBS)
dg: all 


#debug diablo
dd: CFLAGS += $(DEBUGFLAGS)
dd: LIBS += $(DIABLOLIBS)
dd: all 

#debug: all

clean:
	$(RM) $(OBJ) *~ $(EXE)


#use to choose library

#genie: all

diablo: LIBS = -ldiabloSerial -lm

