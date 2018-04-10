//
// Created by Andrew Loftus on 4/8/2018.
//

#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>

// C++ now includes a type for complex numbers std::complex which could also be used.
// For this assignment, this is good enough

class board {
    public:
    //Constructors;
    board() {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                boardArray[r*4+c] = 0;
            }
        }
        Generate();
        Generate();
        score = 0;
        state = 0;
    }
    board(int array[]) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                boardArray[r*4+c] = array[r*4+c];
            }
        }
        score = 0;
        state = 0;
    }

    // Update functions for directional shift
    // These handle scoring, and will set state to 2 if win
    // ShiftLeft is commented for clarity
    void ShiftLeft() {
        for (int r = 0; r < 4; r++) { // Shift the rows one at a time
            for (int z = 0; z < 3; z++) { // This block is a very basic bubble sort that only shifts the 0s to the left
                for (int c = 0; c < 3; c++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 1];
                        boardArray[r * 4 + c + 1] = 0;
                    }
                }
            }

            // This iterates through each pair, starting from the left edge and checks if the pair is equal
            // If the pair is equal, say [x, x] it is replaced by [2x 0] and the score is incremented by 2x
            for (int z = 0; z < 3; z++) {
                if (boardArray[r * 4 + z] == boardArray[r * 4 + 1 + z] && boardArray[r * 4 + z] != 0) {
                    boardArray[r * 4 + z] = 2 * boardArray[r * 4 + z];
                    boardArray[r * 4 + 1 + z] = 0;
                    score += boardArray[r * 4 + z];
                }
            }

            for (int z = 0; z < 3; z++) { // Bubble sort on zero, again
                for (int c = 0; c < 3; c++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 1];
                        boardArray[r * 4 + c + 1] = 0;
                    }
                }
            }
        }
    }
    void ShiftRight() {
        for (int r = 0; r < 4; r++) {
            for (int z = 0; z < 3; z++) {
                for (int c = 0; c < 3; c++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 1];
                        boardArray[r * 4 + c + 1] = 0;
                    }
                }
            }

            for (int z = 3; z > 0; z--) {
                if (boardArray[r * 4 + z] == boardArray[r * 4 - 1 + z] && boardArray[r * 4 + z] != 0) {
                    boardArray[r * 4 + z] = 2 * boardArray[r * 4 + z];
                    boardArray[r * 4 - 1 + z] = 0;
                    score += boardArray[r * 4 + z];
                }
            }

            for (int z = 0; z < 3; z++) {
                for (int c = 3; c > 0; c--) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c - 1];
                        boardArray[r * 4 + c - 1] = 0;
                    }
                }
            }
        }
    }
    void ShiftUp() {
        for (int c = 0; c < 4; c++) {
            for (int z = 0; z < 3; z++) {
                for (int r = 0; r < 3; r++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 4];
                        boardArray[r * 4 + c + 4] = 0;
                    }
                }
            }

            for (int z = 0; z < 12; z += 4) {
                if (boardArray[c + z] == boardArray[c + z + 4] && boardArray[c + z] != 0) {
                    boardArray[c + z] = 2 * boardArray[c + z];
                    boardArray[c + z + 4] = 0;
                    score += boardArray[c + z];
                }
            }

            for (int z = 0; z < 3; z++) {
                for (int r = 0; r < 3; r++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 4];
                        boardArray[r * 4 + c + 4] = 0;
                    }
                }
            }
        }
    }
    void ShiftDown() {
    for (int c = 0; c < 4; c++) {
        for (int z = 0; z < 3; z++) {
            for (int r = 0; r < 3; r++) {
                if (boardArray[r * 4 + c] == 0) {
                    boardArray[r * 4 + c] = boardArray[r * 4 + c + 4];
                    boardArray[r * 4 + c + 4] = 0;
                }
            }
        }

        for (int z = 12; z > 0; z -= 4) {
            if (boardArray[c + z] == boardArray[c + z - 4] && boardArray[c + z] != 0) {
                boardArray[c + z] = 2 * boardArray[c + z];
                boardArray[c + z - 4] = 0;
                score += boardArray[c + z];
            }
        }

        for (int z = 0; z < 3; z++) {
            for (int r = 3; r > 0; r--) {
                if (boardArray[r * 4 + c] == 0) {
                    boardArray[r * 4 + c] = boardArray[r * 4 + c - 4];
                    boardArray[r * 4 + c - 4] = 0;
                }
            }
        }
    }
}

    // Chooses random empty spot and spawns a 2 or 4
    // Returns 0 if no spots were found, 1 otherwise
    int Generate() {
        int index = 0; // Keeps track of the index of emptySpots being added to. Also acts as a counter for # of empty
        int emptySpots[16];
        for (int r = 0; r < 4; r++) { // Iterate through the board, looking for zeros
            for (int c = 0; c < 4; c++) {
                if (boardArray[r*4+c] == 0) {
                    emptySpots[index] = r*4+c;
                    index++;
                }
            }
        }
        if (index == 0) {
            return 0;
        } else {
            int randomSpotIndex = rand() % index;
            int valueIndex = rand() % 2;
            boardArray[emptySpots[randomSpotIndex]] = newValue[valueIndex];
            return 1;
        }
    }

    // Updates state
    // Returns updated state
    int UpdateState() {
        int zeroCheck;
        int pairCheck;
        int maxCheck;
        state = 1;
        for (int r = 0; r < 4; r++) { // Iterate through the board for 2048's and zeros
            for (int c = 0; c < 4; c++) {
                if (boardArray[r*4+c] == 2048) {
                    state = 2;
                    return state;
                } else if (boardArray[r*4+c] == 0 && state != 2) {
                    state = 0;
                }
            }
        }
        if (state == 0) {
            return state;
        }
        for (int r = 0; r < 3; r++) { // Iterate through the board, looking for pairs with the 3x3 as the base value
            for (int c = 0; c < 3; c++) {
                if (boardArray[r*4+c] == boardArray[r*4+c+4] || boardArray[r*4+c] == boardArray[r*4+c+1]) {
                    state = 0;
                    return state;
                }
            }
        }
        for (int r = 0; r < 3; r++) { // Pair iteration for the right column
            int c = 3;
            if (boardArray[r*4+c] == boardArray[r*4+c+4]) {
                state = 0;
                return state;
            }
        }
        for (int c = 0; c < 3; c++) { // Pair iteration for the bottom row
            int r = 3;
            if (boardArray[r*4+c] == boardArray[r*4+c+1]) {
                state = 0;
                return state;
            }
        }
        state = 1;
        return state;
    }

    // Getters
    int GetBoard() const {
        return *boardArray;
    }
    int GetScore() const {
        return score;
    }
    int GetGameState()  const {
        return state;
    }
    int GetValue(int r, int c)  const {
        return boardArray[r*4 + c];
    }

    // Setters
    void SetValue(int r, int c, int value) {
        boardArray[r*4 + c] = value;
    }

    //Print
    void Print() const {
        for (int r = 0; r < 4; r++) {
            std::cout << boardArray[r*4 + 0] << " " << boardArray[r*4 + 1] << " "
                 << boardArray[r*4 + 2] << " "<< boardArray[r*4 + 3] << std::endl;
        }
        std::cout << "\n";
    }
// Data Members
public:
    int boardArray[16]; // Array holding the board, index with: (row * 4) + col
    int score; // When blocks are combined, score += value of new combined block
    int state; // 0 is ongoing, 1 is loss, 2 is win
    const int newValue[2] = {2, 4}; // Array holding two possible new squares
};
