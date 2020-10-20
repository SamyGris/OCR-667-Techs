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

	    Uint8 grey = (r + g + b)/3;
	    pixel = SDL_MapRGB(image_surface->format, grey, grey, grey);

	    put_pixel(image_surface, x, y, pixel);
	  }
      }

    return image_surface;
}
