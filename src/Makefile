#Malawi Game by Arthur PERAUD, Khalil BENKHALI, Hassan ELHILALI

CC = gcc
CFLAGS = -Wall -Wextra -g -Werror
LDFLAGS = -lm `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf
OBJ = main.o malawi.o list.o minimax.o

#Main
main : main.h $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

#Malawi 
malawi : malawi.c malawi.h $(OBJ)
	$(CC) -c malawi.c $(CFLAGS)

#Minimax
minimax.o : minimax.h minimax.c malawi.h
	$(CC) -c minimax.c $(CFLAGS)

#List
list.o : list.h list.c
	$(CC) -c list.c $(CFLAGS)

#Cleaning objects
clean :
	rm -f $(OBJ)