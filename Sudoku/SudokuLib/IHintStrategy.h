#pragma once
#include <optional>
#include "Hint.h"
#include "HintContext.h"

/**
 * @brief Interface for individual hint strategies.
 *
 * Implementations analyze a HintContext and return an optional Hint when
 * they can produce a meaningful suggestion.
 */
class IHintStrategy {
public:
    virtual ~IHintStrategy() = default;
    // strategies now receive the precomputed HintContext
    /**
     * @brief Compute a hint using the provided precomputed hint context.
     *
     * @param ctx The HintContext containing a read-only game snapshot and precomputed candidates.
     * @return Optional Hint; std::nullopt if the strategy has no suggestion.
     */
    virtual std::optional<Hint> getHint(const HintContext& ctx) const = 0;
};