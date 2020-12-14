#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Pretreats/grayscale.h"
#include "../Pretreats/pixel_operations.h"
#include "../Pretreats/median.h"
#include "../Pretreats/otsu.h"
#include "../Segmentation/rlsa.h"

void pause();

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(1, "Invalid number of arguments..\n");
    
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    image_surface = IMG_Load(argv[1]);
    if (!image_surface)
      errx(3, "can't load %s: %s", argv[1], IMG_GetError());

    // Set the window to the same size as the image
    screen_surface = SDL_SetVideoMode(image_surface->w, image_surface->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen_surface == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                image_surface->w, image_surface->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(image_surface, NULL, screen_surface, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen_surface, 0, 0, image_surface->w, image_surface->h);
    
    pause();

    image_surface = grayscale(image_surface);
    update_surface(screen_surface, image_surface);

    pause();

    //image_surface = noise_canceled(image_surface);
    //update_surface(screen_surface, image_surface);

    //pause();

    image_surface = black_n_white(image_surface);
    update_surface(screen_surface, image_surface);

    pause();
    
    int w = image_surface->w;
    int h = image_surface->h;


    int **mat1;
    mat1 = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat1[i] = (int*) malloc(sizeof(int) * w);

    int **mat2;
    mat2 = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat2[i] = (int*) malloc(sizeof(int) * w);

    mat1 = set_matrix(image_surface, mat1);
    mat2 = set_matrix(image_surface, mat2);

    mat1 = rlsa_horizontal(mat1, h, w, 15);
    set_image(mat1, image_surface);
    update_surface(screen_surface, image_surface);

    pause();
    
    mat2 = rlsa_vertical(mat2, h, w, 25);
    set_image(mat2, image_surface);
    update_surface(screen_surface, image_surface);
    
    pause();

    set_image(rlsa_or(image_surface), image_surface);
    update_surface(screen_surface, image_surface);

    pause();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}

void pause()
{
    SDL_Event event;
    // Wait for a key to be down.
     do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
    
}
