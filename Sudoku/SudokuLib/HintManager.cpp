#include "HintManager.h"
#include <random>
#include <numeric>
#include <stdexcept>
#include <chrono>

// Thread-local RNG seeded once
static std::mt19937& getRng() {
    static thread_local std::mt19937 engine(
        static_cast<unsigned long>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) ^ std::random_device{}()
    );
    return engine;
}

std::optional<Hint> HintManager::next(const ISudokuGame& game) const {
    HintContext ctx(game, /*revealSolution=*/false);
    struct Candidate {
        Hint hint;
        int weight;
        std::string sourceName;
    };
    std::vector<Candidate> candidates;

    for (const auto& entry : strategies) {
        if (!entry.strategy) continue;
        try {
            auto h = entry.strategy->getHint(ctx);
            if (h.has_value()) {
                candidates.push_back({ std::move(h.value()), entry.weight, entry.name });
            }
        }
        catch (...) {
            continue;
        }
    }

    if (candidates.empty()) return std::nullopt;
    long long total = 0;
    for (const auto& c : candidates) total += std::max(0, c.weight);
    if (total <= 0) {
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
        return candidates[dist(getRng())].hint;
    }

    std::uniform_int_distribution<long long> dist(1, total);
    long long pick = dist(getRng());
    for (const auto& c : candidates) {
        pick -= std::max(0, c.weight);
        if (pick <= 0) return c.hint;
    }

    return candidates.front().hint;
}