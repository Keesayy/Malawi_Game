#ifndef _MALAWI_H_
#define _MALAWI_H_

#include "list.h"

#define SIZE 6
#define NB_MARBLES 2
#define MAX_MARBLES 6
#define NB_COUPS 4

typedef struct{
	int x;
	int y;
}Point;

typedef enum{
	WHITE, BLACK, EMPTY
}Player;

typedef enum{
	ATTACK, MOVE, SPLIT
}Action;

typedef struct{
	Player p;
	Point pos;
	int marbles;
	int i; 
}Piece;

typedef struct{
	Piece *pieces;
}Pieces_info;

typedef Piece* Board;
#define B(A, I, J) A[(I)*SIZE + (J)]

typedef	struct{
	Player p;
	Board tab;
	Pieces_info p1; // Player white
	Pieces_info p2;	// Player black
}Game_state;

void Show_player(Player p);
Piece Piece_init(Player p, Point pos, int marbles, int i);
Pieces_info Pieces_info_init(Player p);
Board Board_init(Pieces_info p1, Pieces_info p2);
Player Opposite_player(Player p);

int Possible_move(Board tab, Piece piece, Point p);
int Possible_attack(Board tab, Piece piece, Point p);
int Possible_split(Piece piece, Piece *playerpieces, list *indices);

int Find_i_pieces_info(Pieces_info pinfo, Point p);

void Update_game(Game_state *game);
Game_state Copy_game(Game_state game);
void Apply_move(Game_state *game, Piece piece, Point p);
void Apply_attack(Game_state *game, Point p);
void Apply_split(Game_state *game, Piece piece, list *indices);

typedef struct{
	Action a;
	Piece piece;
	Point p;
	list *indices;
}Play;

typedef struct Playlist{
	Play play;
	struct Playlist *next;
}Playlist;

Playlist *PlaylistAdd(Playlist *s, Action a, Piece piece, Point p, list *indices);
void PlaylistFree(Playlist *pl);

Playlist *Find_all_move(Game_state *game);
Playlist *Find_all_attack(Game_state *game);

list **All_partition_indices(Piece piece);
Playlist *Find_all_split(Game_state *game);

// Playlist **Find_all_actions(Game_state *game);
Point Marbles_players_number(Game_state game);
int Is_win(Game_state game);

#endif