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

    // Helper: gaseste o valoare care exista in grid 3x3
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

// TESTE VALIDARE RAND

TEST_F(SudokuValidationTests, CannotPlaceDuplicateInRow) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int existingValue;
    if (!findValueInRow(row, existingValue)) {
        GTEST_SKIP() << "No value found in row";
    }

    // Incercam sa punem aceeasi valoare pe acelasi rand
    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInRow) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    // Gasim o valoare care NU exista pe rand
    for (int value = 1; value <= 9; value++) {
        bool existsInRow = false;
        for (int c = 0; c < 9; c++) {
            if (game->getValue(row, c) == value) {
                existsInRow = true;
                break;
            }
        }

        if (!existsInRow) {
            // Verificam si celelalte conditii (coloana si grid)
            bool existsInCol = false;
            for (int r = 0; r < 9; r++) {
                if (game->getValue(r, col) == value) {
                    existsInCol = true;
                    break;
                }
            }

            if (!existsInCol) {
                int startRow = row - row % 3;
                int startCol = col - col % 3;
                bool existsInGrid = false;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (game->getValue(startRow + i, startCol + j) == value) {
                            existsInGrid = true;
                            break;
                        }
                    }
                    if (existsInGrid) break;
                }

                if (!existsInGrid) {
                    // Aceasta valoare este sigura
                    EXPECT_TRUE(game->setValue(row, col, value));
                    return;
                }
            }
        }
    }

    GTEST_SKIP() << "No safe value found";
}

TEST_F(SudokuValidationTests, AllRowsMustHaveUniqueValues) {
    // Verificam ca toate randurile au valori unice
    for (int row = 0; row < 9; row++) {
        int valueCounts[10] = { 0 }; // Index 0 nefolosit, 1-9 pentru valori

        for (int col = 0; col < 9; col++) {
            int value = game->getValue(row, col);
            if (value != 0) {
                valueCounts[value]++;
                // Fiecare valoare ar trebui sa apara cel mult o data
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

    // Incercam sa punem aceeasi valoare pe aceeasi coloana
    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInColumn) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    // Gasim o valoare care NU exista pe coloana
    for (int value = 1; value <= 9; value++) {
        bool existsInCol = false;
        for (int r = 0; r < 9; r++) {
            if (game->getValue(r, col) == value) {
                existsInCol = true;
                break;
            }
        }

        if (!existsInCol) {
            // Verificam si celelalte conditii
            bool existsInRow = false;
            for (int c = 0; c < 9; c++) {
                if (game->getValue(row, c) == value) {
                    existsInRow = true;
                    break;
                }
            }

            if (!existsInRow) {
                int startRow = row - row % 3;
                int startCol = col - col % 3;
                bool existsInGrid = false;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (game->getValue(startRow + i, startCol + j) == value) {
                            existsInGrid = true;
                            break;
                        }
                    }
                    if (existsInGrid) break;
                }

                if (!existsInGrid) {
                    EXPECT_TRUE(game->setValue(row, col, value));
                    return;
                }
            }
        }
    }

    GTEST_SKIP() << "No safe value found";
}

TEST_F(SudokuValidationTests, AllColumnsMustHaveUniqueValues) {
    // Verificam ca toate coloanele au valori unice
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

    // Verificam ca valoarea exista intr-adevar in grid
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

    // Incercam sa punem aceeasi valoare in acelasi grid
    EXPECT_FALSE(game->setValue(row, col, existingValue));
}

TEST_F(SudokuValidationTests, CanPlaceValueNotInGrid) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    // Gasim o valoare care NU exista in grid
    for (int value = 1; value <= 9; value++) {
        bool existsInGrid = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (game->getValue(startRow + i, startCol + j) == value) {
                    existsInGrid = true;
                    break;
                }
            }
            if (existsInGrid) break;
        }

        if (!existsInGrid) {
            // Verificam si celelalte conditii
            bool existsInRow = false;
            for (int c = 0; c < 9; c++) {
                if (game->getValue(row, c) == value) {
                    existsInRow = true;
                    break;
                }
            }

            bool existsInCol = false;
            for (int r = 0; r < 9; r++) {
                if (game->getValue(r, col) == value) {
                    existsInCol = true;
                    break;
                }
            }

            if (!existsInRow && !existsInCol) {
                EXPECT_TRUE(game->setValue(row, col, value));
                return;
            }
        }
    }

    GTEST_SKIP() << "No safe value found";
}

TEST_F(SudokuValidationTests, AllGridsMustHaveUniqueValues) {
    // Verificam toate cele 9 grid-uri 3x3
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

    // Gasim o valoare valida care respecta toate regulile
    for (int value = 1; value <= 9; value++) {
        bool isValid = true;

        // Check rand
        for (int c = 0; c < 9; c++) {
            if (game->getValue(row, c) == value) {
                isValid = false;
                break;
            }
        }

        // Check coloana
        if (isValid) {
            for (int r = 0; r < 9; r++) {
                if (game->getValue(r, col) == value) {
                    isValid = false;
                    break;
                }
            }
        }

        // Check grid
        if (isValid) {
            int startRow = row - row % 3;
            int startCol = col - col % 3;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (game->getValue(startRow + i, startCol + j) == value) {
                        isValid = false;
                        break;
                    }
                }
                if (!isValid) break;
            }
        }

        if (isValid) {
            EXPECT_TRUE(game->setValue(row, col, value));

            // Verificam ca valoarea a fost setata
            EXPECT_EQ(game->getValue(row, col), value);
            return;
        }
    }

    GTEST_SKIP() << "No valid value found";
}

TEST_F(SudokuValidationTests, InvalidMoveViolatesAtLeastOneRule) {
    int row, col;
    if (!findEmptyCell(row, col)) {
        GTEST_SKIP() << "No empty cell found";
    }

    // Gasim o valoare care violeaza cel putin o regula
    bool foundInvalidValue = false;

    // Incercam sa gasim o valoare care exista pe rand
    for (int c = 0; c < 9; c++) {
        int value = game->getValue(row, c);
        if (value != 0) {
            EXPECT_FALSE(game->setValue(row, col, value));
            foundInvalidValue = true;
            break;
        }
    }

    if (!foundInvalidValue) {
        // Incercam sa gasim o valoare care exista pe coloana
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
    // Testam celulele din colturile tablei
    int corners[4][2] = { {0, 0}, {0, 8}, {8, 0}, {8, 8} };

    for (int i = 0; i < 4; i++) {
        int row = corners[i][0];
        int col = corners[i][1];

        if (game->getCellState(row, col) == CellState::EMPTY) {
            // Gasim o valoare valida
            for (int value = 1; value <= 9; value++) {
                bool isValid = true;

                // Validare completa
                for (int x = 0; x < 9; x++) {
                    if (game->getValue(row, x) == value ||
                        game->getValue(x, col) == value) {
                        isValid = false;
                        break;
                    }
                }

                if (isValid) {
                    int startRow = row - row % 3;
                    int startCol = col - col % 3;
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            if (game->getValue(startRow + r, startCol + c) == value) {
                                isValid = false;
                                break;
                            }
                        }
                        if (!isValid) break;
                    }
                }

                if (isValid) {
                    EXPECT_TRUE(game->setValue(row, col, value));
                    break;
                }
            }
        }
    }
}

TEST_F(SudokuValidationTests, CenterCellsFollowAllRules) {
    // Testam celula din centrul tablei (4, 4)
    int row = 4, col = 4;

    if (game->getCellState(row, col) == CellState::EMPTY) {
        for (int value = 1; value <= 9; value++) {
            bool isValid = true;

            for (int x = 0; x < 9; x++) {
                if (game->getValue(row, x) == value ||
                    game->getValue(x, col) == value) {
                    isValid = false;
                    break;
                }
            }

            if (isValid) {
                int startRow = row - row % 3;
                int startCol = col - col % 3;
                for (int r = 0; r < 3; r++) {
                    for (int c = 0; c < 3; c++) {
                        if (game->getValue(startRow + r, startCol + c) == value) {
                            isValid = false;
                            break;
                        }
                    }
                    if (!isValid) break;
                }
            }

            if (isValid) {
                EXPECT_TRUE(game->setValue(row, col, value));
                break;
            }
        }
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

    // Prima incercare - ar trebui sa esueze
    bool firstAttempt = game->setValue(row, col, existingValue);

    // A doua incercare - ar trebui sa esueze la fel
    bool secondAttempt = game->setValue(row, col, existingValue);

    EXPECT_EQ(firstAttempt, secondAttempt);
    EXPECT_FALSE(firstAttempt);
}