#pragma once
#include "IHintStrategy.h"
#include "HintContext.h"

class CandidateListStrategy : public IHintStrategy {
public:
    std::optional<Hint> getHint(const HintContext& ctx) const override {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                    const auto& candidates = ctx.candidatesAt(r, c);
                    if (!candidates.empty() && candidates.size() <= 3) {
                        Hint h; h.row = r; h.col = c; h.type = HintType::CandidateList; h.candidates = candidates;
                        h.message = "Candidati pe (" + std::to_string(r) + "," + std::to_string(c) + "): ";
                        for (size_t i = 0; i < candidates.size(); ++i) {
                            h.message += std::to_string(candidates[i]);
                            if (i + 1 < candidates.size()) h.message += ", ";
                        }
                        return h;
                    }
                }
            }
        }
        return std::nullopt;
    }
};