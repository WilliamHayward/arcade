#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

typedef struct {
    int info;
    int size;
    int card[13];
} Run;

typedef struct {
    Run run[35];
    int runs;
    int play[106];
} Board;

typedef struct {
    int card[106];
    int position;
    bool exhausted;
} Deck;

typedef struct {
    int card[106];
    int size;
} Hand;

typedef struct {
    Hand hand;
    char* name;
} Player;

typedef struct {
    Board board;
    Board boardCopy;
    Deck deck;
    Player player[2];
    Hand loose;
    int currentPlayer;
} Game;

typedef int Card;

bool game_over(Game game);
void perform_action(Game* gamePtr, char* command);
void shuffle(Game* gamePtr);
void deal(Game* gamePtr);
void draw(Game* gamePtr);
void display_game(Game game);
void display_board(Board board);
void display_hand(Hand hand);
void display_run(Run run);
void display_card(Card card);
