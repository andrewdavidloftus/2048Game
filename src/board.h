//
// Created by Andrew Loftus on 4/8/2018.
//

#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <ctime>

// C++ now includes a type for complex numbers std::complex which could also be used.
// For this assignment, this is good enough

typedef struct {
    int endValue;
    int direction; // 0-3, 0 is right, 1 is up, and so on in a cc direction... -1 is invalid (only when board start)
    int distance;
    int destroy;
    int isNew;
    int newVal;
} moveData;

class board {
    public:
    //Constructors;
    board() {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                boardArray[r*4+c] = 0;
            }
        }
        ResetMoveArray(-1);
        Generate();
        Generate();
        score = 0;
        state = 0;
        srand(time(NULL));
    }
    board(int array[]) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                boardArray[r*4+c] = 0;
            }
        }
        ResetMoveArray(-1);
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                boardArray[r*4+c] = array[r*4+c];
                moveArray[r*4+c].isNew = 1;
                moveArray[r*4+c].newVal = array[r*4+c];
            }
        }
        score = 0;
        state = 0;
        srand(time(NULL));
    }

    // Update functions for directional shift
    // These handle scoring, and will set state to 2 if win
    // ShiftLeft is commented for clarity
    void ShiftLeft() {
        // For these comments, a notation of [x x y z] will be used to describe cases. Any variable is non-zero and 0 will
        // represent 0. If something can handle two cases of variable or zero, it will look like [x x y/0 0] which
        // would represent [x x y 0] and [x x 0 0]. X != Y and Y != Z. X may equal Z, but this is irrelevant as Y will
        // be in between as a buffer in this notation.
        int oldArray[16];
        for (int i = 0; i < 16; i++) {
            oldArray[i] = boardArray[i];
        }
        ResetMoveArray(2);
        for (int r = 0; r < 4; r++) { // Shift the rows one at a time
            for (int z = 0; z < 3; z++) { // This block is a very basic bubble sort that only shifts the 0s to the left
                for (int c = 0; c < 3; c++) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c + 1];
                        boardArray[r * 4 + c + 1] = 0;
                        if (((c + 1 + z) < 4) && moveArray[r * 4 + c + 1 + z].endValue != 0) { // Updates to distance shifted
                            moveArray[r * 4 + c + 1 + z].distance += 1;
                        }
                    }
                }
            }

            // This goes through each pair, starting from the left edge and checks if the pair is equal
            // If the pair is equal, say [x, x] it is replaced by [2x 0] and the score is incremented by 2x
            if (boardArray[r * 4] == boardArray[r * 4 + 1] && boardArray[r * 4] != 0) {
                boardArray[r * 4] = 2 * boardArray[r * 4];
                boardArray[r * 4 + 1] = 0;
                score += boardArray[r * 4];
                if (moveArray[r * 4].endValue == 0 && moveArray[r * 4 + 1].endValue == 0) { // [0 0 x x]
                    moveArray[r * 4 + 3].destroy = 1;
                    moveArray[r * 4 + 3].distance += 1;
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4];
                    moveArray[r * 4 + 3].endValue = boardArray[r * 4];
                } else if (moveArray[r * 4 + 3].distance == 2) { // This handles [0 x 0 x] and [x 0 0 x]
                    moveArray[r * 4 + 3].destroy = 1;
                    moveArray[r * 4 + 3].distance += 1;
                    moveArray[r * 4 + 3].endValue = boardArray[r * 4];
                    if (moveArray[r * 4 + 1].endValue != 0) {
                        moveArray[r * 4 + 1].endValue = boardArray[r * 4]; // [0 x 0 x]
                    } else {
                        moveArray[r * 4].endValue = boardArray[r * 4]; // [x 0 0 x]
                    }
                } else if (moveArray[r * 4 + 2].distance == 1) { // This handles [x 0 x y/0] and [0 x x y/0]
                    moveArray[r * 4 + 2].destroy = 1;
                    moveArray[r * 4 + 2].distance += 1;
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4];
                    if (moveArray[r * 4 + 1].endValue != 0) { // [0 x x y/0]
                        moveArray[r * 4 + 1].endValue = boardArray[r * 4];
                    } else { // [x 0 x y/0]
                        moveArray[r * 4].endValue = boardArray[r * 4];
                    }
                    if (moveArray[r * 4 + 3].endValue != 0) { // Handles if the last value is y (non-zero)
                        boardArray[r * 4 + 1] = boardArray[r * 4 + 2];
                        boardArray[r * 4 + 2] = 0;
                        moveArray[r * 4 + 3].distance += 1;
                    }
                } else { // This handles [x x 0 0], [x x y 0], [x x 0 y], [x x y z], and [x x y y]
                    moveArray[r * 4].endValue = boardArray[r * 4];
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4];
                    moveArray[r * 4 + 1].distance += 1;
                    moveArray[r * 4 + 1].destroy = 1;
                    if (moveArray[r * 4 + 2].endValue == 0) { // [x x 0 0], [x x y 0], [x x 0 y]
                        if (moveArray[r * 4 + 3].endValue != 0) { // [x x 0 y]
                            moveArray[r * 4 + 3].distance += 1;
                            boardArray[r * 4 + 1] = boardArray[r * 4 + 2];
                            boardArray[r * 4 + 2] = 0;
                        }
                    } else if (moveArray[r * 4 + 3].endValue == 0) { // [x x y 0]
                        moveArray[r * 4 + 2].distance += 1;
                        boardArray[r * 4 + 1] = boardArray[r * 4 + 2];
                        boardArray[r * 4 + 2] = 0;
                    } else if (boardArray[r * 4 + 2] != boardArray[r * 4 + 3]) { // [x x y z]
                        moveArray[r * 4 + 2].distance += 1;
                        moveArray[r * 4 + 3].distance += 1;
                        boardArray[r * 4 + 1] = boardArray[r * 4 + 2];
                        boardArray[r * 4 + 2] = boardArray[r * 4 + 3];
                        boardArray[r * 4 + 3] = 0;
                    } else { // [x x y y]
                        boardArray[r * 4 + 1] = 2 * boardArray[r * 4 + 2];
                        boardArray[r * 4 + 2] = 0;
                        boardArray[r * 4 + 3] = 0;
                        score += boardArray[r * 4 + 1];
                        moveArray[r * 4].endValue = boardArray[r * 4];
                        moveArray[r * 4 + 1].endValue = boardArray[r * 4];
                        moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 1];
                        moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 1];
                        moveArray[r * 4 + 1].distance = 1;
                        moveArray[r * 4 + 2].distance = 1;
                        moveArray[r * 4 + 3].distance = 2;
                        moveArray[r * 4 + 1].destroy = 1;
                        moveArray[r * 4 + 3].destroy = 1;
                    }
                }
            } else if (boardArray[r * 4 + 1] == boardArray[r * 4 + 2] && boardArray[r * 4 + 1] != 0) {
                // This handles [x y y 0], [x y y z], and [x y 0 y] and [x 0 y y], [0 x y y]
                boardArray[r * 4 + 1] = 2 * boardArray[r * 4 + 1];
                boardArray[r * 4 + 2] = 0;
                score += boardArray[r * 4 + 1];
                if (moveArray[r * 4 + 1].endValue == moveArray[r * 4 + 2].endValue) { // [x y y 0], [x y y z]
                    moveArray[r * 4 + 2].destroy = 1;
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 1];
                    moveArray[r * 4 + 2].distance = 1;
                    if (boardArray[r * 4 + 3] != 0) {
                        moveArray[r * 4 + 3].distance = 1;
                        boardArray[r * 4 + 2] = boardArray[r * 4 + 3];
                        boardArray[r * 4 + 3] = 0;
                    }
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 1];
                } else if (moveArray[r * 4 + 2].endValue == 0) { // [x y 0 y]
                    moveArray[r * 4 + 3].destroy = 1;
                    moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 1];
                    moveArray[r * 4 + 3].distance = 2;
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 1];
                } else { // [x 0 y y], [0 x y y]
                    moveArray[r * 4 + 3].destroy = 1;
                    moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 1];
                    moveArray[r * 4 + 3].distance = 2;
                    moveArray[r * 4 + 2].distance = 1;
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 1];
                }
            } else if (boardArray[r * 4 + 2] == boardArray[r * 4 + 3] && boardArray[r * 4 + 2] != 0) {
                // This handles [x y z z]
                boardArray[r * 4 + 2] = 2 * boardArray[r * 4 + 2];
                boardArray[r * 4 + 3] = 0;
                score += boardArray[r * 4 + 2];
                moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 2];
                moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 2];
                moveArray[r * 4 + 3].distance = 1;
                moveArray[r * 4 + 3].destroy = 1;
            }
        }
        for (int i = 0; i < 16; i++) {
            if (oldArray[i] != boardArray[i]) {
                Generate();
                return;
            }
        }
    }
    void ShiftRight() {
        int oldArray[16];
        for (int i = 0; i < 16; i++) {
            oldArray[i] = boardArray[i];
        }
        ResetMoveArray(0);
        for (int r = 0; r < 4; r++) { // Shift the rows one at a time
            for (int z = 0; z < 3; z++) {
                for (int c = 3; c > 0; c--) {
                    if (boardArray[r * 4 + c] == 0) {
                        boardArray[r * 4 + c] = boardArray[r * 4 + c - 1];
                        boardArray[r * 4 + c - 1] = 0;
                        if (((c - 1 - z) >= 0 && (c - 1 - z) < 4) && moveArray[r * 4 + c - 1 - z].endValue != 0) { // Updates to distance shifted
                            moveArray[r * 4 + c - 1 - z].distance += 1;
                        }
                    }
                }
            }
            // This goes through each pair, starting from the left edge and checks if the pair is equal
            // If the pair is equal, say [x, x] it is replaced by [2x 0] and the score is incremented by 2x
            if (boardArray[r * 4 + 3] == boardArray[r * 4 + 2] && boardArray[r * 4 + 3] != 0) {
                boardArray[r * 4 + 3] = 2 * boardArray[r * 4 + 3];
                boardArray[r * 4 + 2] = 0;
                score += boardArray[r * 4 + 3];
                if (moveArray[r * 4 + 3].endValue == 0 && moveArray[r * 4 + 2].endValue == 0) { // [x x 0 0]
                    moveArray[r * 4].destroy = 1;
                    moveArray[r * 4].distance += 1;
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 3];
                    moveArray[r * 4].endValue = boardArray[r * 4 + 3];
                } else if (moveArray[r * 4].distance == 2) { // This handles [0 x 0 x] and [x 0 0 x]
                    moveArray[r * 4].destroy = 1;
                    moveArray[r * 4].distance += 1;
                    moveArray[r * 4].endValue = boardArray[r * 4 + 3];
                    if (moveArray[r * 4 + 2].endValue != 0) {
                        moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 3]; // [0 x 0 x]
                    } else {
                        moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 3]; // [x 0 0 x]
                    }
                } else if (moveArray[r * 4 + 1].distance == 1) { // This handles [x 0 x y/0] and [0 x x y/0]
                    moveArray[r * 4 + 1].destroy = 1;
                    moveArray[r * 4 + 1].distance += 1;
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 3];
                    if (moveArray[r * 4 + 2].endValue != 0) { // [0 x x y/0]
                        moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 3];
                    } else { // [x 0 x y/0]
                        moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 3];
                    }
                    if (moveArray[r * 4].endValue != 0) { // Handles if the last value is y (non-zero)
                        boardArray[r * 4 + 2] = boardArray[r * 4 + 1];
                        boardArray[r * 4 + 1] = 0;
                        moveArray[r * 4].distance += 1;
                    }
                } else { // This handles [x x 0 0], [x x y 0], [x x 0 y], [x x y z], and [x x y y]
                    moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 3];
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 3];
                    moveArray[r * 4 + 2].distance += 1;
                    moveArray[r * 4 + 2].destroy = 1;
                    if (moveArray[r * 4 + 1].endValue == 0) { // [x x 0 0], [x x y 0], [x x 0 y]
                        if (moveArray[r * 4].endValue != 0) { // [x x 0 y]
                            moveArray[r * 4].distance += 1;
                            boardArray[r * 4 + 2] = boardArray[r * 4 + 1];
                            boardArray[r * 4 + 1] = 0;
                        }
                    } else if (moveArray[r * 4].endValue == 0) { // [x x y 0]
                        moveArray[r * 4 + 1].distance += 1;
                        boardArray[r * 4 + 2] = boardArray[r * 4 + 1];
                        boardArray[r * 4 + 1] = 0;
                    } else if (boardArray[r * 4 + 1] != boardArray[r * 4]) { // [x x y z]
                        moveArray[r * 4 + 1].distance += 1;
                        moveArray[r * 4].distance += 1;
                        boardArray[r * 4 + 2] = boardArray[r * 4 + 1];
                        boardArray[r * 4 + 1] = boardArray[r * 4];
                        boardArray[r * 4] = 0;
                    } else { // [x x y y]
                        boardArray[r * 4 + 2] = 2 * boardArray[r * 4 + 1];
                        boardArray[r * 4 + 1] = 0;
                        boardArray[r * 4] = 0;
                        score += boardArray[r * 4 + 2];
                        moveArray[r * 4 + 3].endValue = boardArray[r * 4 + 3];
                        moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 3];
                        moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 2];
                        moveArray[r * 4].endValue = boardArray[r * 4 + 2];
                        moveArray[r * 4 + 2].distance = 1;
                        moveArray[r * 4 + 1].distance = 1;
                        moveArray[r * 4].distance = 2;
                        moveArray[r * 4 + 2].destroy = 1;
                        moveArray[r * 4].destroy = 1;
                    }
                }
            } else if (boardArray[r * 4 + 1] == boardArray[r * 4 + 2] && boardArray[r * 4 + 1] != 0) {
                // This handles [0 y y x], [z y y x], [y y 0 x], [y y x 0], and [y 0 y x]
                boardArray[r * 4 + 2] = 2 * boardArray[r * 4 + 2];
                boardArray[r * 4 + 1] = 0;
                score += boardArray[r * 4 + 2];
                if (moveArray[r * 4 + 2].endValue == moveArray[r * 4 + 1].endValue) { // [0 y y x], [z y y x]
                    moveArray[r * 4 + 1].destroy = 1;
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 2];
                    moveArray[r * 4 + 1].distance = 1;
                    if (boardArray[r * 4] != 0) {
                        moveArray[r * 4].distance = 1;
                        boardArray[r * 4 + 1] = boardArray[r * 4];
                        boardArray[r * 4] = 0;
                    }
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 2];
                } else if (moveArray[r * 4 + 1].endValue == 0) { // [y 0 y x]
                    moveArray[r * 4].destroy = 1;
                    moveArray[r * 4].endValue = boardArray[r * 4 + 2];
                    moveArray[r * 4].distance = 2;
                    moveArray[r * 4 + 2].endValue = boardArray[r * 4 + 2];
                } else { // [y y 0 x], [y y x 0]
                    moveArray[r * 4].destroy = 1;
                    moveArray[r * 4].endValue = boardArray[r * 4 + 2];
                    moveArray[r * 4].distance = 2;
                    moveArray[r * 4 + 1].distance = 1;
                    moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 2];
                }
            } else if (boardArray[r * 4] == boardArray[r * 4 + 1] && boardArray[r * 4 + 1] != 0) {
                // This handles [x y z z]
                boardArray[r * 4 + 1] = 2 * boardArray[r * 4 + 1];
                boardArray[r * 4] = 0;
                score += boardArray[r * 4 + 1];
                moveArray[r * 4 + 1].endValue = boardArray[r * 4 + 1];
                moveArray[r * 4].endValue = boardArray[r * 4 + 1];
                moveArray[r * 4].distance = 1;
                moveArray[r * 4].destroy = 1;
            }
        }
        for (int i = 0; i < 16; i++) {
            if (oldArray[i] != boardArray[i]) {
                Generate();
                return;
            }
        }

    }
    void ShiftUp() {
        std::swap(boardArray[1], boardArray[4]); // Transpose
        std::swap(boardArray[2], boardArray[8]);
        std::swap(boardArray[3], boardArray[12]);
        std::swap(boardArray[6], boardArray[9]);
        std::swap(boardArray[7], boardArray[13]);
        std::swap(boardArray[11], boardArray[14]);
        ShiftLeft();
        std::swap(boardArray[1], boardArray[4]);
        std::swap(boardArray[2], boardArray[8]);
        std::swap(boardArray[3], boardArray[12]);
        std::swap(boardArray[6], boardArray[9]);
        std::swap(boardArray[7], boardArray[13]);
        std::swap(boardArray[11], boardArray[14]);
        std::swap(moveArray[1], moveArray[4]);
        std::swap(moveArray[2], moveArray[8]);
        std::swap(moveArray[3], moveArray[12]);
        std::swap(moveArray[6], moveArray[9]);
        std::swap(moveArray[7], moveArray[13]);
        std::swap(moveArray[11], moveArray[14]);
        for(int i = 0; i < 16; i++) {
            moveArray[i].direction = 1;
        }
    }
    void ShiftDown() {
        std::swap(boardArray[1], boardArray[4]); // Transpose
        std::swap(boardArray[2], boardArray[8]);
        std::swap(boardArray[3], boardArray[12]);
        std::swap(boardArray[6], boardArray[9]);
        std::swap(boardArray[7], boardArray[13]);
        std::swap(boardArray[11], boardArray[14]);
        ShiftRight();
        std::swap(boardArray[1], boardArray[4]);
        std::swap(boardArray[2], boardArray[8]);
        std::swap(boardArray[3], boardArray[12]);
        std::swap(boardArray[6], boardArray[9]);
        std::swap(boardArray[7], boardArray[13]);
        std::swap(boardArray[11], boardArray[14]);
        std::swap(moveArray[1], moveArray[4]);
        std::swap(moveArray[2], moveArray[8]);
        std::swap(moveArray[3], moveArray[12]);
        std::swap(moveArray[6], moveArray[9]);
        std::swap(moveArray[7], moveArray[13]);
        std::swap(moveArray[11], moveArray[14]);
        for(int i = 0; i < 16; i++) {
            moveArray[i].direction = 3;
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
            moveArray[emptySpots[randomSpotIndex]].newVal = newValue[valueIndex];
            moveArray[emptySpots[randomSpotIndex]].isNew = 1;
            return 1;
        }
    }

    // Resets the MoveArray for the next update
    // It will set direction to direction, the endValue to the current board values
    // and everything else is reset to 0
    void ResetMoveArray(int direction) {
        for(int i = 0; i < 16; i++) {
            moveArray[i].endValue = boardArray[i];
            moveArray[i].direction = direction;
            moveArray[i].distance = 0;
            moveArray[i].destroy = 0;
            moveArray[i].isNew = 0;
            moveArray[i].newVal = 0;
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
                if (boardArray[r*4+c] == winValue) {
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
    moveData GetMoveArray() const {
        return *moveArray;
    }
    moveData GetMoveData(int r, int c) const {
        return moveArray[r*4+c];
    }
    int GetScore() const {
        return score;
    }
    int GetGameState() const {
        return state;
    }
    int GetValue(int r, int c)  const {
        return boardArray[r*4 + c];
    }
    int GetWinValue() const {
        return winValue;
    }

    // Setters
    void SetValue(int r, int c, int value) {
        boardArray[r*4 + c] = value;
    }
    void SetWinValue(int w) {
        winValue = w;
    }

    //Print, input type: 0 for print board, 1 for print move, 2 for print both
    void Print(int type) const {
        if (type != 1) {
            std::cout << "Board:" << std::endl;
            for (int r = 0; r < 4; r++) {
                std::cout << boardArray[r * 4 + 0] << " " << boardArray[r * 4 + 1] << " "
                          << boardArray[r * 4 + 2] << " " << boardArray[r * 4 + 3] << std::endl;
            }
            std::cout << std::endl;
        }
        if(type > 0) {
            std::cout << "Move Array" << std::endl;
            std::cout << "Order: endVal, dir, dist, destroy, isNew, newVal" << std::endl;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    std::cout << moveArray[r*4 + c].endValue << "," << moveArray[r*4 + c].direction << ","
                              << moveArray[r*4 + c].distance << "," << moveArray[r*4 + c].destroy << ","
                              << moveArray[r*4 + c].isNew << "," << moveArray[r*4 + c].newVal << "   ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

// Data Members
public:
    int boardArray[16]; // Array holding the board, index with: (row * 4) + col
    moveData moveArray[16]; // Array holding the movement data structures for each position
    int score; // When blocks are combined, score += value of new combined block
    int state; // 0 is ongoing, 1 is loss, 2 is win
    int winValue;
    const int newValue[2] = {2, 4}; // Array holding two possible new squares
};
