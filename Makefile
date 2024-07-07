
CC = clang
CFLAGS = -Wall -Wextra -Wno-strict-prototypes -pedantic 
INCLUDES = -I include/ 
LIBS = -L lib/ -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
TESTDIR = tests
TESTSRC = $(wildcard $(TESTDIR)/*.c)

all:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(SRC) -o ./bin/space-invaders
