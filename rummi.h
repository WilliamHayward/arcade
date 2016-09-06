#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
