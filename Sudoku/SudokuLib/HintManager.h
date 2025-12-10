//#pragma once
//#include <memory>
//#include <vector>
//#include <optional>
//#include "IHintStrategy.h"
//#include "IHintProvider.h"
//
//class HintManager : public IHintProvider {
//public:
//    void setStrategies(std::vector<std::unique_ptr<IHintStrategy>> strategies) {
//        this->strategies = std::move(strategies);
//    }
//
//    std::optional<Hint> next(const ISudokuGame& game) const override {
//        for (const auto& s : strategies) {
//            if (!s) continue;
//            auto h = s->getHint(game);
//            if (h.has_value()) return h;
//        }
//        return std::nullopt;
//    }
//
//private:
//    std::vector<std::unique_ptr<IHintStrategy>> strategies;
//};


#pragma once
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include "IHintStrategy.h"
#include "IHintProvider.h"
#include "HintContext.h"

// HintManager stores strategies together with metadata (weight, name).
class HintManager : public IHintProvider {
public:
    struct StrategyEntry {
        std::unique_ptr<IHintStrategy> strategy;
        int weight = 1;
        std::string name;
    };

    void setStrategyEntries(std::vector<StrategyEntry> entries) {
        strategies = std::move(entries);
    }

    // next builds a HintContext (precomputes candidates once) and then asks strategies in any order,
    // collects all available hints and chooses one via weighted random selection.
    std::optional<Hint> next(const ISudokuGame& game) const override;

private:
    std::vector<StrategyEntry> strategies;
};