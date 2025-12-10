#pragma once
#include "ISudokuGame.h"
#include "Difficulty.h"
#include <memory>

/**
 * @brief Create a concrete SudokuGame configured with the given difficulty.
 *
 * @param difficulty Difficulty level for the new game instance.
 * @return unique_ptr<ISudokuGame> owning the created game instance.
 */
std::unique_ptr<ISudokuGame> CreateSudokuGame(Difficulty difficulty);