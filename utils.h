#pragma once
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>



SDL_Surface* utils_init( int SCREEN_WIDTH, int SCREEN_HEIGHT ); // initialise SDL and return the screen after initialising it

SDL_Surface* utils_load_image( char* filename ); //loads an optimized image

void utils_apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect *clip ); //apply a part of an image "source" definedd by the sdl_rect "clip" and apply it on the "destination"(screen) on the coordinates defined by x, y

