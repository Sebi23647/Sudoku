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
/**
 * @brief HintManager aggregates strategies and selects a hint to return.
 *
 * HintManager builds a HintContext (precomputes candidates), queries all
 * configured strategies, and selects one of the returned hints using a
 * weighted random selection based on StrategyEntry::weight.
 */
class HintManager : public IHintProvider {
public:
    /**
     * @brief Entry describing a strategy, its selection weight and a name.
     */
    struct StrategyEntry {
        /** @brief Owned strategy instance that produces hints. */
        std::unique_ptr<IHintStrategy> strategy;

        /** @brief Selection weight used when multiple strategies provide hints (default 1). */
        int weight = 1;

        /** @brief Optional human-readable name used for debugging/logging. */
        std::string name;
    };

    /**
     * @brief Set the strategy entries that this manager will use.
     *
     * The manager takes ownership of the strategy instances via the entries.
     *
     * @param entries Vector of StrategyEntry structures. The entries are moved
     * into the manager.
     */
    void setStrategyEntries(std::vector<StrategyEntry> entries) {
        strategies = std::move(entries);
    }

    // next builds a HintContext (precomputes candidates once) and then asks strategies in any order,
    // collects all available hints and chooses one via weighted random selection.
    /**
     * @brief Request the next hint for the provided game state.
     *
     * The implementation builds a HintContext, invokes each strategy and
     * collects all produced hints. Then a weighted random selection chooses
     * which hint to return. If no strategy returns a hint, std::nullopt is returned.
     *
     * @param game Read-only reference to the ISudokuGame instance.
     * @return Optional Hint when available, std::nullopt otherwise.
     */
    std::optional<Hint> next(const ISudokuGame& game) const override;

private:
    /** @brief Stored strategy entries (ownership held by HintManager). */
    std::vector<StrategyEntry> strategies;
};