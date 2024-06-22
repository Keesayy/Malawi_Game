#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include"malawi.h"

Play choisir_coup(Game_state game, int profendeur);

Playlist **Find_all_actions(Game_state *game);

Game_state applique(Game_state *game, Play coup);
void Update(Game_state *game, Play coup);

int compter_marbles(Game_state game, Player p);

int PlaylistSize(Playlist *s); 

int nb_mouvements(Game_state game, Player p);
int nb_attaques(Game_state game, Player p);
int nb_split(Game_state game, Player p);

int evaluation(Game_state game, Player p);

int minimax(Game_state game, Player p, int profendeur);

#endif