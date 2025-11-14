#pragma once;
#include "ISudokuGame.h"
#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"
#include <vector>
#include <list>

class SudokuGame : public ISudokuGame {
private:
    int board[9][9];
    int initialBoard[9][9];
    std::list<IObserver*> observers;
    int remainingAttempts;
    Difficulty currentDifficulty;

    void notifyBoardChanged();
    void notifyGameComplete();
    void notifyAttemptsChanged();
    bool isValidMove(int row, int col, int value);
    void generatePuzzle();
    void fillBoard();
    bool fillCell(int pos);
    void removeCells();
    bool solveSudoku(int pos);
    bool isSafe(int row, int col, int num);

public:
    SudokuGame();
    explicit SudokuGame(Difficulty difficulty);

    void startNewGame() override;
    bool setValue(int row, int col, int value) override;
    int getValue(int row, int col) override;
    bool isComplete() override;
    int getRemainingAttempts() override;
    void reset() override;
    void attachObserver(IObserver* observer) override;
};

