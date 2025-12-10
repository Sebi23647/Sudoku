
#pragma once
#include "IHintStrategy.h"
#include "HintContext.h"
#include <algorithm>
#include "Hint.h"

// LocalReasoning: simplified local reasoning as an example (non-exhaustive).
class LocalReasoningStrategy : public IHintStrategy {
public:
    std::optional<Hint> getHint(const HintContext& ctx) const override {
        // Find a cell with 2-3 candidates where at least one candidate is restricted in its row/col/box
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                    const auto& cand = ctx.candidatesAt(r, c);
                    if (cand.size() >= 2 && cand.size() <= 3) {
                        // Check if any candidate appears in very few places in the unit (simple heuristic)
                        for (int v : cand) {
                            int rowCount = 0, colCount = 0, boxCount = 0;
                            for (int cc = 0; cc < 9; ++cc) {
                                if (ctx.game.getCellState(r, cc) == CellState::EMPTY) {
                                    const auto& cands = ctx.candidatesAt(r, cc);
                                    if (std::find(cands.begin(), cands.end(), v) != cands.end()) ++rowCount;
                                }
                            }
                            for (int rr = 0; rr < 9; ++rr) {
                                if (ctx.game.getCellState(rr, c) == CellState::EMPTY) {
                                    const auto& cands = ctx.candidatesAt(rr, c);
                                    if (std::find(cands.begin(), cands.end(), v) != cands.end()) ++colCount;
                                }
                            }
                            int sr = r - r % 3, sc = c - c % 3;
                            for (int i = 0; i < 3; ++i)
                                for (int j = 0; j < 3; ++j) {
                                    int rr = sr + i, cc = sc + j;
                                    if (ctx.game.getCellState(rr, cc) == CellState::EMPTY) {
                                        const auto& cands = ctx.candidatesAt(rr, cc);
                                        if (std::find(cands.begin(), cands.end(), v) != cands.end()) ++boxCount;
                                    }
                                }

                            if ((rowCount == 1) || (colCount == 1) || (boxCount == 1)) {
                                Hint h; h.row = r; h.col = c; h.type = HintType::LocalReasoning;
                                h.candidates = cand;
                                h.message = "Rationament local: celula (" + std::to_string(r) + "," + std::to_string(c) + ") are candidati restransi; verifica cifra " + std::to_string(v);
                                return h;
                            }
                        }
                    }
                }
            }
        }
        return std::nullopt;
    }
};