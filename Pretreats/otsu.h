#ifndef OTSU_H
#define OTSU_H

Uint8* histogram(SDL_Surface* image_surface);
int* moy_grey(Uint8* hist, int size);
float variance(Uint8* hist, int size, int* data);
float variance_intra_classe(Uint8* class1, Uint8* class2, int size1, int size2);
int otsu(Uint8* hist);
SDL_Surface* black_n_white(SDL_Surface* image);

#endif
