#include <err.h>
#include "SDL/SDL.h"
#include "../Pretreats/pixel_operations.h"

int** set_matrix(SDL_Surface* image_surface, int** bin_arr)
{
    for (int i = 0; i < image_surface->h; i++)
      {
	for (int j = 0; j < image_surface->w; j++)
	  {
	    Uint32 pixel = get_pixel(image_surface, j, i);
	    Uint8 grey;
	    SDL_GetRGB(pixel, image_surface->format, &grey, &grey, &grey);

	    if (grey == 0) bin_arr[i][j] = 1;
	    else bin_arr[i][j] = 0; 
	  }
      }
    return bin_arr;
}

void set_image(int** bin_arr, SDL_Surface* image_surface)
{
    for (int i = 0; i < image_surface->h; i++)
      {
	for (int j = 0; j < image_surface->w; j++)
	  {
		  if (bin_arr[i][j] == 1) put_pixel(image_surface, j, i, SDL_MapRGB(image_surface->format, 0, 0, 0));
		  else put_pixel(image_surface, j, i, SDL_MapRGB(image_surface->format, 255, 255, 255));
	  }
      }
}

/* return 0 if the pixel with value 0 have at least c 0 next to him or 1 if not */
/* if we are analizing weigth, w_or_h = 0 for heigth w_or_h = 1 */
int is_switch(int** bin_arr, int i, int j, int size, int w_or_h, int c)
{
	/*printf("is_switch (l: %d, c: %d) %s", i, j, "(");*/
	int sum = 0;
	if ( w_or_h == 0 )
	{
		int x = j;
		while ( x < size && x < c+j && sum == 0)
		{
			sum = bin_arr[i][x];
			x++;
		}
		if (x == size || j == 0) sum = 0; 
	}	
	else
	{
		int y = i;
		while (y < size && y < c+i && sum == 0)
		{
			sum = bin_arr[y][j];
			y++;
		}
		if (i == 0 || y == size) sum = 0; 
	}
	return sum;
}	

int** rlsa_horizontal(int** bin_arr, int h, int w, int c)
{
	int swtch = -1;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (bin_arr[i][j] == 0 )
			{
				if (swtch == -1) swtch = is_switch(bin_arr, i, j, w, 0, c);
				if (swtch == 1) bin_arr[i][j] = 1;
			}
			else swtch = -1;
		}
		swtch = -1;
	}
	return bin_arr;
}


int** rlsa_vertical(int** bin_arr, int h, int w, int c)
{
	int swtch = -1;
	for (int j = 0; j < w; j++)
	{
		for (int i = 0; i < h; i++)
		{
			if (bin_arr[i][j] == 0 )
			{
				if (swtch == -1) swtch = is_switch(bin_arr, i, j, h, 1, c);
				if (swtch == 1) bin_arr[i][j] = 1;
			}
			else swtch = -1;
		}
		swtch = -1;
	}
	return bin_arr;
}

int or(int a, int b)
{
	if (a == 0 && b == 0) return 0;
	return 1;
}

int** rlsa_or(SDL_Surface* image_surface)
{
    int w = image_surface->w;
    int h = image_surface->h;


    int **mat1;
    mat1 = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat1[i] = (int*) malloc(sizeof(int) * w);

    int **mat2;
    mat2 = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat2[i] = (int*) malloc(sizeof(int) * w);

    mat2 = set_matrix(image_surface, mat1);

    mat1 = rlsa_horizontal(mat2, h, w, 15);
    mat2 = rlsa_vertical(mat2, h, w, 25);

    for (int i = 0; i < h; i++)
    {
	    for (int j = 0; j < w; j++)
	    {
	    	mat1[i][j] = or(mat1[i][j], mat2[i][j]);
	    }
    }

    return mat1;
} 

