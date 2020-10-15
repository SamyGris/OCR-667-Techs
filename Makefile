# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2
LDFLAGS =
LDLIBS = `sdl-config --cflags --libs` -lSDL_image

SRC = main.c binarize.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

run: all
	./main

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program

-include ${DEP}
