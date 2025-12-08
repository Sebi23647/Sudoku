#include "GameFactory.h"
#include "SudokuGame.h"

std::unique_ptr<ISudokuGame> CreateSudokuGame(Difficulty difficulty) {
    return std::make_unique<SudokuGame>(difficulty);
}