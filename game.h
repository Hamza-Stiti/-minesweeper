#pragma once
#include <SDL.h>
#include "utils.h"

typedef enum square
{
    bomb=-1,zero,one,two,three,four,five,six,seven,eight,flag
}square;

typedef enum state
{
    not_opened,opened,flaged
}state;

state** create_state(int length, int width);

void game_setscreen( SDL_Surface* normalsquare, SDL_Surface* screen, int length, int width);//initialize the game by creating the screen according to the length and width and uses bombs and other functions to create a 2D array representing the bomb placements and fill the other squares with numbers accordingly

square** create_board(int length, int width);

square** fill_bomb(square** board, int length, int width, int bombs); //create 2D array that serve as a representation of the game and fill it randomly with bombs

int fill_cell(square** board, int a, int b, int length, int width);// fills up the square with a number based on the number of bombs directly in touch with it

square** fill_num(square** board, int length, int width);

void special_reveal(square** board, state** board_state, int a, int b, int length, int width, SDL_Surface** numbers, SDL_Surface* screen);

void game_OpenNum(square** board, state** board_state, int a, int b, int length, int width, SDL_Surface** numbers, SDL_Surface* screen, int* gamecheck);//opening a number square (will be used when a player clicks on the square)



void flaging(state** board_state, int x, int y, int length, int width, SDL_Surface* normalsquare, SDL_Surface* screen);



void game_over(square** board, int length, int width, SDL_Surface* screen, int* gamecheck);

int playing_game(square** board, state** board_state, int length, int width, int bombs, SDL_Surface** numbers, SDL_Surface* normalsquare, SDL_Surface* screen);


int flooring(double x); //


int start_new_game(int length, int width, int bombs, SDL_Surface* screen);


int continu_game(SDL_Surface* screen);



int get_game_char( SDL_Surface* screen);


int win_game_check(state** board_state, int length, int width, int bombs);


void free_board(square** board_square, state** board_state, int length);
