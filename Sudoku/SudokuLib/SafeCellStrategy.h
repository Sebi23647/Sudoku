#pragma once
#include "IHintStrategy.h"
#include "HintContext.h"

class SafeCellStrategy : public IHintStrategy {
public:
    // Fallback: first return candidates (soft), next call can decide to reveal value via solution
    std::optional<Hint> getHint(const HintContext& ctx) const override {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                    const auto& candidates = ctx.candidatesAt(r, c);
                    if (candidates.size() == 1) {
                        Hint h; h.row = r; h.col = c; h.type = HintType::SafeCell; h.candidates = candidates;
                        h.value = candidates[0]; // reveal definite value
                        h.message = "Celula sigura (unic candidat): valoare sugerata " + std::to_string(h.value) + " la (" + std::to_string(r) + "," + std::to_string(c) + ")";
                        return h;
                    }
                    else if (candidates.size() == 2) {
                        Hint h; h.row = r; h.col = c; h.type = HintType::SafeCell; h.candidates = candidates;
                        h.message = "Celula cu 2 candidati la (" + std::to_string(r) + "," + std::to_string(c) + "): " + std::to_string(candidates[0]) + ", " + std::to_string(candidates[1]);
                        return h;
                    }
                }
            }
        }
        return std::nullopt;
    }
};