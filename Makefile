CC=gcc
CFLAGS=-I
DFLAGS=-D DEBUG
#LDFLAGS = -L/usr/local/lib
#LDLIBS = -lgeniePi
LIBS = -lgeniePi
EXE = genieInterface
SOURCES = genieInterface.c
OBJ = $(SOURCES:.c=.o)
#MAIN = genieInterface

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(DFLAGS) -o $(EXE) $(OBJ) -lgeniePi


#$(OBJ): $(SOURCES)
	#$(CC) $(CFLAGS) -c $(SOURCES) $(LDFLAGS) $(LDLIBS)


.PHONY: clean


all:
	$(EXE)


clean:
	$(RM) $(OBJ) *~ $(EXE)
