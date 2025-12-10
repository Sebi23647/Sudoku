#pragma once
#include <vector>
#include "ISudokuGame.h"
#include "CellState.h"

// Snapshot / precomputed context passed to hint strategies.
// candidates is a vector of length 81; index = r*9 + c
struct HintContext {
    const ISudokuGame& game;
    std::vector<std::vector<int>> candidates; // 81 entries, each a vector<int>
    bool revealSolution = false; // whether strategies are allowed to read the stored solution

    HintContext(const ISudokuGame& g, bool reveal = false)
        : game(g), candidates(81), revealSolution(reveal) {
        computeAllCandidates();
    }

    const std::vector<int>& candidatesAt(int r, int c) const {
        return candidates[r * 9 + c];
    }

private:
    inline bool isCellEmpty(int r, int c) const {
        return game.getCellState(r, c) == CellState::EMPTY;
    }

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