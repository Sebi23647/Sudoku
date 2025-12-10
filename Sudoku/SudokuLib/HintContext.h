#pragma once
#include <vector>
#include "ISudokuGame.h"
#include "CellState.h"

// Snapshot / precomputed context passed to hint strategies.
// candidates is a vector of length 81; index = r*9 + c
/**
 * @brief Snapshot and precomputed candidate context for hint strategies.
 *
 * HintContext computes candidate lists for every empty cell once and
 * provides read-only access to the ISudokuGame required by strategies.
 */
struct HintContext {
    /** @brief Reference to the game being analyzed (read-only). */
    const ISudokuGame& game;

    /**
     * @brief Precomputed candidate lists for each cell.
     *
     * This is a vector of length 81 where index = row*9 + col. Empty cells
     * contain a vector of possible candidates (1-9). Non-empty cells have an
     * empty candidate vector.
     */
    std::vector<std::vector<int>> candidates; // 81 entries, each a vector<int>

    /**
     * @brief When true strategies are allowed to consult the stored solution
     * for stronger hints. Use with care; typically false for normal hints.
     */
    bool revealSolution = false; // whether strategies are allowed to read the stored solution

    /**
     * @brief Construct a HintContext for the provided game and options.
     *
     * The constructor precomputes candidate lists for all cells.
     *
     * @param g Reference to the ISudokuGame instance to analyze.
     * @param reveal If true strategies may read the stored solution where available.
     */
    HintContext(const ISudokuGame& g, bool reveal = false)
        : game(g), candidates(81), revealSolution(reveal) {
        computeAllCandidates();
    }

    /**
     * @brief Get the precomputed candidates for the cell at (r,c).
     *
     * @param r Row index (0-8).
     * @param c Column index (0-8).
     * @return const reference to the vector<int> of candidates (possibly empty).
     */
    const std::vector<int>& candidatesAt(int r, int c) const {
        return candidates[r * 9 + c];
    }

private:
    /**
     * @brief Helper to check whether a cell is currently empty.
     *
     * @param r Row index.
     * @param c Column index.
     * @return true if the cell state is CellState::EMPTY.
     */
    inline bool isCellEmpty(int r, int c) const {
        return game.getCellState(r, c) == CellState::EMPTY;
    }

    /**
     * @brief Check whether n is a safe candidate for cell (r,c) given current board.
     *
     * This performs row/column/3x3 block checks using game.getValue().
     *
     * @param r Row index.
     * @param c Column index.
     * @param n Candidate digit to test (1-9).
     * @return true if n is not present in the row/column/block, false otherwise.
     */
    inline bool isSafeCandidate(int r, int c, int n) const {
        // use game.getValue for row/col/box checks; read-only
        for (int x = 0; x < 9; ++x) {
            if (game.getValue(r, x) == n) return false;
            if (game.getValue(x, c) == n) return false;
        }
        int sr = r - r % 3, sc = c - c % 3;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (game.getValue(sr + i, sc + j) == n) return false;
        return true;
    }

    /**
     * @brief Compute candidate lists for all cells and store them in candidates.
     *
     * This examines each empty cell and populates its candidates vector with
     * digits 1-9 that pass isSafeCandidate(). Non-empty cells receive an empty list.
     */
    void computeAllCandidates() {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                auto& v = candidates[r * 9 + c];
                v.clear();
                if (!isCellEmpty(r, c)) continue;
                for (int n = 1; n <= 9; ++n) {
                    if (isSafeCandidate(r, c, n)) v.push_back(n);
                }
            }
        }
    }
};