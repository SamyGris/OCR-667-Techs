#ifndef BINARIZE_H
#define BINARIZE_H

int greyscale(int red, int green, int blue);

SDL_Surface* greyscaleSurface(SDL_Surface *image);

int median(SDL_Surface *image, int x, int y);

SDL_Surface* noiseCancelling(SDL_image *image);

int binarize(int grey);

SDL_Surface* binarizeSurface(SDL_Surface *image);

#endif
