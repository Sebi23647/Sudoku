//#pragma once
//#include "IHintStrategy.h"
//#include "HintUtils.h"
//
//class NakedSingleStrategy : public IHintStrategy {
//public:
//    std::optional<Hint> getHint(const ISudokuGame& game) const override {
//        for (int r = 0; r < 9; ++r) {
//            for (int c = 0; c < 9; ++c) {
//                if (game.getCellState(r, c) == CellState::EMPTY) {
//                    auto candidates = computeCandidates(game, r, c);
//                    if (candidates.size() == 1) {
//                        Hint h;
//                        h.row = r; h.col = c;
//                        h.value = candidates[0];
//                        h.type = HintType::NakedSingle;
//                        h.candidates = candidates;
//                        h.message = "Naked Single: singurul candidat valid pe (" + std::to_string(r) + "," + std::to_string(c) + ") este " + std::to_string(h.value);
//                        return h;
//                    }
//                }
//            }
//        }
//        return std::nullopt;
//    }
//};

#pragma once
#include "IHintStrategy.h"
#include "HintContext.h"

class NakedSingleStrategy : public IHintStrategy {
public:
    std::optional<Hint> getHint(const HintContext& ctx) const override {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (ctx.game.getCellState(r, c) == CellState::EMPTY) {
                    const auto& cand = ctx.candidatesAt(r, c);
                    if (cand.size() == 1) {
                        Hint h;
                        h.row = r; h.col = c;
                        h.value = cand[0];
                        h.type = HintType::NakedSingle;
                        h.candidates = cand;
                        h.message = "Naked Single: singurul candidat valid pe (" + std::to_string(r) + "," + std::to_string(c) + ") este " + std::to_string(h.value);
                        return h;
                    }
                }
            }
        }
        return std::nullopt;
    }
};