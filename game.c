#include "game.h"
#include <stdlib.h>



state** create_state(int length, int width)
{
    // dynamically allocate memory for the table
    state** board_state = (state**)malloc(length * sizeof(state*));
    for (int i = 0; i < length; i++)
        board_state[i] = (state*)malloc( width * sizeof(state));

    //initialize the table with all 0
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            board_state[i][j] = not_opened;
        }
    }

    return board_state;
}




void game_setscreen( SDL_Surface* normalsquare, SDL_Surface* screen, int length, int width)
{
    SDL_Surface* quit = utils_load_image("quit.png");
    utils_apply_surface(100 + length * 10, 0, quit, screen, NULL);

    //according to the length and width apply ( length * width ) square to the screen
    for (int i=0; i < length; i++)
    {
        for (int j=0; j < width; j++)
        {
            utils_apply_surface(100+i*20, 50+j*20, normalsquare, screen, NULL);
        }
    }

}


square** create_board(int length, int width)
{
    // dynamically allocate memory for the table
    square** board = (square**)malloc(length * sizeof(square*));
    for (int i = 0; i < length; i++)
        board[i] = (square*)malloc( width * sizeof(square));

    //initialize the table with all 0
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            board[i][j] = zero;
        }
    }

    return board;
}


square** fill_bomb(square** board, int length, int width, int bombnumber) // recusrsive function
{
    if (bombnumber == 0)
        return board; // the function finishes when the bombs are all placed

    int a = rand() % length , b = rand() % width; // random bomb placement

    if (board[a][b] == bomb)
        fill_bomb(board, length, width, bombnumber); // in case the place chosen randomly already contain a bomb the function call itself without decrementing the bomb number
    else
    {
        board[a][b] = bomb; //fill the zero initialized cell with a bomb
        fill_bomb(board, length, width, bombnumber-1); // function calling itself with decremented bomb number
    }
}


int fill_cell(square** board, int a, int b, int length, int width)
{
    square result = zero;// result to return later initialized by 0 in case there is no bombs near it

    for (int i = a-1; i <= a+1; i++)
    {
        if (i >= 0 && i < length)
        {
            for (int j = b-1; j <= b+1; j++)
            {
                if (j >= 0 && j < width)
                {
                    if(board[i][j] == -1)
                    {
                        result++; //run through all the nearby cells and increment result each time it finds a bomb
                    }
                }
            }
        }
    }

    return result;
}

square** fill_num(square** board, int length, int width)
{

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] != bomb)
            {
                board[i][j] = fill_cell(board, i, j, length, width); // run through each cell of the table and fill it using fill_cell function
            }
        }
    }
    return board;
}

void special_reveal(square** board, state** board_state, int a, int b, int length, int width, SDL_Surface** numbers, SDL_Surface* screen)
{
    for (int i = a-1; i <= a+1; i++)
    {
        if (i >= 0 && i < length)
        {
            for (int j = b-1; j <= b+1; j++)
            {
                if (j >= 0 && j < width)
                {
                    if (board_state[i][j] == not_opened)
                        game_OpenNum(board, board_state, i, j, length, width, numbers, screen, NULL);//if it is zero it will open all the non-opened nearby cells
                }
            }
        }
    }
}

void game_OpenNum(square** board, state** board_state, int a, int b, int length, int width, SDL_Surface** numbers, SDL_Surface* screen, int* gamecheck)
{
    if (board_state[a][b] != not_opened)
        return;

    if (board[a][b] == bomb){
        game_over(board, length, width, screen, gamecheck);

        for (int i = 0; i < length; ++i)
                    for (int j = 0; j < width; ++j)
                        board_state[i][j] = opened;
        return;
    }

    board_state[a][b] = opened;
    //opening the square by applying the image of the number according to its coordinates in the representing 2D array
    if (board[a][b] != zero){

        utils_apply_surface(100 + a * 20, 50 + b * 20, numbers[board[a][b]], screen, NULL );

    }else{

        utils_apply_surface(100 + a * 20, 50 + b * 20, numbers[0], screen, NULL );
        special_reveal(board, board_state, a, b, length, width, numbers, screen);

    }
}



void flaging(state** board_state, int x, int y, int length, int width, SDL_Surface* normalsquare, SDL_Surface* screen){

    if (board_state[x][y] == opened)
        return;

    SDL_Surface* flag_image = utils_load_image("flag.png");

    if (board_state[x][y] == flaged){
        board_state[x][y] = not_opened;
        utils_apply_surface(100+x*20, 50+y*20, normalsquare, screen, NULL);
    } else{
        board_state[x][y] = flaged;
        utils_apply_surface(100+x*20, 50+y*20, flag_image, screen, NULL);
    }

    SDL_FreeSurface(flag_image);
}


void game_over(square** board, int length, int width, SDL_Surface* screen, int* gamecheck){
    SDL_Surface* bomb_image = utils_load_image("bomb.png");
    for (int i = 0; i < length; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (board[i][j] == bomb)
            {
                utils_apply_surface(100 + (20 * i) , 50 + (20 * j), bomb_image, screen, NULL );
            }
        }
    }
    SDL_Surface* game_over_message = utils_load_image("game over.png");
    utils_apply_surface((20 * length) / 2, (20 * width) / 2, game_over_message, screen, NULL );
    *gamecheck = 0;
    FILE* fp = fopen("continu.txt", "w");
    putw(0,fp);
    fclose(fp);
    SDL_FreeSurface(bomb_image);
    SDL_FreeSurface(game_over_message);
}


int playing_game(square** board, state** board_state, int length, int width, int bombs, SDL_Surface** numbers, SDL_Surface* normalsquare, SDL_Surface* screen)
{
    SDL_Event event;
    int is_running = 1, gamecheck = 1;
    while (is_running)
    {
        while(SDL_PollEvent(&event))
        {
            if  (event.type == SDL_QUIT)
            {
                if (gamecheck)
                {
                    FILE* fp = fopen("continu.txt", "w");
                    putw(1, fp);
                    putw(length, fp);
                    putw(width, fp);
                   //putw(bombs, fp);
                    for (int i = 0; i < length; ++i)
                        for(int j = 0; j < width; ++j)
                            putw(board[i][j], fp);

                    for (int i = 0; i < length; ++i)
                        for (int j = 0; j < width; ++j)
                            putw(board_state[i][j], fp);

                    fclose(fp);
                }
                return 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.x >= 100 && event.button.x <= (100 + length * 20) && event.button.y >= 50 && event.button.y <= (50 + width * 20) && gamecheck)
                    {
                    int x = (flooring(event.button.x)-100) / 20, y = (flooring(event.button.y) - 50) / 20;

                    if (event.button.button == SDL_BUTTON_LEFT)
                        game_OpenNum(board, board_state, x, y,length, width, numbers, screen, &gamecheck);

                    else if(event.button.button == SDL_BUTTON_RIGHT)
                        flaging(board_state, x, y, length, width, normalsquare, screen);

                }
                else if (event.button.x >= (100 + length * 10) && event.button.x <= (100 + length * 10) + 170 && event.button.y >= 0 && event.button.y < 50 )
                {
                    if (gamecheck)
                    {
                    FILE* fp = fopen("continu.txt", "w");
                    putw(1, fp);
                    putw(length, fp);
                    putw(width, fp);
                    for (int i = 0; i < length; ++i)
                        for(int j = 0; j < width; ++j)
                            putw(board[i][j], fp);

                    for (int i = 0; i < length; ++i)
                        for (int j = 0; j < width; ++j)
                            putw(board_state[i][j], fp);

                    fclose(fp);
                    }
                    return 1;
                }

            }
            int win = win_game_check(board_state, length, width, bombs);
            if (win)
            {
                FILE* fp = fopen("continu.txt", "w");
                putw(0,fp);
                fclose(fp);
                SDL_Surface* win_message = utils_load_image("win.png");
                gamecheck = 0;
                utils_apply_surface((20 * length) / 2, (20 * width) / 2, win_message, screen, NULL );
                SDL_FreeSurface(win_message);
                for (int i = 0; i < length; ++i)
                    for (int j = 0; j < width; ++j)
                        board_state[i][j] = opened;
            }


            //flip screen
            if( SDL_Flip( screen ) == -1 )
                return 1;

        }

    }
}


int flooring(double x){
    int a = 0;
    for (int i = 0; i+1 <= x; i++ ) a++;
    return a;
}



int start_new_game(int length, int width, int bombs, SDL_Surface* screen)
{
    SDL_Surface* normalsquare = utils_load_image( "square.png" );


    //getting numbers
    SDL_Surface* numbers[9];
    numbers[0] = utils_load_image("0.png");
    numbers[1] = utils_load_image("1.png");
    numbers[2] = utils_load_image("2.png");
    numbers[3] = utils_load_image("3.png");
    numbers[4] = utils_load_image("4.png");
    numbers[5] = utils_load_image("5.png");
    numbers[6] = utils_load_image("6.png");
    numbers[7] = utils_load_image("7.png");
    numbers[8] = utils_load_image("8.png");

    game_setscreen(normalsquare, screen, length, width);

    square** board = create_board(length, width);
    state** board_state = create_state(length, width);
    board = fill_bomb(board, length, width, bombs);
    board = fill_num(board, length, width);

    screen = SDL_SetVideoMode( 200 + length * 20, 100 + width * 20, 0, SDL_SWSURFACE );
    if ( screen == NULL )
        return 0;

    game_setscreen( normalsquare, screen, length, width);

    int check = playing_game(board, board_state, length, width, bombs, numbers, normalsquare, screen);

    //free and quit
    SDL_FreeSurface(normalsquare);
    SDL_FreeSurface(numbers[0]);
    SDL_FreeSurface(numbers[1]);
    SDL_FreeSurface(numbers[2]);
    SDL_FreeSurface(numbers[3]);
    SDL_FreeSurface(numbers[4]);
    SDL_FreeSurface(numbers[5]);
    SDL_FreeSurface(numbers[6]);
    SDL_FreeSurface(numbers[7]);
    SDL_FreeSurface(numbers[8]);
    SDL_FreeSurface(screen);

    free_board( board, board_state, length);

    return check;
}


int continu_game(SDL_Surface* screen)
{
    SDL_Surface* normalsquare = utils_load_image( "square.png" ),
                *flag_image = utils_load_image("flag.png");



    //getting numbers
    SDL_Surface* numbers[9];
    numbers[0] = utils_load_image("0.png");
    numbers[1] = utils_load_image("1.png");
    numbers[2] = utils_load_image("2.png");
    numbers[3] = utils_load_image("3.png");
    numbers[4] = utils_load_image("4.png");
    numbers[5] = utils_load_image("5.png");
    numbers[6] = utils_load_image("6.png");
    numbers[7] = utils_load_image("7.png");
    numbers[8] = utils_load_image("8.png");


    FILE* fp = fopen("continu.txt", "r");
    getw(fp);
    int length = getw(fp), width = getw(fp);

    square** board = create_board(length, width);
    state** board_state = create_state(length, width);

    for (int i = 0; i < length; ++i)
        for (int j = 0; j < width; ++j)
            board[i][j] = getw(fp);

    for (int i = 0; i < length; ++i)
        for (int j = 0; j < width; ++j)
            board_state[i][j] = getw(fp);
    fclose(fp);

    screen = SDL_SetVideoMode( 200 + length * 20, 100 + width * 20, 0, SDL_SWSURFACE );
    if ( screen == NULL )
        return 1;

    game_setscreen( normalsquare, screen, length, width);

    for (int i=0; i < length; i++)
    {
        for (int j=0; j < width; j++)
        {
            if (board_state[i][j] == opened)
                utils_apply_surface(100+i*20, 50+j*20, numbers[board[i][j]], screen, NULL);

            else if (board_state[i][j] == flaged)
                utils_apply_surface(100+i*20, 50+j*20, flag_image, screen, NULL);
        }
    }
    int bombs = 0;
    for (int i = 0; i < length; ++i)
        for (int j = 0; j < width; ++j)
            if(board[i][j] == bomb)
                bombs++;

    int check = playing_game(board, board_state, length, width, bombs, numbers, normalsquare, screen);


    //free
    SDL_FreeSurface(normalsquare);
    SDL_FreeSurface(numbers[0]);
    SDL_FreeSurface(numbers[1]);
    SDL_FreeSurface(numbers[2]);
    SDL_FreeSurface(numbers[3]);
    SDL_FreeSurface(numbers[4]);
    SDL_FreeSurface(numbers[5]);
    SDL_FreeSurface(numbers[6]);
    SDL_FreeSurface(numbers[7]);
    SDL_FreeSurface(numbers[8]);
    SDL_FreeSurface(screen);

    free_board( board , board_state, length);

    return check;

}



int get_game_char(SDL_Surface* screen)
{
    int length = 20, width = 20, bombs = 50, result = 1, is_running = 1;
    SDL_Surface *background = utils_load_image("background");
    SDL_Surface *easy = utils_load_image("easy.png");
    SDL_Surface *normal = utils_load_image("normal.png");
    SDL_Surface *hard = utils_load_image("hard.png");
    SDL_Event event;


    while (is_running)
    {
        utils_apply_surface(0, 0, background, screen, NULL);
        utils_apply_surface(200, 100, easy, screen, NULL);
        utils_apply_surface(200, 200, normal, screen, NULL);
        utils_apply_surface(200, 300, hard, screen, NULL);

        while(SDL_PollEvent(&event))
        {
            if  (event.type == SDL_QUIT)
            {
                return 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                if (event.button.x >= 200 && event.button.x <= 400 && event.button.y >= 100 && event.button.y < 200)
                {

                    length = 10; width = 10; bombs = 10;
                    is_running = 0;

                }
                else if(event.button.x >= 200 && event.button.x <= 400 && event.button.y >= 200 && event.button.y < 300)
                {

                    length = 16; width = 16; bombs = 40;
                    is_running = 0;

                }
                else if(event.button.x >= 200 && event.button.x <= 400 && event.button.y >= 300 && event.button.y < 400)
                {

                    length = 30; width = 16; bombs = 99;
                    is_running = 0;

                }
            }


            //flip screen
            if( SDL_Flip( screen ) == -1 )
                return 0;

        }

    }
    SDL_FreeSurface(background);
    SDL_FreeSurface(easy);
    SDL_FreeSurface(normal);
    SDL_FreeSurface(hard);

    result = start_new_game(length, width, bombs, screen);
    return result;
}



int win_game_check(state** board_state, int length, int width, int bombs)
{
    int result = 0, case_win = length * width - bombs;

    for (int i = 0; i < length; ++i)
        for (int j = 0; j < width; ++j)
            if (board_state[i][j] == opened)
                ++result;

    return result == case_win;
}

void free_board(square** board_square, state** board_state, int length)
{

    for (int i = 0; i < length; ++i)
    {
        free(board_square[i]);
        free(board_state[i]);
    }

    free(board_square);
    free(board_state);

}



