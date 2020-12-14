#include <err.h>
#include "SDL/SDL.h"
#include "pixel_operations.h"

SDL_Surface* grayscale(SDL_Surface* image_surface)
{
    for (int x = 0; x < image_surface->w; x++)
      {
        for (int y = 0; y < image_surface->h; y++)
          {
            Uint32 pixel = get_pixel(image_surface, x, y);

            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            Uint8 grey = r*0.299 + g*0.587 + b*0.114;
            pixel = SDL_MapRGB(image_surface->format, grey, grey, grey);

            put_pixel(image_surface, x, y, pixel);
          }
      }

    return image_surface;
}



unsigned long *get_histogram(SDL_Surface* image_surface, unsigned long *array)
{
  unsigned long x = image_surface->h;
  unsigned long y = image_surface->w;
  for(unsigned long i = 0; i < x; i++)
    {
      for(unsigned long j = 0; j < y; j++)
	{
          Uint32 pixel = get_pixel(image_surface, x, y);
          Uint8 grey;
          SDL_GetRGB(pixel, image_surface->format, &grey, &grey, &grey);

          array[grey] += 1;
	}
    }
  return array;
}



/*Uint8 otsu(unsigned long *histogram, int total)
{
    unsigned long sum = 0, wf = 0, sumb = 0;
    unsigned long mb, mf;
    float between = 0, max = 0;
    Uint8 threshold1 = 0, threshold2 =0;
    for(int i = 0; i < 256; i++)
        sum += i * histogram[i];
    unsigned long wb = 0;
    for(int i = 0; i < 256; i++)
    {
        wb += histogram[i];
        if(wb == 0)
            continue;
        wf = total - wb;
        if( wf == 0)
            break;
        sumb += i * histogram[i];
        mb = sumb / wb;
        mf = (sum - sumb) / wf;
        between = wb * wf * (mb - mf) * (mb - mf);
        if(between >= max)
        {
            threshold1 = i;
            if(between > max)
                threshold2 = i;
            max = between;
        }
    }
    Uint8 threshold = ((threshold1 + threshold2) / 2);
    return threshold;
}


*/
int moyenneRGB(SDL_Surface* image_surface)
{
  Uint8 r, g, b;
  int moyPixel = 0;

  int w = image_surface->w;
  int h = image_surface->h;
  int nbPixel = w * h;
  for(int x = 0; x < w; x++)
    {
      for(int y = 0; y < h; y++)
        {
          Uint32 pixel = get_pixel(image_surface, x, y);
          SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
          moyPixel += r;
        }
    }
  moyPixel = moyPixel / nbPixel;
  return(moyPixel);
}



/*SDL_Surface* segment(SDL_Surface* image_surface)
{
  unsigned long *hist = malloc(256 * sizeof(unsigned long));
  for(int i = 0; i < 256; i++)
        hist[i] = 0;
  int moy = otsu(get_histogram(image_surface, hist), (image_surface->w)*(image_surface->h));
    for (int x = 0; x < image_surface->w; x++)
      {
	for (int y = 0; y < image_surface->h; y++)
	  {
	    Uint32 pixel = get_pixel(image_surface, x, y);

	    Uint8 grey;
	    SDL_GetRGB(pixel, image_surface->format, &grey, &grey, &grey);

	    if (grey > moy)
	      grey = 255;
	    else
	      grey = 0;
	    
	    pixel = SDL_MapRGB(image_surface->format, grey, grey, grey);

	    put_pixel(image_surface, x, y, pixel);
	  }
      }

    free(hist);

    return image_surface;
}*/



SDL_Surface* increaseChar(SDL_Surface* image_surface)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE, 28, 28, image_surface->format->BitsPerPixel, 0, 0, 0, 0);
  SDL_SoftStretch(image_surface, NULL, dest, NULL);
  return dest;
}
