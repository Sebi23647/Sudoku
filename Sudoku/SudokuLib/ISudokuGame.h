#ifndef ISUDOKUGAME_H
#define ISUDOKUGAME_H

#include "Difficulty.h"
#include "CellState.h"
#include "IObserver.h"

class ISudokuGame {
public:
    virtual ~ISudokuGame() = default;

    virtual void startNewGame() = 0;
    virtual void startNewGame(Difficulty difficulty) = 0;
    virtual void generatePuzzle() = 0;
    virtual bool setValue(int row, int col, int value) = 0;
    virtual bool isValidMove(int row, int col, int value) const = 0;
    virtual int getValue(int row, int col) const = 0;
    virtual CellState getCellState(int row, int col) const = 0;
    virtual bool isComplete() const = 0;
    virtual int getRemainingAttempts() const = 0;
    virtual int getElapsedTime() const = 0;
    virtual Difficulty getCurrentDifficulty() const = 0;
    virtual void reset() = 0;

    virtual void attachObserver(IObserver* observer) = 0;
    virtual void detachObserver(IObserver* observer) = 0;
};

#endif