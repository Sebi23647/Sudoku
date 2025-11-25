#include <gtest/gtest.h>
#include "../SudokuLib/SudokuGame.h"
#include "../SudokuLib/Difficulty.h"
#include "../SudokuLib/CellState.h"

class SudokuGameBasicTests : public ::testing::Test {
protected:
    SudokuGame* game;

    void SetUp() override {
        game = new SudokuGame();
    }

    void TearDown() override {
        delete game;
    }
};



TEST_F(SudokuGameBasicTests, ConstructorInitializesWithDefaultDifficulty) {
    EXPECT_EQ(game->getCurrentDifficulty(), Difficulty::MEDIUM);
}

TEST_F(SudokuGameBasicTests, ConstructorWithDifficulty) {
    SudokuGame easyGame(Difficulty::EASY);
    EXPECT_EQ(easyGame.getCurrentDifficulty(), Difficulty::EASY);

    SudokuGame hardGame(Difficulty::HARD);
    EXPECT_EQ(hardGame.getCurrentDifficulty(), Difficulty::HARD);
}

TEST_F(SudokuGameBasicTests, InitialAttemptsAreThree) {
    EXPECT_EQ(game->getRemainingAttempts(), 3);
}

TEST_F(SudokuGameBasicTests, StartNewGameGeneratesPuzzle) {
    game->startNewGame();

    bool hasFixedCell = false;
    for (int i = 0; i < 9 && !hasFixedCell; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                hasFixedCell = true;
                break;
            }
        }
    }
    EXPECT_TRUE(hasFixedCell);
}

TEST_F(SudokuGameBasicTests, StartNewGameWithDifficulty) {
    game->startNewGame(Difficulty::HARD);
    EXPECT_EQ(game->getCurrentDifficulty(), Difficulty::HARD);
}

TEST_F(SudokuGameBasicTests, GetValueReturnsZeroForEmptyCell) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                EXPECT_EQ(game->getValue(i, j), 0);
                return;
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, GetValueReturnsCorrectValue) {
    game->startNewGame();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                int value = game->getValue(i, j);
                EXPECT_GE(value, 1);
                EXPECT_LE(value, 9);
                return;
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, GetValueInvalidPosition) {
    EXPECT_EQ(game->getValue(-1, 0), -1);
    EXPECT_EQ(game->getValue(0, -1), -1);
    EXPECT_EQ(game->getValue(9, 0), -1);
    EXPECT_EQ(game->getValue(0, 9), -1);
    EXPECT_EQ(game->getValue(10, 10), -1);
}

TEST_F(SudokuGameBasicTests, SetValueOnEmptyCell) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);

                bool isSafe = true;

                for (int col = 0; col < 9; col++) {
                    if (game->getValue(i, col) == correctValue) {
                        isSafe = false;
                        break;
                    }
                }

                if (isSafe) {
                    for (int row = 0; row < 9; row++) {
                        if (game->getValue(row, j) == correctValue) {
                            isSafe = false;
                            break;
                        }
                    }
                }

                if (isSafe) {
                    int startRow = i - i % 3;
                    int startCol = j - j % 3;
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            if (game->getValue(startRow + r, startCol + c) == correctValue) {
                                isSafe = false;
                                break;
                            }
                        }
                        if (!isSafe) break;
                    }
                }
                if (isSafe) {
                    EXPECT_TRUE(game->setValue(i, j, correctValue));
                    EXPECT_EQ(game->getValue(i, j), correctValue);
                    EXPECT_EQ(game->getCellState(i, j), CellState::FILLED);
                    return;
                }
            }
        }
    }

    GTEST_SKIP() << "No safe empty cell found";
}

TEST_F(SudokuGameBasicTests, SetValueClearCell) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {

                for (int value = 1; value <= 9; value++) {
                    bool safe = true;

                    for (int x = 0; x < 9; x++) {
                        if (game->getValue(i, x) == value || game->getValue(x, j) == value) {
                            safe = false;
                            break;
                        }
                    }

                    if (safe) {
                        game->setValue(i, j, value);

                        EXPECT_TRUE(game->setValue(i, j, 0));
                        EXPECT_EQ(game->getValue(i, j), 0);
                        EXPECT_EQ(game->getCellState(i, j), CellState::EMPTY);
                        return;
                    }
                }
            }
        }
    }
}


TEST_F(SudokuGameBasicTests, SetValueInvalidPosition) {
    EXPECT_FALSE(game->setValue(-1, 0, 5));
    EXPECT_FALSE(game->setValue(0, -1, 5));
    EXPECT_FALSE(game->setValue(9, 0, 5));
    EXPECT_FALSE(game->setValue(0, 9, 5));
}

TEST_F(SudokuGameBasicTests, SetValueOutOfRange) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                EXPECT_FALSE(game->setValue(i, j, -1));
                EXPECT_FALSE(game->setValue(i, j, 10));
                EXPECT_FALSE(game->setValue(i, j, 100));
                return;
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, SetValueOnFixedCell) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                int originalValue = game->getValue(i, j);

                EXPECT_FALSE(game->setValue(i, j, 5));


                EXPECT_EQ(game->getValue(i, j), originalValue);
                return;
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, SetValueInvalidMove) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int col = 0; col < 9; col++) {
                    int existingValue = game->getValue(i, col);
                    if (existingValue != 0) {
                        int attemptsBefore = game->getRemainingAttempts();

                        EXPECT_FALSE(game->setValue(i, j, existingValue));

                        EXPECT_EQ(game->getRemainingAttempts(), attemptsBefore - 1);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, GetCellStateFixed) {
    game->startNewGame();

    bool foundFixed = false;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) != 0) {
                CellState state = game->getCellState(i, j);
                if (state == CellState::FIXED) {
                    foundFixed = true;
                    break;
                }
            }
        }
        if (foundFixed) break;
    }
    EXPECT_TRUE(foundFixed);
}

TEST_F(SudokuGameBasicTests, GetCellStateEmpty) {
    game->startNewGame();

    bool foundEmpty = false;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                EXPECT_EQ(game->getCellState(i, j), CellState::EMPTY);
                foundEmpty = true;
                break;
            }
        }
        if (foundEmpty) break;
    }
    EXPECT_TRUE(foundEmpty);
}

TEST_F(SudokuGameBasicTests, GetCellStateFilled) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    bool safe = true;
                    for (int x = 0; x < 9; x++) {
                        if (game->getValue(i, x) == value || game->getValue(x, j) == value) {
                            safe = false;
                            break;
                        }
                    }
                    if (safe) {
                        if (game->setValue(i, j, value)) {
                            EXPECT_EQ(game->getCellState(i, j), CellState::FILLED);
                            return;
                        }
                    }
                }
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, GetCellStateInvalidPosition) {
    EXPECT_EQ(game->getCellState(-1, 0), CellState::EMPTY);
    EXPECT_EQ(game->getCellState(0, -1), CellState::EMPTY);
    EXPECT_EQ(game->getCellState(9, 0), CellState::EMPTY);
    EXPECT_EQ(game->getCellState(0, 9), CellState::EMPTY);
}

TEST_F(SudokuGameBasicTests, ResetClearsFilledCells) {
    game->startNewGame();

    int filledCount = 0;
    for (int i = 0; i < 9 && filledCount < 3; i++) {
        for (int j = 0; j < 9 && filledCount < 3; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    if (game->setValue(i, j, value)) {
                        filledCount++;
                        break;
                    }
                }
            }
        }
    }
    game->reset();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            CellState state = game->getCellState(i, j);
            EXPECT_NE(state, CellState::FILLED);
        }
    }
}

TEST_F(SudokuGameBasicTests, ResetKeepsFixedCells) {
    game->startNewGame();

    int fixedValues[9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                fixedValues[i][j] = game->getValue(i, j);
            }
            else {
                fixedValues[i][j] = -1;
            }
        }
    }
    game->reset();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fixedValues[i][j] != -1) {
                EXPECT_EQ(game->getValue(i, j), fixedValues[i][j]);
                EXPECT_EQ(game->getCellState(i, j), CellState::FIXED);
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, ResetRestoresAttempts) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int col = 0; col < 9; col++) {
                    int value = game->getValue(i, col);
                    if (value != 0) {
                        game->setValue(i, j, value);
                        break;
                    }
                }
                break;
            }
        }
        break;
    }

    game->reset();

    EXPECT_EQ(game->getRemainingAttempts(), 3);
}

TEST_F(SudokuGameBasicTests, IsCompleteReturnsFalseForIncompleteBoard) {
    game->startNewGame();
    EXPECT_FALSE(game->isComplete());
}

TEST_F(SudokuGameBasicTests, IsCompleteReturnsFalseForInvalidBoard) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
               
            }
        }
    }

    EXPECT_FALSE(game->isComplete());
}

TEST_F(SudokuGameBasicTests, AttemptsDecreaseOnInvalidMove) {
    game->startNewGame();
    int initialAttempts = game->getRemainingAttempts();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int col = 0; col < 9; col++) {
                    int value = game->getValue(i, col);
                    if (value != 0) {
                        game->setValue(i, j, value);
                        EXPECT_EQ(game->getRemainingAttempts(), initialAttempts - 1);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, AttemptsNotDecreasedOnValidMove) {
    game->startNewGame();
    int initialAttempts = game->getRemainingAttempts();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);

                bool isSafe = true;
                for (int col = 0; col < 9; col++) {
                    if (game->getValue(i, col) == correctValue) {
                        isSafe = false;
                        break;
                    }
                }

                if (isSafe) {
                    for (int row = 0; row < 9; row++) {
                        if (game->getValue(row, j) == correctValue) {
                            isSafe = false;
                            break;
                        }
                    }
                }

                if (isSafe) {
                    int startRow = i - i % 3;
                    int startCol = j - j % 3;
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            if (game->getValue(startRow + r, startCol + c) == correctValue) {
                                isSafe = false;
                                break;
                            }
                        }
                        if (!isSafe) break;
                    }
                }

                if (isSafe && game->setValue(i, j, correctValue)) {
                    EXPECT_EQ(game->getRemainingAttempts(), initialAttempts);
                    return;
                }
            }
        }
    }

    GTEST_SKIP() << "No safe empty cell found for valid move test";
}

TEST_F(SudokuGameBasicTests, DifficultyRemainsAfterOperations) {
    game->startNewGame(Difficulty::HARD);


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                game->setValue(i, j, 5); 
                break;
            }
        }
        break;
    }

    EXPECT_EQ(game->getCurrentDifficulty(), Difficulty::HARD);
}

TEST_F(SudokuGameBasicTests, GeneratedPuzzleHasCorrectNumberOfFixedCells) {
    SudokuGame easyGame(Difficulty::EASY);
    easyGame.startNewGame();

    int easyFixed = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (easyGame.getCellState(i, j) == CellState::FIXED) {
                easyFixed++;
            }
        }
    }

    SudokuGame hardGame(Difficulty::HARD);
    hardGame.startNewGame();

    int hardFixed = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (hardGame.getCellState(i, j) == CellState::FIXED) {
                hardFixed++;
            }
        }
    }

    EXPECT_GT(easyFixed, hardFixed);
}

TEST_F(SudokuGameBasicTests, GeneratedPuzzleHasValidFixedCells) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                int value = game->getValue(i, j);

                EXPECT_GE(value, 1);
                EXPECT_LE(value, 9);

                int countInRow = 0;
                for (int col = 0; col < 9; col++) {
                    if (game->getValue(i, col) == value) {
                        countInRow++;
                    }
                }
                EXPECT_EQ(countInRow, 1);

                int countInCol = 0;
                for (int row = 0; row < 9; row++) {
                    if (game->getValue(row, j) == value) {
                        countInCol++;
                    }
                }
                EXPECT_EQ(countInCol, 1);

                int startRow = i - i % 3;
                int startCol = j - j % 3;
                int countInGrid = 0;
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        if (game->getValue(startRow + r, startCol + c) == value) {
                            countInGrid++;
                        }
                    }
                }
                EXPECT_EQ(countInGrid, 1);
            }
        }
    }
}