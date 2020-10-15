#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "binarize.h"

void pause();

int main(int argc, char *argv[])
{
   if (argc != 2)
     return EXIT_FAILURE;
  
   SDL_Surface *ecran = NULL, *image = NULL;
   image = IMG_Load(argv[1]);
   SDL_Rect position;
   position.x = 0;
   position.y = 0;

   SDL_Init(SDL_INIT_VIDEO);
   ecran = SDL_SetVideoMode(image->w, image->h, 32, SDL_HWSURFACE);
   SDL_WM_SetCaption("OCR Project (667 Techs)", NULL);

   image = binarizeSurface(*image);
   
   SDL_BlitSurface(image, NULL, ecran, &position);

   SDL_Flip(ecran);
   pause();


   SDL_FreeSurface(image);
   SDL_Quit();

   return EXIT_SUCCESS;
}


void pause()
{
   int continuer = 1;
   SDL_Event event;

   while (continuer)
   {
       SDL_WaitEvent(&event);
       switch(event.type)
       {
           case SDL_QUIT:
               continuer = 0;
       }
   }
}
