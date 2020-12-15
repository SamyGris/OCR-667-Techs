#OCR Makefile
CC=gcc
CPPFLAGS=   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra  -std=c99 -O3
LDFLAGS= $(PTHREAD) $(GTKLIB) -export-dynamic
LDLIBS= `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lSDL_image -lm 


SRC= gui.c ../Pretreats/median.c ../Pretreats/grayscale.c ../Pretreats/pixel_operations.c ../Pretreats/otsu.c ../Segmentation/rlsa.c ../Segmentation/xycut.c ../Segmentation/matrices.c
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all: gui

gui:${OBJ}


pp: gui.o ../Pretreats/median.o ../Pretreats/grayscale.o ../Pretreats/pixel_operations.o ../Pretreats/otsu.o ../Segmentation/rlsa.o ../Segmentation/xycut.o ../Segmentation/matrices.o



clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} gui    #remove main program
	${RM} -r cache




-include ${DEP}
#END

# This makefile is use to test the preprocessing step of our program.
# 

