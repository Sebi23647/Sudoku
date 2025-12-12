#pragma once

#include "ISudokuGame.h"
#include "IObserver.h"
#include "Difficulty.h"
#include "CellState.h"
#include "SudokuBoard.h"
#include "SudokuGenerator.h"
#include "GameTimer.h"
#include <list>

/**
 * @brief Concrete implementation of ISudokuGame that manages Sudoku gameplay.
 *
 * SudokuGame holds the board state, a generator for creating puzzles, a
 * timer, and a list of observers that are notified about game events. It
 * enforces game rules and attempt limits.
 */
class SudokuGame : public ISudokuGame {
private:
    /**
     * @brief The Sudoku board storing current values and metadata.
     */
    SudokuBoard board;

    /**
     * @brief Generator used to produce puzzles according to difficulty.
     */
    SudokuGenerator generator;

    /**
     * @brief Current difficulty used for puzzle generation.
     */
    Difficulty currentDifficulty;

    /**
     * @brief Number of remaining allowed incorrect attempts.
     */
    int remainingAttempts;

    /**
     * @brief Observers registered to receive game updates (UI, etc.).
     */
    std::list<IObserver*> observers;

    /**
     * @brief Timer tracking the elapsed play time for the current game.
     */
    GameTimer timer;

    /**
     * @brief Notify observers that the board has changed.
     */
    void notifyBoardChanged();

    /**
     * @brief Notify observers that the game has been completed successfully.
     */
    void notifyGameComplete();

    /**
     * @brief Notify observers that the number of remaining attempts changed.
     */
    void notifyAttemptsChanged();

    /**
     * @brief Check whether placing num at (row, col) violates Sudoku rules.
     *
     * This checks row, column and 3x3 block constraints ignoring the cell itself.
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @param num Candidate number (1-9).
     * @return true if the placement would not violate rules, false otherwise.
     */
    bool isSafe(int row, int col, int num) const;

    /**
     * @brief Validate that the given (row, col) indices fall inside the board.
     *
     * @param row Row index to validate.
     * @param col Column index to validate.
     * @return true if indices are within [0,8], false otherwise.
     */
    bool isValidPosition(int row, int col) const;

public:
    /**
     * @brief Construct a SudokuGame using the default difficulty (MEDIUM).
     */
    SudokuGame();

    /**
     * @brief Construct a SudokuGame with the specified difficulty.
     *
     * @param difficulty Difficulty level to use for generated puzzles.
     */
    explicit SudokuGame(Difficulty difficulty);

    /**
     * @copydoc ISudokuGame::startNewGame()
     */
    void startNewGame() override;

    /**
     * @copydoc ISudokuGame::startNewGame(Difficulty)
     */
    void startNewGame(Difficulty difficulty) override;

    /**
     * @copydoc ISudokuGame::generatePuzzle()
     */
    void generatePuzzle() override;

    /**
     * @copydoc ISudokuGame::setValue(int,int,int)
     */
    bool setValue(int row, int col, int value) override;

    /**
     * @copydoc ISudokuGame::isValidMove(int,int,int)
     */
    bool isValidMove(int row, int col, int value) const override;

    /**
     * @copydoc ISudokuGame::getValue(int,int)
     */
    int getValue(int row, int col) const override;

    /**
     * @copydoc ISudokuGame::getCellState(int,int)
     */
    CellState getCellState(int row, int col) const override;

    /**
     * @copydoc ISudokuGame::isComplete()
     */
    bool isComplete() const override;

    /**
     * @copydoc ISudokuGame::getRemainingAttempts()
     */
    int getRemainingAttempts() const override;

    /**
     * @copydoc ISudokuGame::getCurrentDifficulty()
     */
    Difficulty getCurrentDifficulty() const override;

    /**
     * @copydoc ISudokuGame::reset()
     */
    void reset() override;

    /**
     * @copydoc ISudokuGame::getElapsedTime()
     */
    int getElapsedTime() const override;

    /**
     * @copydoc ISudokuGame::attachObserver(IObserver*)
     */
    void attachObserver(IObserver* observer) override;

    /**
     * @copydoc ISudokuGame::detachObserver(IObserver*)
     */
    void detachObserver(IObserver* observer) override;

    /**
     * @brief Get the solution value for a given cell from the stored solution.
     *
     * Useful for hinting and test assertions. Returns 0 for invalid indices.
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @return Correct value for the cell (1-9) or 0 if position invalid.
     */
    int getSolutionValue(int row, int col) const;
};