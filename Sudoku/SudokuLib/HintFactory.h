#pragma once
#include <memory>
#include "Difficulty.h"
#include "IHintProvider.h"
#include "HintFactory.h"
#include "HintManager.h"
#include "NakedSingleStrategy.h"
#include "HiddenSingleStrategy.h"
#include "CandidateListStrategy.h"
#include "SafeCellStrategy.h"
#include "LocalReasoningStrategy.h"

// Builds a hint provider configured with strategies per Difficulty
std::unique_ptr<IHintProvider> CreateHintManager(Difficulty difficulty);
