#include <gtest/gtest.h>
#include "../SudokuLib/SudokuGame.h"
#include "../SudokuLib/Difficulty.h"
#include "../SudokuLib/CellState.h"


class SudokuValidationTests : public ::testing::Test {
protected:
    SudokuGame* game;

    void SetUp() override {
        game = new SudokuGame();
        game->startNewGame();
    }

    void TearDown() override {
        delete game;
    }

    bool findEmptyCell(int& row, int& col) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (game->getCellState(i, j) == CellState::EMPTY) {
                    row = i;
                    col = j;
                    return true;
                }
            }
        }
        return false;
    }

    bool findValueInRow(int row, int& value) {
        for (int col = 0; col < 9; col++) {
            int val = game->getValue(row, col);
            if (val != 0) {
                value = val;
                return true;
            }
        }
        return false;
    }

    bool findValueInColumn(int col, int& value) {
        for (int row = 0; row < 9; row++) {
            int val = game->getValue(row, col);
            if (val != 0) {
                value = val;
                return true;
            }
        }
        return false;
    }
    bool findValueInGrid(int row, int col, int& value) {
        int startRow = row - row % 3;
        int startCol = col - col % 3;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int val = game->getValue(startRow + i, startCol + j);
                if (val != 0) {
                    value = val;
                    return true;
                }
            }
        }
        return false;
    }
};

TEST_F(SudokuValidationTests, CannotPlaceDuplicateInRow) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int existingValue;
    if (!findValueInRow(row, existingValue)) {
        GTEST_SKIP() << "No value found in row";
    }

    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInRow) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }
    int correctValue = game->getSolutionValue(row, col);
    EXPECT_TRUE(game->setValue(row, col, correctValue));
}

TEST_F(SudokuValidationTests, AllRowsMustHaveUniqueValues) {
    for (int row = 0; row < 9; row++) {
        int valueCounts[10] = { 0 };

        for (int col = 0; col < 9; col++) {
            int value = game->getValue(row, col);
            if (value != 0) {
                valueCounts[value]++;
                EXPECT_LE(valueCounts[value], 1)
                    << "Duplicate value " << value << " in row " << row;
            }
        }
    }
}


TEST_F(SudokuValidationTests, CannotPlaceDuplicateInColumn) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int existingValue;
    if (!findValueInColumn(col, existingValue)) {
        GTEST_SKIP() << "No value found in column";
    }
    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInColumn) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int correctValue = game->getSolutionValue(row, col);
    EXPECT_TRUE(game->setValue(row, col, correctValue));
}

TEST_F(SudokuValidationTests, AllColumnsMustHaveUniqueValues) {
    for (int col = 0; col < 9; col++) {
        int valueCounts[10] = { 0 };

        for (int row = 0; row < 9; row++) {
            int value = game->getValue(row, col);
            if (value != 0) {
                valueCounts[value]++;
                EXPECT_LE(valueCounts[value], 1)
                    << "Duplicate value " << value << " in column " << col;
            }
        }
    }
}

TEST_F(SudokuValidationTests, CannotPlaceDuplicateInGrid) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int existingValue;
    if (!findValueInGrid(row, col, existingValue)) {
        GTEST_SKIP() << "No value found in grid";
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    bool valueExistsInGrid = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->getValue(startRow + i, startCol + j) == existingValue) {
                valueExistsInGrid = true;
                break;
            }
        }
        if (valueExistsInGrid) break;
    }

    if (!valueExistsInGrid) {
        GTEST_SKIP() << "Value not actually in grid";
    }

    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInGrid) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int correctValue = game->getSolutionValue(row, col);
    EXPECT_TRUE(game->setValue(row, col, correctValue));
}

TEST_F(SudokuValidationTests, AllGridsMustHaveUniqueValues) {
    for (int gridRow = 0; gridRow < 3; gridRow++) {
        for (int gridCol = 0; gridCol < 3; gridCol++) {
            int valueCounts[10] = { 0 };
            int startRow = gridRow * 3;
            int startCol = gridCol * 3;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int value = game->getValue(startRow + i, startCol + j);
                    if (value != 0) {
                        valueCounts[value]++;
                        EXPECT_LE(valueCounts[value], 1)
                            << "Duplicate value " << value
                            << " in grid (" << gridRow << "," << gridCol << ")";
                    }
                }
            }
        }
    }
}

TEST_F(SudokuValidationTests, ValidMoveSatisfiesAllRules) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int correctValue = game->getSolutionValue(row, col);

    EXPECT_TRUE(game->setValue(row, col, correctValue));

    EXPECT_EQ(game->getValue(row, col), correctValue);
}

TEST_F(SudokuValidationTests, InvalidMoveViolatesAtLeastOneRule) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    bool foundInvalidValue = false;

    for (int c = 0; c < 9; c++) {
        int value = game->getValue(row, c);
        if (value != 0) {
            EXPECT_FALSE(game->setValue(row, col, value));
            foundInvalidValue = true;
            break;
        }
    }

    if (!foundInvalidValue) {
        for (int r = 0; r < 9; r++) {
            int value = game->getValue(r, col);
            if (value != 0) {
                EXPECT_FALSE(game->setValue(row, col, value));
                foundInvalidValue = true;
                break;
            }
        }
    }

    if (!foundInvalidValue) {
        GTEST_SKIP() << "No invalid value found";
    }
}


TEST_F(SudokuValidationTests, CornerCellsFollowAllRules) {
    int corners[4][2] = { {0, 0}, {0, 8}, {8, 0}, {8, 8} };

    for (int i = 0; i < 4; i++) {
        int row = corners[i][0];
        int col = corners[i][1];

        if (game->getCellState(row, col) == CellState::EMPTY) {
            int correctValue = game->getSolutionValue(row, col);
            EXPECT_TRUE(game->setValue(row, col, correctValue));
        }
    }
}

TEST_F(SudokuValidationTests, CenterCellsFollowAllRules) {
    int row = 4, col = 4;

    if (game->getCellState(row, col) == CellState::EMPTY) {
        int correctValue = game->getSolutionValue(row, col);
        EXPECT_TRUE(game->setValue(row, col, correctValue));
    }
}

TEST_F(SudokuValidationTests, ValidationConsistentAcrossMultipleCalls) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int existingValue;
    if (!findValueInRow(row, existingValue)) {
        GTEST_SKIP() << "No value in row";
    }

    bool firstAttempt = game->setValue(row, col, existingValue);
    bool secondAttempt = game->setValue(row, col, existingValue);

    EXPECT_EQ(firstAttempt, secondAttempt);
    EXPECT_FALSE(firstAttempt);
}