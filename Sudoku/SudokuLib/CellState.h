#pragma once

/**
 * @brief Represents the state of a Sudoku cell.
 *
 * This enum describes whether a cell on the board is empty, part of the
 * original puzzle (fixed) or filled by the player during the game.
 */
enum class CellState {
    /**
     * @brief The cell contains no value (empty).
     */
    EMPTY,

    /**
     * @brief The cell value is part of the initial puzzle and cannot be changed.
     */
    FIXED,

    /**
     * @brief The cell has been filled in by the player (not part of the initial puzzle).
     */
    FILLED
};
