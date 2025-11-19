#pragma once

#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"

class ISudokuGame {
public:
    virtual ~ISudokuGame() = default;

    // Game control
    virtual void startNewGame() = 0;
    virtual void startNewGame(Difficulty difficulty) = 0;
    virtual void reset() = 0;
    
    // Board manipulation
    virtual bool setValue(int row, int col, int value) = 0;
    virtual int getValue(int row, int col) const = 0;
    virtual CellState getCellState(int row, int col) const = 0;
    
    // Game state
    virtual bool isComplete() const = 0;
    virtual int getRemainingAttempts() const = 0;
    virtual Difficulty getCurrentDifficulty() const = 0;
    
    // Observer pattern
    virtual void attachObserver(IObserver* observer) = 0;
    virtual void detachObserver(IObserver* observer) = 0;
};

