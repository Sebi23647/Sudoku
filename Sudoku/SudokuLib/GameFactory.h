#pragma once
#include "ISudokuGame.h"
#include "Difficulty.h"
#include <memory>

std::unique_ptr<ISudokuGame> CreateSudokuGame(Difficulty difficulty);