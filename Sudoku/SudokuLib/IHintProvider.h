//#pragma once
//#include <optional>
//#include "ISudokuGame.h"
//#include "Hint.h"
//
//class IHintProvider {
//public:
//    virtual ~IHintProvider() = default;
//    virtual std::optional<Hint> next(const ISudokuGame& game) const = 0;
//};


#pragma once
#include <optional>
#include "Hint.h"
#include "ISudokuGame.h"

// Public API for hint providers: UI calls next() with ISudokuGame snapshot; HintManager builds HintContext internally;
/**
 * @brief Public API for hint providers used by the UI.
 *
 * The UI calls next() with a snapshot of the ISudokuGame to request a hint.
 * Implementations may aggregate strategies or provide simpler behavior.
 */
class IHintProvider {
public:
    virtual ~IHintProvider() = default;
    // revealSolution option could be added later as overload/parameters

    /**
     * @brief Request the next hint for the provided game state.
     *
     * @param game Read-only reference to the ISudokuGame to analyze.
     * @return Optional Hint; std::nullopt when no hint is available.
     */
    virtual std::optional<Hint> next(const ISudokuGame& game) const = 0;
};