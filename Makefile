VERSION ?= 0.1.0
PREVIOUS_TAG = 0.1.0
BUILD_NUMBER := $(shell git rev-list --all --count)
BUILDDATE := $(shell date '+%Y%m%d')

CXX			= g++ -std=gnu++17
INCLUDE	= -Iinclude

CFLAGS  = -ggdb -O2 -Wall  $(INCLUDE) `ppl7-config --cflags`
LIBS    = -L/usr/local/lib  `ppl7-config --libs`  -lstdc++

LIBDEP  := $(shell ppl7-config --ppllib release)

PROGRAM	= pwcrack

OBJECTS=compile/main.o

$(PROGRAM): $(OBJECTS) $(LIBDEP)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(LIBS)
	-chmod 755 $(PROGRAM)

clean:
	-rm -f compile/*.o $(PROGRAM) *.core


compile/main.o: main.cpp Makefile
	- @mkdir -p compile
	$(CXX) -o compile/main.o -c main.cpp $(CFLAGS)

