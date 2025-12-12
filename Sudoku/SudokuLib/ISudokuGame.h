#pragma once
#include "Difficulty.h"
#include "CellState.h"
#include "IObserver.h"

/**
 * @brief Abstract interface representing a Sudoku game.
 *
 * Implementations provide game lifecycle management, board manipulation
 * and observer registration for UI or other components to receive updates.
 */
class ISudokuGame {
public:
    virtual ~ISudokuGame() = default;

    /**
     * @brief Start a new game using the current difficulty setting.
     *
     * This resets attempts, timer and generates a new puzzle.
     */
    virtual void startNewGame() = 0;

    /**
     * @brief Start a new game with the specified difficulty.
     *
     * @param difficulty The desired difficulty level for the new game.
     */
    virtual void startNewGame(Difficulty difficulty) = 0;

    /**
     * @brief Generate a new puzzle for the current difficulty without
     * performing other start-up steps.
     */
    virtual void generatePuzzle() = 0;

    /**
     * @brief Attempt to set a value on the board at the given position.
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @param value Value to set (0 to clear, 1-9 to set a digit).
     * @return true if the value was accepted and applied, false otherwise.
     */
    virtual bool setValue(int row, int col, int value) = 0;

    /**
     * @brief Check whether placing a value in the specified cell would be a valid move.
     *
     * This typically checks both the Sudoku rules and any stored solution.
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @param value Candidate value (1-9).
     * @return true if the move is valid, false otherwise.
     */
    virtual bool isValidMove(int row, int col, int value) const = 0;

    /**
     * @brief Retrieve the current value at a board position.
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @return Value at the requested cell (0 if empty), or -1 for invalid positions.
     */
    virtual int getValue(int row, int col) const = 0;

    /**
     * @brief Get the state of the specified cell (empty, fixed or filled).
     *
     * @param row Row index (0-8).
     * @param col Column index (0-8).
     * @return CellState value describing the cell.
     */
    virtual CellState getCellState(int row, int col) const = 0;

    /**
     * @brief Check whether the current board is complete and valid.
     *
     * @return true if the puzzle is solved correctly, false otherwise.
     */
    virtual bool isComplete() const = 0;

    /**
     * @brief Get the number of remaining incorrect attempt allowances.
     *
     * @return Remaining attempts before the game is considered lost or locked.
     */
    virtual int getRemainingAttempts() const = 0;

    /**
     * @brief Get the currently selected difficulty of the game.
     *
     * @return Difficulty enum value currently used when generating puzzles.
     */
    virtual Difficulty getCurrentDifficulty() const = 0;

    /**
     * @brief Reset the current puzzle to its initial state.
     *
     * Clears any filled (non-fixed) cells, resets attempts and may restart the timer.
     */
    virtual void reset() = 0;

    /**
     * @brief Get the elapsed play time in seconds.
     *
     * @return Number of seconds passed since the timer started.
     */
    virtual int getElapsedTime() const = 0;

    /**
     * @brief Attach an observer to receive game events.
     *
     * @param observer Pointer to an object implementing IObserver. Null pointers are ignored.
     */
    virtual void attachObserver(IObserver* observer) = 0;

    /**
     * @brief Detach a previously attached observer.
     *
     * @param observer Observer pointer to remove. Null pointers are ignored.
     */
    virtual void detachObserver(IObserver* observer) = 0;
};