#pragma once

#include "ISudokuGame.h"
#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"
#include <vector>
#include <list>

class SudokuGame : public ISudokuGame {
private:
    int board[9][9];
    int solutionBoard[9][9];
    bool initialCells[9][9];
    std::list<IObserver*> observers;
    int remainingAttempts;
    Difficulty currentDifficulty;

    // Observer notifications
    void notifyBoardChanged();
    void notifyGameComplete();
    void notifyAttemptsChanged();
    
    // Validation
    bool isValidMove(int row, int col, int value) const;
    bool isSafe(int row, int col, int num) const;
    bool isValidPosition(int row, int col) const;
    
    // Puzzle generation
    void generatePuzzle();
    void fillBoard();
    bool fillCell(int pos);
    void removeCells();
    void saveSolution();

public:
    SudokuGame();
    explicit SudokuGame(Difficulty difficulty);

    // Game control
    void startNewGame() override;
    void startNewGame(Difficulty difficulty) override;
    void reset() override;
    
    // Board manipulation
    bool setValue(int row, int col, int value) override;
    int getValue(int row, int col) const override;
    CellState getCellState(int row, int col) const override;
    
    // Game state
    bool isComplete() const override;
    int getRemainingAttempts() const override;
    Difficulty getCurrentDifficulty() const override;
    
    // Observer pattern
    void attachObserver(IObserver* observer) override;
    void detachObserver(IObserver* observer) override;
};

