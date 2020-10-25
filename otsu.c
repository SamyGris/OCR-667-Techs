#include <err.h>
#include "SDL/SDL.h"
#include "pixel_operations.h"

Uint8* histogram(SDL_Surface* image_surface)
{
    static Uint8 hist[256];

  
    for (int i = 0; i < image_surface->w; i++)
      {
	for (int j = 0; j < image_surface->h; j++)
	  {
	    Uint32 pixel = get_pixel(image_surface, i, j);
	    Uint8 grey;
	    SDL_GetRGB(pixel, image_surface->format, &grey, &grey, &grey);
	    hist[grey] += 1;
	  }
      }

    return hist;
}

int* moy_grey(Uint8* hist, int size)
{
    int coef = 0;
    static int data[2] = {0,0};
    for (int i = 0; i < size; i++)
    {
	    coef+=i;
	    data[0] += hist[i];
	    data[1] += i*hist[i];
    }
    if (size > 1)
    {
	    int tot = data[1];
	    data[1] = tot/coef;
    }
    return data;
}

float variance(Uint8* hist, int size, int* data)
{
    float moyen = data[1];
    float var = 0;
    for (int i = 0; i < size; i++) var += hist[i]*(i-moyen)*(i-moyen);
    return var/data[0];
}

float variance_intra_classe(Uint8* class1, Uint8* class2, int size1)
{

    int* data1 = moy_grey(class1, size1+1);
    int* data2 = moy_grey(class2, 256-size1);
    int nb_pixel = data1[0]+data2[0];
    float vw = ((float)data1[0]/(float)nb_pixel)*variance(class1, size1+1, data1)+((float)data2[0]/(float)nb_pixel)*variance(class2, 256-size1, data2);

    return vw;
}

int otsu(Uint8* hist)
{
    float vw = -1;
    int vw_t = 0;
    for (int t = 0; t<255; t++)
    {
        Uint8 hist2[255-t];
	Uint8 hist1[t+1];
    	for (int i = 0; i < 256; i++) 
	{
		if (i <= t) hist1[i] = hist[i];
		else hist2[i-t-1] = hist[i];	
	}
	float vw_tmp = variance_intra_classe(hist1, hist2, t);
	if (vw_tmp < vw || vw < 0)
	{
		vw = vw_tmp;
		vw_t = t;
	}
    }
    return vw_t;
}

SDL_Surface* black_n_white(SDL_Surface* image)
{
    int seuil = otsu(histogram(image));
    for (int i = 0; i < image->w; i++)
      {
	for (int j = 0; j < image->h; j++)
	  {
	    Uint32 pixel = get_pixel(image, i, j);
	    Uint8 grey;
	    SDL_GetRGB(pixel, image->format, &grey, &grey, &grey);
	  
	    if (grey > seuil) grey = 255;
	    else grey = 0;
	    pixel = SDL_MapRGB(image->format, grey, grey, grey);

	    put_pixel(image, i, j, pixel);
	  }
      } 
    return image;
}
