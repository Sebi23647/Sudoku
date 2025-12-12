#pragma once
#include "IHintStrategy.h"
#include "HintContext.h"
#include <algorithm>

// Hidden single: finds a number n that can go in exactly one empty cell in a unit.
class HiddenSingleStrategy : public IHintStrategy {
public:
    std::optional<Hint> getHint(const HintContext& ctx) const override {
        // Row-based
        for (int r = 0; r < 9; ++r) {
            for (int n = 1; n <= 9; ++n) {
                int count = 0, lastC = -1;
                for (int c = 0; c < 9; ++c) {
                    if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                        const auto& cand = ctx.candidatesAt(r, c);
                        if (std::find(cand.begin(), cand.end(), n) != cand.end()) {
                            ++count; lastC = c;
                        }
                    }
                }
                if (count == 1) {
                    Hint h; h.row = r; h.col = lastC; h.value = n; h.type = HintType::HiddenSingle;
                    h.message = "Hidden Single (row): cifra " + std::to_string(n) + " poate fi plasata doar la (" + std::to_string(r) + "," + std::to_string(lastC) + ")";
                    return h;
                }
            }
        }

        // Column-based
        for (int c = 0; c < 9; ++c) {
            for (int n = 1; n <= 9; ++n) {
                int count = 0, lastR = -1;
                for (int r = 0; r < 9; ++r) {
                    if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                        const auto& cand = ctx.candidatesAt(r, c);
                        if (std::find(cand.begin(), cand.end(), n) != cand.end()) {
                            ++count; lastR = r;
                        }
                    }
                }
                if (count == 1) {
                    Hint h; h.row = lastR; h.col = c; h.value = n; h.type = HintType::HiddenSingle;
                    h.message = "Hidden Single (col): cifra " + std::to_string(n) + " poate fi plasata doar la (" + std::to_string(lastR) + "," + std::to_string(c) + ")";
                    return h;
                }
            }
        }

        // Box-based
        for (int br = 0; br < 3; ++br) {
            for (int bc = 0; bc < 3; ++bc) {
                int sr = br * 3, sc = bc * 3;
                for (int n = 1; n <= 9; ++n) {
                    int count = 0, lastR = -1, lastC = -1;
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            int r = sr + i, c = sc + j;
                            if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                                const auto& cand = ctx.candidatesAt(r, c);
                                if (std::find(cand.begin(), cand.end(), n) != cand.end()) {
                                    ++count; lastR = r; lastC = c;
                                }
                            }
                        }
                    }
                    if (count == 1) {
                        Hint h; h.row = lastR; h.col = lastC; h.value = n; h.type = HintType::HiddenSingle;
                        h.message = "Hidden Single (box): cifra " + std::to_string(n) + " poate fi plasata doar la (" + std::to_string(lastR) + "," + std::to_string(lastC) + ")";
                        return h;
                    }
                }
            }
        }

        return std::nullopt;
    }
};