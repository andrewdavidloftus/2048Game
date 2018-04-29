//
// Created by Andrew Loftus on 4/8/2018.
//

#include <iostream>
#include "board.h"

int main() {
    // Small demo
    int initialBoard[16] = {0, 2, 2, 4,
                            4, 2, 2, 0,
                            4, 2, 2, 4,
                            4, 2, 4, 4};
    board gameBoard(initialBoard);
    gameBoard.ShiftDown();
    gameBoard.Print(2);
    /*
    board gameBoard(initialBoard);
    std::cout << "Demo of shifting" << std::endl;
    gameBoard.Print();
    std::cout << "Shift Left" << std::endl;
    gameBoard.ShiftLeft();
    gameBoard.Print();
    std::cout << "Shift Up" << std::endl;
    gameBoard.ShiftUp();
    gameBoard.Print();
    std::cout << "Shift Right" << std::endl;
    gameBoard.ShiftRight();
    gameBoard.Print();


    std::cout << "Demo of state updates" << std::endl;
    std::cout << "State: " << gameBoard.UpdateState() << std::endl;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            gameBoard.SetValue(r, c, r*4+c+1);
        }
    }
    gameBoard.Print();
    std::cout << "State: " << gameBoard.UpdateState() << std::endl;

    gameBoard.SetValue(1, 1, 2048);
    gameBoard.Print();
    std::cout << "State: " << gameBoard.UpdateState() << std::endl;
     */
    return 0;
}
