#pragma once

#include "ISudokuGame.h"
#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"
#include "SudokuBoard.h"
#include "SudokuGenerator.h"
#include "GameTimer.h"
#include <list>

class SudokuGame : public ISudokuGame {
private:
    SudokuBoard board;
    SudokuGenerator generator;
    Difficulty currentDifficulty;
    int remainingAttempts;
    std::list<IObserver*> observers;
    GameTimer timer;

    void notifyBoardChanged();
    void notifyGameComplete();
    void notifyAttemptsChanged();

    bool isSafe(int row, int col, int num) const;
    bool isValidPosition(int row, int col) const;

public:
    SudokuGame();
    explicit SudokuGame(Difficulty difficulty);

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
    int getElapsedTime() const override;

    void attachObserver(IObserver* observer) override;
    void detachObserver(IObserver* observer) override;

    int getSolutionValue(int row, int col) const;
};