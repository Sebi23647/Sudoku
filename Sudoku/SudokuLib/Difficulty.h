#pragma once

/**
 * @brief Enumeration that defines difficulty levels for puzzle generation.
 *
 * The difficulty value influences how many cells are revealed in the
 * generated Sudoku puzzle and potentially the solving complexity.
 */
enum class Difficulty {
    /**
     * @brief Easiest difficulty level. More cells revealed.
     */
    EASY,

    /**
     * @brief Medium difficulty level. Balanced number of revealed cells.
     */
    MEDIUM,

    /**
     * @brief Hard difficulty level. Fewer cells revealed and more challenging.
     */
    HARD
};

