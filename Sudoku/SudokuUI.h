#pragma once;

#include "ISudokuGame.h"
#include "IObserver.h"
#include <iostream>

class SudokuUI : public IObserver {
private:
    ISudokuGame* game;

    void displayBoard();
    void displayMenu();
    void displayGameInfo();

public:
    explicit SudokuUI(ISudokuGame* sudokuGame);

    void render();
    void handleClick(int row, int col);

    void onBoardChanged() override;
    void onGameComplete() override;
    void onAttemptsChanged(int remaining) override;
};

