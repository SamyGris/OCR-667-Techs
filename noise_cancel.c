#include <SDL/SDL.h>
#include "pixel_operations.h"

void swapGrayscale(Uint32 grayscaleArr[], int i, int j)
{
        Uint32 tmp = grayscaleArr[i];
        grayscaleArr[i] = grayscaleArr[j];
        grayscaleArr[j] = tmp;
}

Uint32 getMedian(Uint32 grayscaleArr[])
{
        for (int i = 0; i < 8; i++)
        {
                Uint32 min = i;
                for (int j = i+1; j < 9; j++) if (grayscaleArr[j] < grayscaleArr[min]) min = j;
                swapGrayscale(grayscaleArr, i, min);
        }

        return grayscaleArr[4];
}


Uint32 getNewPixel(SDL_Surface *image, int x, int y)
{
        int iArr = 0;
        Uint32 grayscaleArr[9];
        for (int i = -1; i < 2; i++)
        {
                for (int j = -1; j < 2; i++)
                {
                        grayscaleArr[iArr] = get_pixel(image, x+i, y+i);
                        iArr++;
                }
        }

        return getMedian(grayscaleArr);
}

SDL_Surface* noise_canceled(SDL_Surface *image)
{
	int w;
	int h;
	w = image -> w;
	h = image -> h;
	for (int i = 1; i < w-1; i++)
	{
		for (int j = 1; j < h-1; j++) put_pixel(image, i, j, getNewPixel(image, i, j));
	}
	return image;
}

