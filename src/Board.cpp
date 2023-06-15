#include <cstdio>
#include <cmath>
#include "Board.h"

//for the print method, converts numbers on the board into X, O, or -   Only accessed by print method
char Board::numToChar(int num){
    switch(num){
        case 1: 
            return 'X';
        case 0:
            return 'O';
        case 2:
            return 'T';
    };
    return '-';
}

//checks if nested mini tic tac toe board is filled
int Board::checkFilled(MovePos move){
    const int boxStartX = (int) 3 * floor(move.x / 3);
    const int boxStartY = (int) 3 * floor(move.y / 3);
    // horizontal 3-in-a-row check
    for (int y = boxStartY; y < boxStartY + 3; ++y){
        if (asFull[y][boxStartX] != -1 &&
            asFull[y][boxStartX] == asFull[y][boxStartX + 1] &&
            asFull[y][boxStartX + 1] == asFull[y][boxStartX + 2]){
                return asFull[y][boxStartX];
        }
    }
    // vertical 3-in-a-row check
    for (int x = boxStartX; x < boxStartX + 3; ++x){
        if (asFull[boxStartY][x] != -1 && 
            asFull[boxStartY][x] == asFull[boxStartY + 1][x] &&
            asFull[boxStartY + 1][x] == asFull[boxStartY + 2][x]) {
                return asFull[boxStartY][x];
        }
    }
    // diagonal 3-in-a-row check
    if (asFull[boxStartY + 1][boxStartX + 1] != -1){
        if ( (asFull[boxStartY][boxStartX] == asFull[boxStartY + 1][boxStartX + 1] &&
                asFull[boxStartY + 1][boxStartX + 1] == asFull[boxStartY + 2][boxStartX + 2]) ||
                (asFull[boxStartY + 2][boxStartX] == asFull[boxStartY + 1][boxStartX + 1] && 
                asFull[boxStartY + 1][boxStartX + 1] == asFull[boxStartY][boxStartX + 2]) ){
                    return asFull[boxStartY + 1][boxStartX + 1];
        }
    }
    //check for no draw, return -1 if there's no draw
    for (int x = boxStartX; x < boxStartX + 3; ++x){
        for (int y = boxStartY; y < boxStartY + 3; ++y){
            if (asFull[y][x] == -1){
                return -1;
            }
        }
    }
    // otherwise, all spaces are full, return 2 representing a draw
    return 2; 
} 

int Board::checkWin(){
    // horizontal 3-in-a-row check
    for (int y = 0; y < 3; ++y){
        if (as3x3[y][0] != -1 &&
            as3x3[y][0] == as3x3[y][1] &&
            as3x3[y][1] == as3x3[y][2]){
                return as3x3[y][0];
        }
    }
    // vertical 3-in-a-row check
    for (int x = 0; x < 3; ++x){
        if (as3x3[0][x] != -1 && 
            as3x3[0][x] == as3x3[1][x] &&
            as3x3[1][x] == as3x3[2][x]) {
                return as3x3[0][x];
        }
    }
    //diagonal 3-in-a-row check
    if (as3x3[1][1] != -1){
        if ( (as3x3[0][0] == as3x3[1][1] &&
                as3x3[1][1] == as3x3[2][2]) ||
                (as3x3[2][0] == as3x3[1][1] && 
                as3x3[1][1] == as3x3[0][2]) ){
                    return as3x3[1][1];
        }
    }
    //check for no draw, return -1 if there's no draw
    for (int x = 0; x < 3; ++x){
        for (int y = 0; y < 3; ++y){
            if (as3x3[y][x] == -1){
                return -1;
            }
        }
    }
    // otherwise, all spaces are full, return 2 representing a draw
    return 2; 
}
//makes a move that changes asFull board, as well as possibly as3x3 if it fills a mini 3x3 box
void Board::move(MovePos move){
    asFull[move.y][move.x] = move.type;
    const int bigXPos = (int) floor(move.x / 3);
    const int bigYPos = (int) floor(move.y / 3);
    //check if the move changed one of the mini box states, reflect change on as3x3
    as3x3[bigYPos][bigXPos] = checkFilled(move);
}

//prints out the entire NTTT + as3x3 rep. in a neat fashion
void Board::print(){
    printf("\n");
    for (int i = 0; i < 9; ++i){
        if (i % 3 == 0) { 
            printf("\n");
            }
        printf("[%c %c %c] [%c %c %c] [%c %c %c]\n",
        numToChar(asFull[i][0]), numToChar(asFull[i][1]), numToChar(asFull[i][2]), 
        numToChar(asFull[i][3]), numToChar(asFull[i][4]), numToChar(asFull[i][5]), 
        numToChar(asFull[i][6]), numToChar(asFull[i][7]), numToChar(asFull[i][8]));
        }
    printf("\n");
    for (int i = 0; i < 3; i++){
        printf("        [%c %c %c]        \n", 
        numToChar(as3x3[i][0]), numToChar(as3x3[i][1]), numToChar(as3x3[i][2]));
        }
    printf("\n");   
}