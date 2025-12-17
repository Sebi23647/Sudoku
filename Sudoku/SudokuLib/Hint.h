#pragma once
#include <vector>
#include <string>

/**
 * @brief Types of hints that hint providers or strategies can produce.
 *
 * Enumerates the different kinds of hints the system can return. Strategies
 * choose an appropriate HintType to communicate whether they suggest a
 * definite value, provide a candidate list, or offer a reasoning-based hint.
 */
enum class HintType {
    /** @brief Single candidate determined (definite value). */
    NakedSingle,
    /** @brief A hidden single within a unit. */
    HiddenSingle,
    /** @brief Provide a small candidate list for a cell (soft hint). */
    CandidateList,
    /** @brief A cell with a very small set of safe candidates or a revealed safe value. */
    SafeCell,
    /** @brief Hint derived from simple local reasoning (heuristic). */
    LocalReasoning
};

/**
 * @brief Representation of a hint returned by hint providers/strategies.
 *
 * A Hint describes a target cell (row/col), an optional suggested value,
 * the hint type, an optional list of candidate digits and a human-readable
 * message suitable for UI display.
 */
struct Hint {
    /** @brief Row index of the hinted cell (0-8). -1 if unspecified. */
    int row = -1;

    /** @brief Column index of the hinted cell (0-8). -1 if unspecified. */
    int col = -1;

    /**
     * @brief Suggested value for the cell (1-9).
     *
     * Value is 0 when the hint does not reveal a single definite value
     * (e.g. candidate list hints).
     */
    int value = 0;

    /** @brief Kind of hint provided (see HintType). */
    HintType type = HintType::CandidateList;

    /**
     * @brief Optional vector of candidate digits for soft hints.
     *
     * For CandidateList and SafeCell hints this contains the candidate digits
     * (values between 1 and 9).
     */
    std::vector<int> candidates;

    /**
     * @brief A short, human-readable explanation or message to display.
     */
    std::string message; 
};