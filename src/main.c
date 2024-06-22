#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "main.h"

#define DELAY 100
#define WIDTH 1200
#define HEIGHT 600
#define SQUAREDIM (HEIGHT / SIZE)
#define DEPTH 2

void Set_color(SDL_Renderer *renderer, SDL_Color c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

SDL_Color Color_player(Player p){
    if(p == BLACK)
        return Brown;
    return White;
}

SDL_Color Color_pawn(Piece pion){
    return Color_player(pion.p);
}

SDL_Texture *Load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer){
   (void)window;
   SDL_Surface *my_image = NULL;
   SDL_Texture* my_texture = NULL;         

   my_image = IMG_Load(file_image_name);
   if (my_image == NULL){ 
      printf("Echec image : %s\n", IMG_GetError());
      exit(EXIT_FAILURE);
   }

   my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
   SDL_FreeSurface(my_image); 
   if (my_texture == NULL){
      printf("Echec transfo texture: %s\n", IMG_GetError());
      exit(EXIT_FAILURE);
   }
   return my_texture;
}

void Draw_background_texture(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer) {
   SDL_Rect 
   source = {0},           
   window_dimensions = {0},
   destination = {0};      

   SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); 
   SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);       
   destination = window_dimensions;

   SDL_RenderCopy(renderer, my_texture, &source, &destination);                
}

void end_sdl(char ok, const char* msg, SDL_Window* window, SDL_Renderer* renderer) {
    char msg_formated[255];
    int l;

    if (!ok) {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok) {
        exit(EXIT_FAILURE);
    }
}

void Draw_preboard(SDL_Renderer *renderer, SDL_Color c) {
    Set_color(renderer, c);
    for (int i = 0; i <= SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * SQUAREDIM, 0, i * SQUAREDIM, HEIGHT);
        SDL_RenderDrawLine(renderer, i * SQUAREDIM+1, 1, i * SQUAREDIM+1, HEIGHT+1);
        SDL_RenderDrawLine(renderer, 0, i * SQUAREDIM, HEIGHT, i * SQUAREDIM);
        SDL_RenderDrawLine(renderer, 1, i * SQUAREDIM+1, HEIGHT+1, i * SQUAREDIM+1);
    }
}

void Draw_board(SDL_Renderer *renderer, TTF_Font* font, Game_state game){
    Draw_preboard(renderer, Brown);

    for (int i = 0; i < SIZE; ++i) {
        Draw_pawn(renderer, font, game.p1.pieces[i], Red);
        Draw_pawn(renderer, font, game.p2.pieces[i], Red);
    }
    if(game.p == WHITE)
        Draw_text(renderer, font, "Player turn : White", WIDTH/2 + 19*16/2, 0, Color_player(game.p));
    else
        Draw_text(renderer, font, "Player turn : Black", WIDTH/2 + 19*16/2, 0, Color_player(game.p));
    Draw_text(renderer, font, "How to play", WIDTH/2 + WIDTH/4 - 19*9/2, 200, Beige);
    Draw_text(renderer, font, "To Move    : Left click + Drag", 700, 275, Beige);
    Draw_text(renderer, font, "To Split   : Left click + ", 700, 325, Beige);
    Draw_text(renderer, font, "               Choose with  ", 700, 350, Beige);
    Draw_text(renderer, font, "               Right click  ", 700, 375, Beige);
    Draw_text(renderer, font, "To Attack : Left click + Drag", 700, 425, Beige);
    Draw_text(renderer, font, "Good luck Have fun !", WIDTH/2 + WIDTH/4 - 19*20/2.5f, 500, Beige);
}

void Draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color couleur) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, couleur);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Draw_nb_marbles(SDL_Renderer *renderer, TTF_Font *font, Game_state game){
    char nb_p1[8];
    char nb_p2[8];
    Point nb_marbles = Marbles_players_number(game);

    sprintf(nb_p1, "%d/%d", nb_marbles.x, NB_MARBLES * SIZE);
    sprintf(nb_p2, "%d/%d", nb_marbles.y, NB_MARBLES * SIZE);

    Draw_text(renderer, font, nb_p1, WIDTH/2 +5, HEIGHT-20, White);
    Draw_text(renderer, font, nb_p2, WIDTH/2 +5, 0, Brown);
}

void Draw_fill_circle(SDL_Renderer *renderer, int x, int y , int r, SDL_Color c){
    Set_color(renderer, c);
    for(int w = 0; w<=2*r ; ++w){
        for(int h = 0 ; h<=2*r; ++h){
            int dx = r - w;
            int dy = r - h;
            if((SQUARE(dx) + SQUARE(dy)) <= SQUARE(r))
                SDL_RenderDrawPoint(renderer, x + dx , y + dy);
        }
    }
}

void Draw_circle(SDL_Renderer *renderer, int x, int y,  int r, int t, SDL_Color c, SDL_Color d){
    Draw_fill_circle(renderer, x, y, r+t, c);
    Draw_fill_circle(renderer, x, y, r, d);
}

void Draw_pawn(SDL_Renderer* renderer, TTF_Font* font, Piece pion, SDL_Color c){
    int centerX = pion.pos.y * SQUAREDIM + SQUAREDIM / 2;
    int centerY = pion.pos.x * SQUAREDIM + SQUAREDIM / 2;
    int radius = SQUAREDIM / 4;

    Draw_fill_circle(renderer, centerX, centerY, radius, Color_pawn(pion));

    Set_color(renderer, c);
    for (int i = 0; i < pion.marbles; ++i) {
        SDL_Rect ring = { centerX + i * 4, centerY + i * 4, radius * 2 - i * 8, radius * 2 - i * 8 };
        Draw_circle(renderer, ring.x, ring.y, ring.w/2, 2, c, Color_pawn(pion));
        ring.x +=1;
        ring.y +=1;
        Draw_circle(renderer, ring.x, ring.y, ring.w/2, 2, c, Color_pawn(pion));
    }

    char text[2];
    sprintf(text, "%d", pion.marbles);
    Draw_text(renderer, font, text, centerX - radius / 2, centerY - radius - 20, Color_pawn(pion));
}

void Draw_select_pawn_split(SDL_Renderer* renderer, TTF_Font* font, Piece pion, SDL_Color c){
    int centerX = pion.pos.y * SQUAREDIM + SQUAREDIM / 2;
    int centerY = pion.pos.x * SQUAREDIM + SQUAREDIM / 2;
    int radius = SQUAREDIM / 4;

    if(pion.marbles != 0)
        Draw_fill_circle(renderer, centerX, centerY, radius, Color_pawn(pion));
    else
        Draw_fill_circle(renderer, centerX, centerY, radius, c);

    Set_color(renderer, c);
    for (int i = 0; i < pion.marbles; ++i) {
        SDL_Rect ring = { centerX + i * 4, centerY + i * 4, radius * 2 - i * 8, radius * 2 - i * 8 };
        Draw_circle(renderer, ring.x, ring.y, ring.w/2, 2, c, Color_pawn(pion));
        ring.x +=1;
        ring.y +=1;
        Draw_circle(renderer, ring.x, ring.y, ring.w/2, 2, c, Color_pawn(pion));
    }

    char text[2];
    sprintf(text, "%d", pion.marbles);
    Draw_text(renderer, font, text, centerX - radius / 2, centerY - radius - 20, Color_pawn(pion));
}

void Show_play(Play coup){
    switch(coup.a){
    case MOVE:
        printf("Move\n");
        break;
    case ATTACK:
        printf("Attack\n");
        break;
    case SPLIT:
        printf("Split\n");
        break;
    default:
        printf("UNKNOW ??\n");
        break;  
    }
    printf("%d\n", coup.piece.i);
    printf("%d %d %d\n", coup.piece.pos.x, coup.piece.pos.y, coup.piece.marbles);
    printf("%d %d\n", coup.p.x, coup.p.y);
    listDisplay(coup.indices);
    printf("\n");
}

int main(void) {
    int flags = IMG_INIT_JPG|IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if((initted&flags) != flags){
        printf("IMG_Init: Impossible d'initialiser le support des formats JPG et PNG requis!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "Erreur d'initialisation SDL", NULL, NULL);
    }

    if (TTF_Init() != 0) {
        end_sdl(0, "Erreur d'initialisation TTF", NULL, NULL);
    }

    SDL_Window* window = SDL_CreateWindow("Malawi_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT+1, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        end_sdl(0, "Erreur de création de fenêtre", NULL, NULL);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        end_sdl(0, "Erreur de création de renderer", window, NULL);
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        end_sdl(0, "Erreur de chargement de la police", window, renderer);
    }
    TTF_Font* font1 = TTF_OpenFont("arial.ttf", 70);
    SDL_Texture *fond = Load_texture_from_image("fond2.jpg", window, renderer);

    //Affiche gagnant
    Player winner = EMPTY;
    int win = false;

    //Pour coup split
    bool s_pressed = false;
    list *indices = NULL;
    list *copy = NULL;
    int i_split = -1;
    int count = 0;

    SDL_Event e;
    int i_pawn = -1;
    bool quit = false;

    Game_state game;
    game.p = WHITE;
    game.p1 = Pieces_info_init(WHITE);
    game.p2 = Pieces_info_init(BLACK);
    game.tab = Board_init(game.p1, game.p2);

    while (!quit) {
        Piece *pinfo = game.p1.pieces;
        if(game.p == BLACK) pinfo = game.p2.pieces; 

        /* Joueur Black : minmax */
        SDL_Delay(DELAY);
        if(game.p == BLACK && !win){
            Play coup = choisir_coup(game, DEPTH);
            Show_play(coup);
            Update(&game, coup);
            listFree(coup.indices);
        }
        /* Joueur Black : minmax */

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                printf("Quitting game\n"); 
                quit = true;
                break;

            case SDL_KEYDOWN :
                if (e.key.keysym.sym == SDLK_q){
                    quit = true;
                    printf("Quitting game\n");
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                s_pressed = false;
                if(e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    for (int i = 0; i < SIZE; ++i) {
                        int centerX = pinfo[i].pos.y * SQUAREDIM + SQUAREDIM / 2;
                        int centerY = pinfo[i].pos.x * SQUAREDIM + SQUAREDIM / 2;
                        int radius = SQUAREDIM / 4;

                        if (mouseX >= centerX - radius && mouseX <= centerX + radius &&
                            mouseY >= centerY - radius && mouseY <= centerY + radius) {
                            i_pawn = i;
                            break;
                        }
                    }
                }
                if(e.button.button == SDL_BUTTON_RIGHT){
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    for (int i = 0; i < SIZE; ++i) {
                        int centerX = pinfo[i].pos.y * SQUAREDIM + SQUAREDIM / 2;
                        int centerY = pinfo[i].pos.x * SQUAREDIM + SQUAREDIM / 2;
                        int radius = SQUAREDIM / 4;

                        if (mouseX >= centerX - radius && mouseX <= centerX + radius &&
                            mouseY >= centerY - radius && mouseY <= centerY + radius) {

                            i_split = i;
                            if(i_split != i_pawn && pinfo[i_split].marbles != MAX_MARBLES){
                                count++;
                                s_pressed = true;
                                indices = listAdd(indices, i_split);
                            }
                            break;
                        }
                    }

                    if(count == pinfo[i_pawn].marbles || count == SIZE-1){
                        if(Possible_split(pinfo[i_pawn], pinfo, indices))
                            Apply_split(&game, pinfo[i_pawn], indices);
                        //indices free dans apply
                        indices = NULL;
                        i_split = -1;
                        i_pawn = -1;
                        count = 0;
                        s_pressed = false;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:

                if(e.button.button == SDL_BUTTON_LEFT && i_pawn != -1) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;

                    Point point = (Point){.x = pinfo[i_pawn].pos.x, .y = pinfo[i_pawn].pos.y};

                    int moveDistance = pinfo[i_pawn].marbles;
                    if (mouseX < pinfo[i_pawn].pos.y * SQUAREDIM) {
                        point.y -= moveDistance;

                    } else if (mouseX > (pinfo[i_pawn].pos.y + 1) * SQUAREDIM) {
                        point.y += moveDistance;

                    } else if (mouseY > (pinfo[i_pawn].pos.x + 1) * SQUAREDIM) {
                        point.x += moveDistance;

                    } else if (mouseY < pinfo[i_pawn].pos.x * SQUAREDIM) {
                        point.x -= moveDistance;

                    }
                    if(Possible_move(game.tab, pinfo[i_pawn], point)){
                        Apply_move(&game, pinfo[i_pawn], point);
                        i_pawn = -1;
                    }else if(Possible_attack(game.tab, pinfo[i_pawn], point)){
                        Apply_attack(&game, point);
                        i_pawn = -1;
                    }
                }
                break;    
            default:
                break;
            }
        }
        Draw_background_texture(fond, window, renderer);
        Draw_board(renderer, font, game);

        Draw_nb_marbles(renderer, font, game);
        if(i_pawn != -1) Draw_pawn(renderer, font, pinfo[i_pawn], Grey);
        if(s_pressed){
            Draw_text(renderer, font, "Player is splitting", WIDTH/2 + 19*16/2, 50, Green);
            copy = indices;
            while(copy != NULL){
                Draw_select_pawn_split(renderer, font, pinfo[copy->value], Green);
                copy = copy->next;
            }
        }else{
            listFree(indices);
            indices = NULL;
            i_split = -1;
            count = 0;
        }

        if(Is_win(game)){
            win = true; 
            winner = Opposite_player(game.p);
        }

        if(win){
            if(winner == WHITE) 
                Draw_text(renderer, font1, "WINNER IS PLAYER WHITE", WIDTH/20, HEIGHT/2.2f, Color_player(winner));
            else
                Draw_text(renderer, font1, "WINNER IS PLAYER BLACK", WIDTH/20, HEIGHT/2.2f, Color_player(winner));
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(fond);
    TTF_Quit();
    SDL_Quit();
    listFree(indices);
    free(game.tab);
    free(game.p1.pieces);
    free(game.p2.pieces);
    return 0;
}
