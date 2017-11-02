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

HEADERS = $(shell echo include/*.h)

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
#OBJ = genieInterface.o
OBJ = $(SOURCES:.c=.o)

#Install dir
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin


$(EXE): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS)


#$(OBJ): $(SOURCES) $(HEADERS)
	#$(CC) $(CFLAGS) -c $(SOURCES)


.PHONY: all debug clean release dg dd


#debug genie
dg: CFLAGS += $(DEBUGFLAGS)
dg: CFLAGS += $(DGENIE)
dg: LIBS += $(GENIELIBS)
dg: $(EXE) 


#debug diablo
dd: CFLAGS += $(DEBUGFLAGS)
dd: LIBS += $(DIABLOLIBS)
dd: $(EXE) 

#install bin to /usr/bin
install: release
	install -D $(EXE) $(BINDIR)/$(EXE)


#release is with diablo for now.
release: $(SOURCES)
	$(CC) $(RELEASEFLAGS) $(CFLAGS)  -o $(EXE) $(SOURCES) $(DIABLOLIBS)


uninstall:
	$(RM) $(BINDIR)/$(EXE)


clean:
	$(RM) $(OBJ) *~ $(EXE)

