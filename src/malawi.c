#include <stdlib.h>
#include <stdio.h>
#include "malawi.h"

#define ABS(A) ( ((A) > 0) ? (A) : (-(A)) )
const int moves[4][2] = {{0,1}, {0, -1}, {1, 0}, {-1, 0}};

void Show_player(Player p){
	if(p == WHITE) printf("Player White\n");
	else
	if(p == BLACK) printf("Player Black\n");
	else
		printf("Player Empty");
}

Piece Piece_init(Player p, Point pos, int marbles, int i){
	Piece piece;
	piece.p = p;
	piece.pos = pos;
	piece.marbles = marbles;
	piece.i = i;
	return piece;
}

Pieces_info Pieces_info_init(Player p){
	Pieces_info pi;
	pi.pieces = (Piece*)malloc(SIZE * sizeof(Piece));
	
	int x = 0;
	if(p == WHITE) x = SIZE - 1;

	for(int y=0; y<SIZE; ++y)
		pi.pieces[y] = Piece_init(p, (Point){.x = x, .y = y}, NB_MARBLES, y);

	return pi;
}

//w : white, b : black
Board Board_init(Pieces_info w, Pieces_info b){
	Board tab = (Board)malloc(SIZE * SIZE * sizeof(Piece));
	for(int j=0; j<SIZE; ++j){
		B(tab, 0, j) = b.pieces[j];
		B(tab, SIZE-1, j) = w.pieces[j];
	}

	for(int i=1; i<SIZE-1; ++i){
		for(int j=0; j<SIZE; ++j){
			B(tab, i, j) = Piece_init(EMPTY, (Point){.x = i, .y = j}, 0, 0);
		}
	}

	return tab;
}

Player Opposite_player(Player p){
	if(p == WHITE) return BLACK;
	if(p == BLACK) return WHITE;
	return EMPTY;
}

int Possible_move(Board tab, Piece piece, Point p){
	if(!((p.x >= 0 && p.x < SIZE) && (p.y >= 0 && p.y < SIZE)))
		return 0;

	Point direction = (Point){.x = p.x - piece.pos.x, .y = p.y - piece.pos.y};

	if(direction.x == 0 && direction.y != 0){
		int absy = ABS(direction.y);
		int dy = direction.y / absy;

		if((absy == piece.marbles) && (B(tab, p.x, p.y).p == EMPTY)){
			for(int i = 1; i<absy; ++i){
				if(B(tab, p.x, p.y - i*dy).p != EMPTY)
					return 0;
			}
			return 1;
		}
		return 0;

	}else if(direction.y == 0 && direction.x != 0){
		int absx = ABS(direction.x);
		int dx = direction.x / absx;

		if((absx == piece.marbles) && (B(tab, p.x, p.y).p == EMPTY)){
			for(int i = 1; i<absx; ++i){
				if(B(tab, p.x - i*dx, p.y).p != EMPTY)
					return 0;
				}
			return 1; 
		}
		return 0;
	}

	return 0;
}

int Possible_attack(Board tab, Piece piece, Point p){
   if(!((p.x >= 0 && p.x < SIZE) && (p.y >= 0 && p.y < SIZE)))
      return 0;

   Point direction = (Point){.x = p.x - piece.pos.x, .y = p.y - piece.pos.y};

   if(direction.x == 0 && direction.y != 0){
      int absy = ABS(direction.y);
      int dy = direction.y / absy;

      if((absy == piece.marbles) &&  (B(tab, p.x, p.y).p == Opposite_player(piece.p)) &&
         ((B(tab, p.x, p.y)).marbles != 0)){
         for(int i = 1; i<absy; ++i){
            if(B(tab, p.x, p.y - i*dy).p != EMPTY)
               return 0;
         }
         return 1;
      }
      return 0;

   }else if(direction.y == 0 && direction.x != 0){
      int absx = ABS(direction.x);
      int dx = direction.x / absx;

      if((absx == piece.marbles) && (B(tab, p.x, p.y).p == Opposite_player(piece.p)) &&
         ((B(tab, p.x, p.y)).marbles != 0)){
         for(int i = 1; i<absx; ++i){
            if(B(tab, p.x - i*dx, p.y).p != EMPTY)
               return 0;
         }
         return 1;
      }
      return 0;         
   }
   return 0;
}

int Possible_split(Piece piece, Piece *playerpieces, list *indices){
	if(indices == NULL) return 0;
	int list_size = listSize(indices);
	int marble_size = piece.marbles;

	if(marble_size == MAX_MARBLES) marble_size--;

	if(list_size == marble_size){
		while(indices != NULL){
			if(playerpieces[indices->value].marbles >= MAX_MARBLES)
				return 0;
			indices = indices->next;
		}
		return 1;
	}
	return 0;
}

Game_state Copy_game(Game_state game){
	Game_state new_game;

	new_game.p = game.p;
    new_game.p1 = Pieces_info_init(WHITE);
    new_game.p2 = Pieces_info_init(BLACK);
    for(int i=0; i<SIZE; ++i){
    	new_game.p1.pieces[i] = game.p1.pieces[i];
    	new_game.p2.pieces[i] = game.p2.pieces[i];
    }

    new_game.tab = Board_init(new_game.p1, new_game.p2);
	return new_game;
}

void Update_game(Game_state *game){
	game->p = Opposite_player(game->p);

	for(int i=0; i<SIZE*SIZE; ++i)
		game->tab[i] = Piece_init(EMPTY, (Point){.x = 0, .y = 0}, 0, 0);

	for(int i=0; i<SIZE; ++i){
		B(game->tab, game->p1.pieces[i].pos.x, game->p1.pieces[i].pos.y) = game->p1.pieces[i]; 
		B(game->tab, game->p2.pieces[i].pos.x, game->p2.pieces[i].pos.y) = game->p2.pieces[i]; 
	}
}

void Apply_move(Game_state *game, Piece piece, Point p){
	if(game->p == WHITE)
		game->p1.pieces[piece.i].pos = p;
	else
		game->p2.pieces[piece.i].pos = p;
	Update_game(game);
}

int Find_i_pieces_info(Pieces_info pinfo, Point p){
	for(int i=0; i<SIZE; ++i){
		if(pinfo.pieces[i].pos.x == p.x && pinfo.pieces[i].pos.y == p.y)
			return i;
	}
	return -1;
}

void Apply_attack(Game_state *game, Point p){
	if(game->p == WHITE){
		int i = Find_i_pieces_info(game->p2, p);
		if(i == -1){
			printf("i = -1 ERROR");
			return;
		}
		game->p2.pieces[i].marbles = 0;
	}
	else{
		int i = Find_i_pieces_info(game->p1, p);
		if(i == -1){
			printf("i = -1 ERROR");
			return;
		}
		game->p1.pieces[i].marbles = 0;
	}
	Update_game(game);
}

void Apply_split(Game_state *game, Piece piece, list *indices){
	if(game->p == WHITE){
		game->p1.pieces[piece.i].marbles = 0;
		while(indices != NULL){
			game->p1.pieces[indices->value].marbles += 1;
			indices = indices->next;
		}
	}else{
		game->p2.pieces[piece.i].marbles = 0;
		while(indices != NULL){
			game->p2.pieces[indices->value].marbles += 1;
			indices = indices->next;
		}
	}
	Update_game(game);
	listFree(indices);
}

Playlist *PlaylistAdd(Playlist *pl, Action a, Piece piece, Point p, list *indices){
	Playlist *head = (Playlist*)malloc(sizeof(Playlist));
	head->play.a = a;
	head->play.piece = piece;
	head->play.p = p;
	head->play.indices = indices;
	head->next = pl;
	return head;
}

void PlaylistFree(Playlist *pl){
	Playlist *copy = NULL;
	while(pl != NULL){
		copy = pl;
		listFree(pl->play.indices);
		pl = pl->next;
		free(copy);
	}
}

Playlist *Find_all_move(Game_state *game){
	Playlist *pl = NULL;
	Pieces_info pinfo = game->p2;

	if(game->p == WHITE) pinfo = game->p1;

	for(int i=0; i<SIZE; ++i){
		Piece piece = pinfo.pieces[i];
		int l = piece.marbles;

		for(int k=0; k<NB_COUPS; ++k){
			Point p = (Point){.x = piece.pos.x + l*moves[k][0], .y = piece.pos.y + l*moves[k][1]};
			if(Possible_move(game->tab, piece, p))
				pl = PlaylistAdd(pl, MOVE, piece, p, NULL);
		}
	}
	return pl;
}

Playlist *Find_all_attack(Game_state *game){
	Playlist *pl = NULL;
	Pieces_info pinfo = game->p2;

	if(game->p == WHITE) pinfo = game->p1;

	for(int i=0; i<SIZE; ++i){
		Piece piece = pinfo.pieces[i];
		int l = piece.marbles;

		for(int k=0; k<NB_COUPS; ++k){
			Point p = (Point){.x = piece.pos.x + l*moves[k][0], .y = piece.pos.y + l*moves[k][1]};
			if(Possible_attack(game->tab, piece, p))
				pl = PlaylistAdd(pl, ATTACK, piece, p, NULL);
		}
	}
	return pl;
}

list **All_partition_indices(Piece piece){
	list *indices = NULL;
	for(int i=0; i<SIZE; ++i){
		if(i != piece.i)
			indices = listAdd(indices, i);
	}
	return listPartition(indices, piece.marbles);
}

Playlist *Find_all_split(Game_state *game){
	Playlist *pl = NULL;
	Pieces_info pinfo = game->p2;

	if(game->p == WHITE) pinfo = game->p1;

	for(int i=0; i<SIZE; ++i){
		Piece piece = pinfo.pieces[i];
		list **partition = All_partition_indices(piece);
		int size_partition = comb(SIZE-1, piece.marbles);

		for(int k=0; k<size_partition; ++k){
			if(Possible_split(piece, pinfo.pieces, partition[k]))
				pl = PlaylistAdd(pl, SPLIT, piece, (Point){.x = -1, .y = -1}, partition[k]);
		}
		free(partition);
		partition = NULL;
	}
	return pl;
}

Point Marbles_players_number(Game_state game){
	int marbles_p1 = 0;
	int marbles_p2 = 0;
	for(int i=0; i<SIZE; ++i){
		marbles_p1 += game.p1.pieces[i].marbles;
		marbles_p2 += game.p2.pieces[i].marbles;
	}
	return (Point){.x = marbles_p1, .y = marbles_p2};
}

int Is_win(Game_state game){
	Piece *pieces_j = game.p2.pieces;
	Piece *pieces_jop = game.p1.pieces;
	int row = 0;

	if(game.p == WHITE){
		pieces_j = game.p1.pieces;
		pieces_jop = game.p2.pieces;
		row = SIZE-1;
	}

	int win = 1;
	//Cas 1
	for(int i=0; i<SIZE; ++i){
		if(pieces_j[i].marbles != 0){
			win = 0;
			break;
		}
	}
	if(win) return win;

	// Cas 2
	Playlist *pl = Find_all_attack(&game);
	int casefond = 0;
	for(int i=0; i<SIZE; ++i){
		if(pieces_jop[i].pos.x == row && pieces_jop[i].marbles != 0){
			casefond = 1;
			break;
		}
	}
	win = 1;
	while(pl != NULL){
		if(pl->play.p.x == row){
			win = 0;
			break;
		}
		pl = pl->next;
	}
	PlaylistFree(pl);
	return casefond && win;
}