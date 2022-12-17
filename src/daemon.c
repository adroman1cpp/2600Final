#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main()
{
    
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);

    reciever();

    return 0;
}


void reciever()
{
    while (1)
    {
        
        char* start_game = getenv("START_GAME");

        //if that runs the game 
        if (start_game != NULL) {
            // Starts the game
            system("./tictactoe");

            setenv("START_GAME", "", 1); //resets the variable
        } else { //else that keeps on checking
            
            sleep(20);
        }
    }
}