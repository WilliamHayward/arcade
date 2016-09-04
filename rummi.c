#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


/**
 * Each bit of a Rummi tile (from least signicant)
 * 0-3 = Number value. 0 for joker, 1-13 numbers.
 * 4-5 = Suit. 00 Red, 01 Blue, 10 Green, 11 Yellow
 * 6 = Is joker
 * 7 = Played by current player
 */

//TODO: Most of the constants here are "Max Value" type situations

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

char* get_input();
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

int main(int argc, char** argv) {
    Game game;
    Game* gamePtr = &game;
    game.player[0].name = malloc(14);
    game.player[0].name = "Marie";

    game.player[1].name = malloc(14);
    game.player[1].name = "Alice";
    
    game.loose.size = 0;
    game.board.runs = 0;
    shuffle(gamePtr);
    deal(gamePtr);
    char* input;
    int player;
    do {
        display_game(game);
        player = game.currentPlayer;
        printf("%s: ", game.player[player].name);
        input = get_input();
        perform_action(gamePtr, input);
    } while(!game_over(game));
    free(input);
}

void add_run(Board* boardPtr) {
    Board board = *boardPtr;
    new_run(board.run[board.runs]);
    board.runs++;
    *boardPtr = board;
}

void new_run(Run* runPtr) {
    Run run = *runPtr;
    run.size = 0;
    int i;
    for (i = 0; i < 13; i++) {
        run.card[i] = 15;
    }
    *runPtr = run;
}

void shuffle(Game* gamePtr) {
    Game game = *gamePtr;
    int card, suit, value, pos;
    for (pos = 0; pos < 106; pos++) {
        game.deck.card[pos] = -1;
    }

    for (value = 0; value < 2; value++) {
        card = 0;
        card |= 1 << 6;
        do {
            pos = rand() % 106;
        } while (game.deck.card[pos] != -1);
        game.deck.card[pos] = card;
        fflush(stdout);
    }
     
    for (suit = 0; suit < 4; suit++) {
        for (value = 0; value < 26; value++) {
            card = 0;
            card |= value % 13 + 1;
            card |= suit << 4;
            do {
                pos = rand() % 106;
            } while (game.deck.card[pos] != -1);
            game.deck.card[pos] = card;
            fflush(stdout);
        }
    }

    game.deck.position = 0;
    game.deck.exhausted = false;
    *gamePtr = game;
}

void deal(Game* gamePtr) {
    Game game = *gamePtr;
    
    int i, k;
    for (i = 0; i < 2; i++) {
        game.player[i].hand.size = 0;
    }
    
    for (k = 0; k < 28; k++) {
        draw(&game);
    }
    printf("\n");

    *gamePtr = game;
}

char* get_input() {
    char* input = malloc(70);
    int next;
    int pos = 0;
    do {
        next = fgetc(stdin);
        if(next == EOF) {
            exit(0);
        }
        input[pos] = next;
        pos++;
        if(pos > 70) {
            return "";
        }
    } while(next != '\n');
    input[pos] = '\0';
    return input;
}

void help() {
    
}

void draw(Game* gamePtr) {
    Game game = *gamePtr;
    if(game.deck.exhausted) {
        printf("Deck exhausted");
        return;
    } else {

    int player = game.currentPlayer;
    int handPos = game.player[player].hand.size;
    game.player[player].hand.size++; // Store size then increment

    int deckPos = game.deck.position; //Store position then increment
    game.deck.position++;
    int card = game.deck.card[deckPos];
    game.player[player].hand.card[handPos] = card;
    
    end_turn(&game);

    *gamePtr = game;
    }
}

void display_game(Game game) {
    int i;
    for(i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n");
    display_board(game.board);
    printf("Loose cards - ");
    display_hand(game.loose);
    for (i = 0; i < 2; i++) {
        printf("%s's hand - ", game.player[i].name);
        display_hand(game.player[i].hand);
    }
}

void display_board(Board board) {
    int i;
    char* label = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";
    for (i = 0; i < board.runs; i++) {
        printf("%c - ", label[i]);
        display_run(board.run[i]);
    }
}

void display_hand(Hand hand) {
    int i;
    for (i = 0; i < hand.size; i++) {
        if (i != 0) {
            printf(", ");
        }
        display_card(hand.card[i]);
    }
    printf("\n");
}

void display_run(Run run) {
    int i;
    for (i = 0; i < 13; i++) {
        if (run.card[i] == 15) {
            break;
        }
        display_card(run.card[i]);
    }
    printf("\n");
}

void display_card(int card) {
    int suit, value, joker;
    suit = (card >> 4) & 3;
    value = card & 15;
    joker = card >> 6 & 1;

    char pre = '[';
    char post = ']';
    char jokerChar = '\0'; 
    char* color = malloc(70);
    if (joker) {
        jokerChar = 'J';
    }

    switch (suit) {
        case 0:
            color = KMAG;
            break;
        case 1:
            color = KGRN;
            break;
        case 2:
            color = KBLU;
            break;
        case 3:
            color = KYEL;
            break;
    }
    
    if(value == 0) {
        printf("%s%c%c%c%s", color, pre, jokerChar, post, KWHT);
    } else {
        printf("%s%c%c%i%c%s", color, pre, jokerChar, value, post, KWHT);
    }
}

void play(Game* gamePtr, char* card) {
    
}

void withdraw(Game* gamePtr, char* card) {
    
}

void add(Game* gamePtr, char* run) {
    
}

void split(Game* gamePtr, char* run, char* card) {

}

void merge(Game* gamePtr, char* origin, char* destination) {

}

void commit(Game* gamePtr) {

}

void end_turn(Game* gamePtr) {
    Game game = *gamePtr;
    game.currentPlayer = 1 - game.currentPlayer;
    *gamePtr = game;
}

bool strequal(char* a, char* b) {
    return strcmp(a, b) == 0;
}

void perform_action(Game* gamePtr, char* command) {
    char* action = malloc(70);
    char* arg1 = malloc(70);
    char* arg2 = malloc(70);
    int numAssigned = sscanf(command, "%s %s %s", action, arg1, arg2);
    //TODO: Lower Case!
    if (strequal(action, "help")) {
        if (numAssigned != 1) {
            printf("Usage: help takes no arguments\n");
        } else {
            help();
        }
    } else if (strequal(action, "draw")) {
        if (numAssigned != 1) {
            printf("Usage: draw takes no arguments\n");
        } else {
            draw(gamePtr);
        }     
    } else if (strequal(action, "sort")) {
        if (numAssigned != 2) {
            printf("Usage: sort takes no arguments\n");
        } else {
            
        }
    } else if (strequal(action, "play")) {
        if (numAssigned != 2) {
            printf("Usage: play [card]\n");
        } else {

        }
    } else if (strequal(action, "withdraw")) {
        if (numAssigned != 2) {
            printf("Usage: withdraw [card]\n");
        } else {

        }
    } else if (strequal(action, "add")) {
        if (numAssigned != 3) {
            printf("Usage: add [run] [card]\n");
        } else {

        }
    } else if (strequal(action, "split")) {
        if (numAssigned != 3) {
            printf("Usage: split [run] [card]\n");
        } else {

        }
    } else if (strequal(action, "merge")) {
        if (numAssigned != 3) {
            printf("Usage: merge [run] [run]\n");
        } else {

        }
    } else if (strequal(action, "commit")) {
         if (numAssigned != 1) {
            printf("Usage: commit takes no arguments\n");
         } else {

         }
    } else if (strequal(action, "revert")) {
        if (numAssigned != 1) {
            printf("Usage: revert takes no arguments");
        } else {

        }
    } else if (strequal(action, "undo")) {
        if (numAssigned != 1) {
            printf("Usage: undo takes no arguments");
        } else {
            
        }
    } else {
        printf("Invalid command '%s'. Enter 'help' for list of commands\n", 
                action);
    }
}

/**
 * Checks if any player has no remaining cards
 */

bool game_over(Game game) {
    return false;
}

/**
 * Board Example
 *    0  1  2  3  4
 * A  5  6  7
 * B  
 * C
 * D
 * E
 * F
 */

/**
 * Valid actions
 * Play (Card)
 * - Puts a card from hand to play
 * Withdraw (Card)
 * - Pick up played card
 * Split (Run) (Card)
 * - Splits run at card
 * Draw
 * - Put card from deck into hand
 * - Cannot be done if any cards played from hand
 * - Ends turn
 * Add (Card) (Run)
 * - Add card to start/end of run
 * -- Automatically determine if start/end of run
 * -- Sort by suit, then value
 * Submit
 * - Submit board state
 * - If not valid board state, does not work
 */
