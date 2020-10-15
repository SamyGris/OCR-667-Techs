#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "binarize.h"

void pause();

int main(int argc, char *argv[])
{
   if (argc != 2)
     return EXIT_FAILURE;
  
   SDL_Window *ecran = NULL;
   SDL_Renderer *renderer = NULL;
   SDL_Surface *image = NULL;
   image = IMG_Load(argv[1]);

   SDL_Init(SDL_INIT_VIDEO);
     
   ecran = SDL_CreateWindow("OCR Project (667 Techs)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image->w, image->h, SDL_WINDOW_SHOWN);

   renderer = SDL_CreateRenderer(ecran, -1, SDL_RENDERER_ACCELERATED);

   if(NULL == renderer)
     {
       fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
       return EXIT_FAILURE;
     }
   
   image = binarizeSurface(image);

   SDL_CreateTextureFromSurface(renderer, image);

   pause();

   SDL_DestroyRenderer(renderer);
   SDL_FreeSurface(image);
   SDL_DestroyWindow(ecran);
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
