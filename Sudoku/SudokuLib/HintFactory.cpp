

#include "HintManager.h"
#include "NakedSingleStrategy.h"
#include "HiddenSingleStrategy.h"
#include "CandidateListStrategy.h"
#include "SafeCellStrategy.h"
#include "LocalReasoningStrategy.h"

// Builds a hint provider configured with strategies per Difficulty. We use StrategyEntry with weights.
std::unique_ptr<IHintProvider> CreateHintManager(Difficulty difficulty) {
    auto mgr = std::make_unique<HintManager>();
    std::vector<HintManager::StrategyEntry> entries;

    switch (difficulty) {
    case Difficulty::EASY:
        entries.push_back({ std::make_unique<HiddenSingleStrategy>(), 20, "HiddenSingle" });
        entries.push_back({ std::make_unique<NakedSingleStrategy>(), 60, "NakedSingle" });
        entries.push_back({ std::make_unique<CandidateListStrategy>(), 20, "CandidateList" });
        break;
    case Difficulty::MEDIUM:
        entries.push_back({ std::make_unique<SafeCellStrategy>(), 40, "SafeCell" });
        entries.push_back({ std::make_unique<CandidateListStrategy>(), 30, "CandidateList" });
        entries.push_back({ std::make_unique<HiddenSingleStrategy>(), 20, "HiddenSingle" });
        entries.push_back({ std::make_unique<NakedSingleStrategy>(), 10, "NakedSingle" });
        break;
    case Difficulty::HARD:
        entries.push_back({ std::make_unique<LocalReasoningStrategy>(), 50, "LocalReasoning" });
        entries.push_back({ std::make_unique<SafeCellStrategy>(), 30, "SafeCell" });
        entries.push_back({ std::make_unique<CandidateListStrategy>(), 20, "CandidateList" });
        break;
    default:
        entries.push_back({ std::make_unique<CandidateListStrategy>(), 1, "CandidateList" });
        break;
    }

    mgr->setStrategyEntries(std::move(entries));
    return mgr;
}