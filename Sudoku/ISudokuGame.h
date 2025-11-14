#ifndef ISUDOKUGAME_H
#define ISUDOKUGAME_H

#include "IObserver.h"
#include "Difficulty.h"

class ISudokuGame {
public:
    virtual ~ISudokuGame() = default;

    virtual void startNewGame() = 0;
    virtual bool setValue(int row, int col, int value) = 0;
    virtual int getValue(int row, int col) = 0;
    virtual bool isComplete() = 0;
    virtual int getRemainingAttempts() = 0;
    virtual void reset() = 0;
    virtual void attachObserver(IObserver* observer) = 0;
};

#endif