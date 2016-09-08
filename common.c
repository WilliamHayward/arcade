#include <common.h>

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
