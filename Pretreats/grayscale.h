#ifndef GRAYSCALE_H
#define GRAYSCALE_H

SDL_Surface* grayscale(SDL_Surface* image_surface);
int moyenneRGB(SDL_Surface* image_surface);
SDL_Surface* segment(SDL_Surface* image_surface);
SDL_Surface* increaseChar(SDL_Surface* image_surface);

#endif
