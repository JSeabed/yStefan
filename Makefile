#use C compiler
CC=gcc

#uncomment below to show all warinings
#CFLAGS=-Wall -Werror -O3
CFLAGS=-O3

#Rpi flags
RPFLAGS=-march=armv6 -mfpu=vfp -mfloat-abi=hard

#Macro Flag
DFLAGS=-D DEBUG -D GENIE
#DGENIE=-D GENIE

#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
#LIBS = -lgeniePi
ifeq ($(DFLAGS), -D GENIE)
	LIBS = -lgeniePi 
else
	LIBS = -ldiabloSerial -lm
endif

#the executable file that will be created
EXE = genieInterface

#include .c files
SOURCES = genieInterface.c

#obj file
OBJ = genieInterface.o
#OBJ = $(SOURCES:.c=.o)


$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(RPFLAGS) $(GENIE) $(DEBUG) -o $(EXE) $(OBJ) $(LIBS)


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) $(RPFLAGS) $(GENIE) $(DEBUG) -c $(SOURCES) $(LIBS) 


.PHONY: all debug clean


all:	$(EXE) 

debug: DEBUG = -D DEBUG

debug: all

clean:
	$(RM) $(OBJ) *~ $(EXE)

.PHONY: genie

#use make genie to use the genie library
genie: GENIE = -D GENIE LIBS=lgeniePi

genie: all

