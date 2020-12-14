#ifndef RLSA_H 
#define RLSA_H

int** set_matrix(SDL_Surface* image_surface, int** bin_arr);
void set_image(int** bin_arr, SDL_Surface* image_surface);
int is_switch(int** bin_arr, int i, int j, int size, int w_or_h, int c);
int** rlsa_horizontal(int** bin_arr, int h, int w, int c);
int** rlsa_vertical(int** bin_arr, int h, int w, int c);
int or(int a, int b);
int** rlsa_or(SDL_Surface* image_surface);

#endif
