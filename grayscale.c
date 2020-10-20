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



SDL_Surface* segment(SDL_Surface* image_surface)
{
  int moy = moyenneRGB(image_surface);
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

    return image_surface;
}



SDL_Surface* increaseChar(SDL_Surface* image_surface)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE, 28, 28, image_surface->format->BitsPerPixel, 0, 0, 0, 0);
  SDL_SoftStretch(image_surface, NULL, dest, NULL);
  return dest;
}
