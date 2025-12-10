#pragma once
#include "CellState.h"

/**
 * @brief Represents the Sudoku board state including mutable cells, initial cells and the solution.
 *
 * SudokuBoard provides accessor and mutator functions for a 9x9 Sudoku grid, tracks which
 * cells are part of the initial puzzle (fixed) and stores the full solution separately.
 */
class SudokuBoard {
public:
    /**
     * @brief Construct an empty SudokuBoard.
     *
     * Initializes the board to zeros and marks no initial cells.
     */
    SudokuBoard();

    /**
     * @brief Clear the current board values and initial marks.
     *
     * Sets all cells to 0 and clears initial flags. Does not modify the stored solution.
     */
    void Clear();

    /**
     * @brief Get the value at row r and column c.
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return Stored value (0 if empty).
     */
    int Get(int r, int c) const;

    /**
     * @brief Set the value at row r and column c.
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @param v Value to set (0 to clear, 1-9 for digits).
     */
    void Set(int r, int c, int v);

    /**
     * @brief Query whether the cell at (r,c) is part of the initial puzzle (fixed).
     *
     * Fixed cells should not be modified by the player.
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return true if the cell is initial/fixed, false otherwise.
     */
    bool IsInitial(int r, int c) const;

    /**
     * @brief Mark or unmark the given cell as initial (fixed).
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @param value true to mark as initial, false to unmark.
     */
    void MarkInitial(int r, int c, bool value);

    /**
     * @brief Mark or unmark all cells as initial.
     *
     * @param value true to mark all cells initial, false to clear all initial marks.
     */
    void MarkAllInitial(bool value);

    /**
     * @brief Copy the current board values into an output 9x9 array.
     *
     * @param out Preallocated array int[9][9] to receive the values.
     */
    void CopyTo(int out[9][9]) const;

    /**
     * @brief Set the stored solution from a source 9x9 array.
     *
     * The solution is stored separately and can be queried by GetSolution.
     *
     * @param src Source array containing the solved puzzle values.
     */
    void SetSolution(const int src[9][9]);

    /**
     * @brief Get the stored solution value at (r,c).
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return Solution value (1-9) or 0 if not set.
     */
    int GetSolution(int r, int c) const;

    /**
     * @brief Get the CellState for the cell at (r,c).
     *
     * Returns FIXED for initial cells, EMPTY for zero-value cells and FILLED otherwise.
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return CellState enum value describing the cell.
     */
    CellState GetCellState(int r, int c) const;

private:
    /**
     * @brief Current board values (0 means empty).
     */
    int board[9][9];

    /**
     * @brief Flags for cells that belong to the initial puzzle and are fixed.
     */
    bool initialCells[9][9];

    /**
     * @brief Stored complete solution for the puzzle.
     */
    int solution[9][9];
};