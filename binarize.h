#ifndef BINARIZE_H
#define BINARIZE_H

int greyscale(int red, int green, int blue);

SDL_Surface greyscaleSurface(SDL_Surface image);

int binarize(int grey);

SDL_Surface binarizeSurface(SDL_Surface image);

#endif
