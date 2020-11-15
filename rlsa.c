#include <err.h>
#include "SDL/SDL.h"
#include "pixel_operations.h"

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

SDL_Surface* set_image(int** bin_arr, SDL_Surface* image_surface, int h, int w)
{
    for (int i = 0; i < h; i++)
      {
	for (int j = 0; j < w; j++)
	  {
		  if (bin_arr[i][j] == 0) put_pixel(image_surface, i, j, 0);
		  else put_pixel(image_surface, i, j, 255);
	  }
      }
    return image_surface;
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
			/*printf("%d",(char)bin_arr[i][x]);i*/
			x++;
		}
		if (c+j >= size || j == 0) sum = 0; 
	}	
	else
	{
		int y = i;
		while (y < size && y < c+i && sum == 0)
		{
			sum = bin_arr[y][j];
			/*printf("%d",(char)bin_arr[y][j]);*/
			y++;
		}
		if (i+c >= size || i == 0) sum = 0; 
	}
	if (sum > 0) sum = 1;
	/*printf(") : %d\n", sum);*/
	return sum;
}	

int** rlsa_horizontal(int** bin_arr, int h, int w, int c)
{
	int swtch = -1;
	for (int i = 0; i < h; i++)
	{
		/*printf("Line %d :\n",i);*/
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
		/*printf("%s","\n");*/
	}
	return bin_arr;
}


int** rlsa_vertical(int** bin_arr, int h, int w, int c)
{
	int swtch = -1;
	for (int j = 0; j < w; j++)
	{
		/*printf("Column %d :\n",j);*/
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
