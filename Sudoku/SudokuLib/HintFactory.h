#pragma once
#include <memory>
#include "Difficulty.h"
#include "IHintProvider.h"

// Builds a hint provider configured with strategies per Difficulty
std::unique_ptr<IHintProvider> CreateHintManager(Difficulty difficulty);
