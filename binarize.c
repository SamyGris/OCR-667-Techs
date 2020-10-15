#include <SDL/SDL.h>

int greyscale(int red, int green, int blue)
{
  //  Calcule une moyenne pondérée des trois couleurs
  return 0;
}

SDL_Surface greyscaleSurface(SDL_Surface image)
{
  // Applique la fonction greyscale à chaque pixel de l'image
  return image;
}

int binarize(int grey)
{
  // Détermine si le niveau de gris indiqué correspond à du blanc ou du noir
  if (grey > 100)
    return 255
    else
      return 0;
}

SDL_Surface binarizeSurface(SDL_Surface image)
{
  // Convertit l'image en niveau de gris en noir et blanc
  format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  int h = image->h;
  int w = image->w;
  for(i = 0; i < h; i++)
    {
      for(j = 0; j < w; j++)
	{
	  SDL_MapRGBA(format, (Uint8)i, 0, 0, 255)
}
