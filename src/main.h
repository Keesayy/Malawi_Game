#ifndef _MAIN_H_
#define _MAIN_H_

#include "malawi.h"
#include "minimax.h"

#define SQUARE(A) ((A) * (A))

const SDL_Color White = (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255};
const SDL_Color Black = (SDL_Color){.r = 0, .g = 0, .b = 0, . a = 255};
const SDL_Color Grey = (SDL_Color){.r = 128, .g = 128, .b = 128, .a = 255}; 
const SDL_Color Gold = (SDL_Color){.r = 218, .g = 165, .b = 32, .a = 255};
const SDL_Color Red = (SDL_Color){.r = 237, .g = 0  , .b = 16, .a = 255};
const SDL_Color Brown = (SDL_Color){.r = 88, .g = 41  , .b = 0, .a = 255};
const SDL_Color Green = (SDL_Color){.r = 0, .g = 128  , .b = 0, .a = 255};
const SDL_Color Beige = (SDL_Color){.r = 245, .g = 245  , .b = 220, .a = 255};

void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture *Load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer);
void Draw_background_texture(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer);

void Set_color(SDL_Renderer *renderer, SDL_Color c);
SDL_Color Color_player(Player p);
SDL_Color Color_pawn(Piece pion);

void Draw_fill_circle(SDL_Renderer *renderer, int x, int y , int r, SDL_Color c);
void Draw_circle(SDL_Renderer *renderer, int x, int y,  int r, int t, SDL_Color c, SDL_Color d);
void Draw_preboard(SDL_Renderer *renderer, SDL_Color c);
void Draw_board(SDL_Renderer *renderer, TTF_Font* font, Game_state game);
void Draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color couleur);
void Draw_nb_marbles(SDL_Renderer *renderer, TTF_Font *font, Game_state game);
void Draw_pawn(SDL_Renderer* renderer, TTF_Font* font, Piece pion, SDL_Color c);
void Draw_select_pawn_split(SDL_Renderer* renderer, TTF_Font* font, Piece pion, SDL_Color c);

#endif