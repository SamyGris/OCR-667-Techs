# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: test_glade

test_glade: ../Segmentation/rlsa.o ../Pretreats/otsu.o ../Pretreats/median.o ../Pretreats/pixel_operations.o ../Pretreats/grayscale.o test_glade.o 

test_glade.o: ../Segmentation/rlsa.h ../Pretreats/otsu.h ../Pretreats/median.h ../Pretreats/grayscale.h ../Pretreats/pixel_operations.h

clean:	-rm *.o
	-rm *.d
	-rm *~

# END
