#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main(){
    
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    receiver();

    return 0;
}

void receiver(){

    while (1) {    
        char* game = getenv("GAME");
        //if that runs the game 
        if (game != NULL) {
            // Starts the game
            system("./tictactoe");
            setenv("GAME", "", 1); //resets the variable
        } else { //else that keeps on checking           
            sleep(20);
        }
    }
}