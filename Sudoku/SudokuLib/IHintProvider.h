
#pragma once
#include <optional>
#include "Hint.h"
#include "ISudokuGame.h"

// Public API for hint providers: UI calls next() with ISudokuGame snapshot; HintManager builds HintContext internally.
class IHintProvider {
public:
    virtual ~IHintProvider() = default;
    // revealSolution option could be added later as overload/parameters
    virtual std::optional<Hint> next(const ISudokuGame& game) const = 0;
};