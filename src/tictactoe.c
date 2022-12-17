#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <mosquitto.h>

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define TOPIC "project"
#define QOS 1

#define ROWS 3
#define COLUMNS 3

//array for the grid
char grid[ROWS][COLUMNS];

//function that makes the grid
void gameGrid(){
    int i, j;
    for (i = 0; i < ROWS; i++){
        for (j = 0; j < COLUMNS; j++){
            grid[i][j] = ' ';
        }
    }
}

//function that prints the grid out
void printGrid(){
    int i, j;
    printf("\n");
    for (i = 0; i < ROWS; i++){
        for (j = 0; j < COLUMNS; j++){
            printf(" %c |", grid[i][j]);
        }
        printf("\n");
        for (j = 0; j < COLUMNS; j++){
            printf("---");
        }
        printf("\n");
    }
}

//function that checks to see if a player has won
bool checkWin(char player){
    // check rows
    int i, j;
    for (i = 0; i < ROWS; i++){
        bool win = true;
        for (j = 0; j < COLUMNS; j++){
            if (grid[i][j] != player){
                win = false;
                break;
            }
        }
        if (win){
            return true;
        }
    }

    // check columns
    for (j = 0; j < COLUMNS; j++){
        bool win = true;
        for (i = 0; i < ROWS; i++){
            if (grid[i][j] != player){
                win = false;
                break;
            }
        }
        if (win){
            return true;
        }
    }

    // check diagonals
    if (grid[0][0] == player && grid[1][1] == player && grid[2][2] == player){
        return true;
    }
    if (grid[0][2] == player && grid[1][1] == player && grid[2][0] == player){
        return true;
    }

    return false;
}

//checks for draw
bool checkDraw(){
    int i, j;
    for (i = 0; i < ROWS; i++){
        for (j = 0; j < COLUMNS; j++){
            if (grid[i][j] == ' '){
                return false;
            }
        }
    }
    return true;
}

//function for the player to choose their input
void playerMove(int row, int column, char player){
    if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS){
        printf("Invalid move. Please try again.\n");
        return;
    }
    if (grid[row][column] != ' '){
        printf("That cell is already occupied. Please try again.\n");
        return;
    }

    grid[row][column] = player;
    printGrid();
    if (checkWin(player)){
        printf("Player %c wins!\n", player);
        exit(0);
    }
    if (checkDraw()){
        printf("It's a draw!\n");
        exit(0);
    }
}

//function that connects to mqtt
void on_connect(struct mosquitto *mosq, void *obj, int result){
    printf("Connected to MQTT server.\n");
}

//function that disconnects to mqtt
void on_disconnect(struct mosquitto *mosq, void *obj, int result){
    printf("Disconnected from MQTT server.\n");
}

//function that receives the message
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
    char *payload = message->payload;
    int row = payload[0] - '0';
    int column = payload[1] - '0';
    playerMove(row, column, 'O');
}

//function main where the game is played
int main(int argc, char *argv[]){
    gameGrid();
    printGrid();

    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
    mosquitto_subscribe(mosq, NULL, TOPIC, QOS);

    int row, column;
    char player = 'X';

    //whule loop that gets input
    while (true){
        printf("Player %c's turn. Enter row and column: ", player);
        scanf("%d %d", &row, &column);
        playerMove(row, column, player);
        player = (player == 'X') ? 'O' : 'X';

        char move[2];
        sprintf(move, "%d%d", row, column);
        mosquitto_publish(mosq, NULL, TOPIC, strlen(move), move, QOS, false);
    }

    //disconnects from mosquitto
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}