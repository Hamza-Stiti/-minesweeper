#include "utils.h"

SDL_Surface* utils_init( int SCREEN_WIDTH, int SCREEN_HEIGHT )
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return NULL;
    }

    //Set up the screen
    SDL_Surface* screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_SWSURFACE );


    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return NULL;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Event test", NULL );

    //If everything initialized fine
    return screen;
}

SDL_Surface* utils_load_image( char* filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}


void utils_apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect *clip )
{
    //Temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Get the offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}



