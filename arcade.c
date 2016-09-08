#include "arcade.h"
#include <string.h>
int main(int argc, char** argv) {
    pid_t pid;
    pid = fork(); 
    if (pid == 0) { // Child
        printf("Child %s\n", argv[1]);
        char* game = strcat
        execlp(argv[1], argv[1], NULL); 
    } else { // Parent
        printf("Parent\n"); 
    }
    return 0;
}
