#!/bin/bash

#while loop that runs when game is played
while true; do

    #choose a random row
    row=$((RANDOM % 3 + 1))
    #choose a random column
    column=$((RANDOM % 3 + 1))

    # Make move
    player2Move $row $column
    
done