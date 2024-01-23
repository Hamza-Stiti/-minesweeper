#include <SDL.h>
#include "utils.h"
#include "game.h"
#include <stdlib.h>
#include<time.h>


/*int flooring(double x){
    int a = 0;
    for (int i = 0; i+1 <= x; i++ ) a++;
    return a;
}*/



int main ( int argc, char** argv )
{
    srand(time(0)); //for the use of rand later on the prog

    //initialising
    SDL_Surface* screen = utils_init( 640, 480 );
    if ( screen == NULL )
        return 1;

    SDL_Event event;
    int is_running = 1/*, length = 10, width = 10, bombs = 30*/;


    SDL_Surface* new_game = utils_load_image("new game.png"), *continu = utils_load_image("continu.png"), *background = utils_load_image("background.png");



    while (is_running)
    {
        screen = SDL_SetVideoMode( 640, 480, 0, SDL_SWSURFACE );
        utils_apply_surface(0, 0, background, screen, NULL);
        utils_apply_surface(190, 130, new_game, screen, NULL);
        utils_apply_surface(190, 200, continu, screen, NULL);

        while(SDL_PollEvent(&event))
        {
            if  (event.type == SDL_QUIT)
            {
                is_running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.x >= 190 && event.button.x <= 420 && event.button.y >= 130 && event.button.y <= 180)
                {

                    is_running = get_game_char(screen);

                }
                else if(event.button.x >= 190 && event.button.x <= 420 && event.button.y >= 200 && event.button.y <= 250)
                {
                    FILE* fp = fopen("continu.txt", "r");
                    int cont = getw(fp);
                    fclose(fp);
                    if (cont == 1)
                        is_running = continu_game(screen);
                }
            }


            //flip screen
            if( SDL_Flip( screen ) == -1 )
                return 1;

        }

    }

    return 0;
}
