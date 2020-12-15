	#OCR Makefile
CC=gcc



CPPFLAGS=   `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra  -std=c99 -O3
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
LDLIBS= `pkg-config --libs sdl` `pkg-config --libs gtk+-3.0` -lSDL_image -lm 


SRC= test_glade.c ../Pretreats/median.c ../Pretreats/grayscale.c ../Pretreats/pixel_operations.c ../Pretreats/otsu.c ../Segmentation/rlsa.c 
OBJ=${SRC:.c=.o}
DEP=${SRC:.c=.d}

all: test_glade 

test_glade:${OBJ}


pp: test_glade.o ../Pretreats/median.o ../Pretreats/grayscale.o ../Pretreats/pixel_operations.o ../Pretreats/otsu.o ../Segmentation/rlsa.o



clean:
	${RM} ${OBJ} #remove object files
	${RM} ${DEP} #remove depedency files
	${RM} main   #remove main program




-include ${DEP}
#END

# This makefile is use to test the preprocessing step of our program.
# 

