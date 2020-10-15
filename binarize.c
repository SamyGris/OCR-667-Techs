#include <SDL/SDL.h>

int greyscale(int red, int green, int blue)
{
  //  Calcule une moyenne pondérée des trois couleurs
  return ((red + green + blue)/3);
}

SDL_Surface greyscaleSurface(SDL_Surface image)
{
  // Applique la fonction greyscale à chaque pixel de l'image
  w = image->w;
  h = image->h;
  for (int y = 0; i=h; i++)
    {
      for (int x = 0; i=w; i++)
	{
	  pixel = SDL_GetRGB(GetPixel(image, x, y));
	  
	  
  return image;
}

int binarize(int grey)
{
  // Détermine si le niveau de gris indiqué correspond à du blanc ou du noir
  return 0;
}

SDL_Surface binarizeSurface(SDL_Surface image)
{
  // Convertit l'image en niveau de gris en noir et blanc
  return image;
}
