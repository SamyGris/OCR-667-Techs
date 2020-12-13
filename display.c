#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "grayscale.h"
#include "pixel_operations.h"
#include "median.h"
#include "otsu.h"
#include "rlsa.h"

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

    /*image_surface = noise_canceled(image_surface);

    update_surface(screen_surface, image_surface);

    pause();*/

    image_surface = black_n_white(image_surface);

    update_surface(screen_surface, image_surface);

    pause();
    
    int w = image_surface->w;
    int h = image_surface->h;


    int **mat_h;
    mat_h = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat_h[i] = (int*) malloc(sizeof(int) * w);

    int **mat_v;
    mat_v = (int **) malloc(sizeof(int *) * h);
    for (int i = 0; i < h; i++) mat_v[i] = (int*) malloc(sizeof(int) * w);

    mat_h = set_matrix(image_surface, mat_h);

    for (int i = 0; i<h; i++)
    {
	    for (int j = 0; j<w; j++)
	    {
		    if (mat_h[i][j] == 1) printf("\033[0;31m");
		    printf("%d", mat_h[i][j]);
		    if (mat_h[i][j] == 1) printf("\033[0m");
	    }
	    printf("%s","\n");
    }
    printf("%s","\n");

    pause();

    mat_h = rlsa_horizontal(mat_h, h, w, 3);
    for (int i = 0; i<h; i++)
    {
	    for (int j = 0; j<w; j++)
	    {
		    if (mat_h[i][j] == 1) printf("\033[0;31m");
		    printf("%d", mat_h[i][j]);
		    if (mat_h[i][j] == 1) printf("\033[0m");
	    }
	    printf("%s","\n");
    }
    printf("%s","\n");


    pause();

    mat_v = rlsa_vertical(mat_h, h, w, 3);
    for (int i = 0; i<h; i++)
    {
	    for (int j = 0; j<w; j++)
	    {
		    if (mat_h[i][j] == 1) printf("\033[0;31m");
		    printf("%d", mat_v[i][j]);
		    if (mat_h[i][j] == 1) printf("\033[0m");
	    }
	    printf("%s","\n");
    }
    printf("%s","\n");

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
