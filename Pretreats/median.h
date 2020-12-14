#ifndef MEDIAN_H
#define MEDIAN_H

void swapGrayscale(Uint32 grayscaleArr[], int i, int j);
Uint32 getMedian(Uint32 grayscaleArr[]);
Uint32 getNewPixel(SDL_Surface *image, int x, int y);
SDL_Surface* noise_canceled(SDL_Surface *image);

#endif
