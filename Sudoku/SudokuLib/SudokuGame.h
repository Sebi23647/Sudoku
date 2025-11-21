#ifndef SUDOKUGAME_H
#define SUDOKUGAME_H

#include "ISudokuGame.h"
#include "Difficulty.h"
#include "CellState.h"
#include <list>
#include <chrono>

class SudokuGame : public ISudokuGame {
private:
    int board[9][9];
    int solutionBoard[9][9];
    bool initialCells[9][9];
    Difficulty currentDifficulty;
    int remainingAttempts;
    std::chrono::steady_clock::time_point startTime;
    bool timerRunning;
    int elapsedSeconds;
    std::list<IObserver*> observers;

    void fillBoard();
    bool fillCell(int pos);
    void removeCells();
    bool isSafe(int row, int col, int num) const;
    void saveSolution();
    bool isValidPosition(int row, int col) const;

    void notifyBoardChanged();
    void notifyGameComplete();
    void notifyAttemptsChanged();

public:
    SudokuGame();
    SudokuGame(Difficulty difficulty);

    void startNewGame() override;
    void startNewGame(Difficulty difficulty) override;
    void generatePuzzle() override;
    bool setValue(int row, int col, int value) override;
    bool isValidMove(int row, int col, int value) const override;
    int getValue(int row, int col) const override;
    CellState getCellState(int row, int col) const override;
    bool isComplete() const override;
    int getRemainingAttempts() const override;
    Difficulty getCurrentDifficulty() const override;
    void reset() override;

    void startTimer();
    void stopTimer();
    void resetTimer();
    int getElapsedTime() const;

    void attachObserver(IObserver* observer) override;
    void detachObserver(IObserver* observer) override;
};

#endif