# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: display

display: rlsa.o otsu.o median.o pixel_operations.o grayscale.o display.o

display.o: rlsa.h otsu.h median.h grayscale.h pixel_operations.h

clean:	${RM} *.o
	${RM} *.d
	${RM} *~

# END
