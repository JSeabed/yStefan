CC=gcc
CFLAGS=-I
DFLAGS=-D DEBUG
LDFLAGS = -L/usr/local/lib
LDLIBS = -lgeniePi
EXE = genieInterface
SOURCES = genieInterface.c
OBJ = genieInterface.o
MAIN = genieInterface

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(LDFLAGS) $(LDLIBS)


$(OBJ): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES) $(LDFLAGS) $(LDLIBS)


all:
	$(EXE)


clean:
	$(RM) $(OBJ) *~ $(MAIN)
