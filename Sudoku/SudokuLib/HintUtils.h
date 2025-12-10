#pragma once
#include <vector>
#include "ISudokuGame.h"
#include "CellState.h"

inline bool isSafeCandidate(const ISudokuGame& game, int r, int c, int n) {
    // Check row & column
    for (int x = 0; x < 9; ++x) {
        if (game.getValue(r, x) == n) return false;
        if (game.getValue(x, c) == n) return false;
    }
    // Check box
    int sr = r - r % 3;
    int sc = c - c % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (game.getValue(sr + i, sc + j) == n) return false;
        }
    }
    return true;
}

inline std::vector<int> computeCandidates(const ISudokuGame& game, int r, int c) {
    std::vector<int> result;
    if (game.getCellState(r, c) != CellState::EMPTY) return result;
    for (int n = 1; n <= 9; ++n) {
        if (isSafeCandidate(game, r, c, n)) result.push_back(n);
    }
    return result;
}
