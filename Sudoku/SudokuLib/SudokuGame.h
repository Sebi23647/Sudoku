#pragma once

#include "ISudokuGame.h"
#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"
#include <list>
#include <vector>
#include <chrono>

class SudokuGame : public ISudokuGame {
private:
    int board[9][9];
    int solutionBoard[9][9];
    bool initialCells[9][9];
    Difficulty currentDifficulty;
    int remainingAttempts;
    std::list<IObserver*> observers;

    std::chrono::steady_clock::time_point startTime;
    bool timerRunning;
    int elapsedSeconds;

    void generatePuzzle();
    void fillBoard();
    bool fillCell(int pos);
    void saveSolution();
    void removeCells();
    bool isSafe(int row, int col, int num) const;
    bool isValidPosition(int row, int col) const;
    bool isValidMove(int row, int col, int value) const;

    int countSolutions(int board[9][9], int count = 0);
    bool hasUniqueSolution(int testBoard[9][9]);
    bool solveSudoku(int board[9][9], int pos, int& solutionCount, int limit);

    void notifyBoardChanged();
    void notifyGameComplete();
    void notifyAttemptsChanged();

    void startTimer();
    void stopTimer();
    void resetTimer();

public:
    SudokuGame();
    explicit SudokuGame(Difficulty difficulty);

    void startNewGame() override;
    void startNewGame(Difficulty difficulty) override;
    bool setValue(int row, int col, int value) override;
    int getValue(int row, int col) const override;
    CellState getCellState(int row, int col) const override;
    bool isComplete() const override;
    int getRemainingAttempts() const override;
    Difficulty getCurrentDifficulty() const override;
    void reset() override;
    int getElapsedTime() const override;

    void attachObserver(IObserver* observer) override;
    void detachObserver(IObserver* observer) override;
};