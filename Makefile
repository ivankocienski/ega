
CC=g++
CFLAGS=-Wall -g # -std=c++11
LFLAGS=-g
LIBS=-lSDL -lSDL_image

EXE=ega-demo
SOURCES= \
	main.cc \
	screen.cc \
	tile_set.cc \
	font.cc

OBJECTS= $(patsubst %.cc,%.o,$(SOURCES)) 
HEADERS= $(patsubst %.cc,%.hh,$(SOURCES)) 

all: $(OBJECTS)
	$(CC) -o $(EXE) $(LFLAGS) $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f sdl/*.o

tags: FORCE
	ctags *.cc *.hh

FORCE:
