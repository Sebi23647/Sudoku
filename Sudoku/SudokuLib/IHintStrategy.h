//#pragma once
//#include <optional>
//#include "ISudokuGame.h"
//#include "Hint.h"
//
//class IHintStrategy {
//public:
//    virtual ~IHintStrategy() = default;
//    virtual std::optional<Hint> getHint(const ISudokuGame& game) const = 0;
//};

#pragma once
#include <optional>
#include "Hint.h"
#include "HintContext.h"

class IHintStrategy {
public:
    virtual ~IHintStrategy() = default;
    // strategies now receive the precomputed HintContext
    virtual std::optional<Hint> getHint(const HintContext& ctx) const = 0;
};