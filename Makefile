CC=gcc
CFLAGS=-I
DFLAGS=-D DEBUG
#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
LIBS = -lgeniePi
EXE = genieInterface
SOURCES = genieInterface.c
#OBJ = $(SOURCES:.c=.o)
OBJ = genieInterface.o
#MAIN = genieInterface

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lgeniePi


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES) -lgeniePi 


.PHONY: clean


all:
	$(EXE)


clean:
	$(RM) $(OBJ) *~ $(EXE)
