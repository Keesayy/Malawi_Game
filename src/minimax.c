#include "minimax.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXSCORE 10000

Play choisir_coup(Game_state game, int profendeur){
    Play best_coup ;
    best_coup.piece = game.p1.pieces[0];

    if (Is_win(game) || profendeur == 0) {
        best_coup.a = MOVE; // Action par défaut
        best_coup.p = (Point){.x = 0, .y = 0}; // Position arbitraire
        best_coup.indices = NULL; // Aucun indice de division
        return best_coup;
    }

    Playlist **all_coups = Find_all_actions(&game);
    int best_score = (game.p == WHITE) ? -MAXSCORE : MAXSCORE;
    best_coup = all_coups[2]->play;

    for(int i = 0; i<3; ++i){
        Playlist *coups = all_coups[i];
        while (coups != NULL) {
            Play coup = coups->play;
            Game_state new_game = applique(&game, coup);
            int score = minimax(new_game, Opposite_player(game.p), profendeur);

            /* Watch score at each step for each actions possible */
            // switch(coup.a){
            // case MOVE:
            //     printf("Move ");
            //     break;
            // case ATTACK:
            //     printf("Attack ");
            //     break;
            // case SPLIT:
            //     printf("Split ");
            //     break;
            // default:
            //     printf("UNKNOW ?? ");
            //     break;  
            // }
            // printf("%d\n", score);

            if((game.p == WHITE && score > best_score) || (game.p == BLACK && score < best_score)){
                best_score = score ; 
                best_coup = coup ;
            }
            coups = coups->next;
        }
    }
    best_coup.indices = listCopy(best_coup.indices);

    printf("%d\n", best_score);
    PlaylistFree(all_coups[0]);
    PlaylistFree(all_coups[1]);
    PlaylistFree(all_coups[2]);
    free(all_coups);
    return best_coup;
}

Playlist **Find_all_actions(Game_state *game){
    Playlist **actions = (Playlist**)malloc(3*sizeof(Playlist*));

    actions[0] = Find_all_move(game);
    actions[1] = Find_all_attack(game);
    actions[2] = Find_all_split(game);

    return actions;
}

Game_state applique(Game_state *game, Play coup) {
    Game_state new_game = Copy_game(*game);

    switch (coup.a) {
        case MOVE:
            Apply_move(&new_game, coup.piece, coup.p);
            break;

        case ATTACK:
            Apply_attack(&new_game, coup.p);
            break;

        case SPLIT:
            Apply_split(&new_game, coup.piece, coup.indices);
            break;

        default:
            printf("Action inconnue, essaye une autre ;) \n");
            break;
    }
    return new_game;
}

void Update(Game_state *game, Play coup) {
    switch (coup.a) {
        case MOVE:
            Apply_move(game, coup.piece, coup.p);
            break;

        case ATTACK:
            Apply_attack(game, coup.p);
            break;

        case SPLIT:
            Apply_split(game, coup.piece, coup.indices);
            break;
    }
}

int compter_marbles(Game_state game, Player p) {
    Point nb = Marbles_players_number(game);

    if(p == WHITE)
        return 1 - nb.x;
    return 1 - nb.y;
}

int PlaylistSize(Playlist *s) {
    int count = 0;
    while(s != NULL){
        count++;
        s = s->next;
    }
    return count;
}

int nb_mouvements(Game_state game, Player p){
    game.p = p;
    Playlist *pl = Find_all_move(&game);
    Playlist *copy = pl;
    
    int size = PlaylistSize(pl);
    int score = 0;
    int row = SIZE - 1;

    if(game.p == WHITE) row = 0;

    while(copy != NULL){
        if(copy->play.p.x == row) score -= MAXSCORE/6;
        copy = copy->next;
    }
    PlaylistFree(pl);
	return size;
}

int nb_attaques(Game_state game, Player p){
    game.p = p;
    Playlist *pl = Find_all_attack(&game);
    Playlist *copy = pl;
    
    int size = PlaylistSize(pl);
    int score = 0;
    int row = 0;

    if(game.p == WHITE) row = SIZE - 1;

    while(copy != NULL){
        if(copy->play.p.x == row) score -= MAXSCORE/4;
        copy = copy->next;
    }

    PlaylistFree(pl);
    return score + size;
}

int nb_split(Game_state game, Player p){
    int score = 0;
    Piece *pinfo = game.p1.pieces;
    if(p == BLACK) pinfo = game.p2.pieces;

    for(int i=0; i<SIZE; ++i){
        if(pinfo[i].marbles == 0)
            score += 1;
        if(pinfo[i].marbles >= 1 && pinfo[i].marbles <= 2)
            score += 2;
        if(pinfo[i].marbles >= 3 && pinfo[i].marbles <= 5)
        score += 3;
    }

    return score;
}

int evaluation(Game_state game, Player p){
    int score = 0;

    if(Is_win(game) && p == BLACK) return -MAXSCORE; 

    int poids_valeur_marbles = 150;
    int poids_mobilite = 100;
    int poids_split = 200;
    int poids_attaque = 550;

    score += poids_valeur_marbles * compter_marbles(game, p);
    score -= poids_valeur_marbles * compter_marbles(game, Opposite_player(p));

    int nb_mouvements_joueur = nb_mouvements(game, p);
    score += poids_mobilite * (nb_mouvements_joueur);        

    int nb_attaques_joueur = nb_attaques(game, p);
    score += poids_attaque * (nb_attaques_joueur);

    int nb_split_joueur = nb_split(game, p);
    score += poids_split * (nb_split_joueur);

    return score;
}

int minimax(Game_state game, Player p, int profendeur) {
    if (Is_win(game) || profendeur == 0) {
        return evaluation(game, p);
    }

    Playlist **all_coups = Find_all_actions(&game);
    int best_score = (p == WHITE) ? -MAXSCORE : MAXSCORE;

    for(int i = 0; i<3; ++i){
        Playlist *coups = all_coups[i];
        while (coups != NULL) {
            Play coup = coups->play;
            Game_state new_game = applique(&game, coup);
            int score = minimax(new_game, Opposite_player(p), profendeur - 1);
            
            if ((p == WHITE && score > best_score) || (p == BLACK && score < best_score)) {
                best_score = score;
            }

            coups = coups->next;
        }
    }
    PlaylistFree(all_coups[0]);
    PlaylistFree(all_coups[1]);
    PlaylistFree(all_coups[2]);
    free(all_coups);
    return best_score;
}



