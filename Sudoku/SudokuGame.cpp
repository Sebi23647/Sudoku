#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SudokuGame.h"
#include "MockObserver.h"
#include "Difficulty.h"

using ::testing::AtLeast;
using ::testing::_;

class SudokuGameTest : public ::testing::Test {
protected:
    SudokuGame* game;
    MockObserver* mockObserver;

    void SetUp() override {
        game = new SudokuGame(Difficulty::EASY);
        mockObserver = new MockObserver();
        game->attachObserver(mockObserver);
    }

    void TearDown() override {
        delete game;
        delete mockObserver;
    }
};

// Test 1: Initialization
TEST_F(SudokuGameTest, GameInitializesCorrectly) {
    EXPECT_EQ(game->getRemainingAttempts(), 3);
}

// Test 2: Start New Game notifies observers
TEST_F(SudokuGameTest, StartNewGameNotifiesObservers) {
    EXPECT_CALL(*mockObserver, onBoardChanged()).Times(1);
    EXPECT_CALL(*mockObserver, onAttemptsChanged(3)).Times(1);

    game->startNewGame();
}

// Test 3: Invalid coordinates
TEST_F(SudokuGameTest, SetValueWithInvalidCoordinates) {
    game->startNewGame();
    EXPECT_FALSE(game->setValue(-1, 0, 5));
    EXPECT_FALSE(game->setValue(0, -1, 5));
    EXPECT_FALSE(game->setValue(9, 0, 5));
    EXPECT_FALSE(game->setValue(0, 9, 5));
}

// Test 4: Invalid value
TEST_F(SudokuGameTest, SetValueWithInvalidValue) {
    game->startNewGame();

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        EXPECT_FALSE(game->setValue(emptyRow, emptyCol, -1));
        EXPECT_FALSE(game->setValue(emptyRow, emptyCol, 10));
    }
}

// Test 5: Get value
TEST_F(SudokuGameTest, GetValueReturnsCorrectValue) {
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int value = game->getValue(i, j);
            EXPECT_GE(value, 0);
            EXPECT_LE(value, 9);
        }
    }
}

// Test 6: Get value with invalid coordinates
TEST_F(SudokuGameTest, GetValueWithInvalidCoordinates) {
    EXPECT_EQ(game->getValue(-1, 0), -1);
    EXPECT_EQ(game->getValue(0, -1), -1);
    EXPECT_EQ(game->getValue(9, 0), -1);
    EXPECT_EQ(game->getValue(0, 9), -1);
}

// Test 7: Invalid move decreases attempts
TEST_F(SudokuGameTest, InvalidMoveDecreasesAttempts) {
    game->startNewGame();

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        int conflictValue = -1;
        for (int v = 1; v <= 9; v++) {
            bool canPlace = true;
            for (int k = 0; k < 9; k++) {
                if (game->getValue(emptyRow, k) == v ||
                    game->getValue(k, emptyCol) == v) {
                    conflictValue = v;
                    canPlace = false;
                    break;
                }
            }
            if (!canPlace) break;
        }

        if (conflictValue != -1) {
            EXPECT_CALL(*mockObserver, onAttemptsChanged(2)).Times(1);
            game->setValue(emptyRow, emptyCol, conflictValue);
            EXPECT_EQ(game->getRemainingAttempts(), 2);
        }
    }
}

// Test 8: Valid move notifies board changed
TEST_F(SudokuGameTest, ValidMoveNotifiesBoardChanged) {
    game->startNewGame();

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        int validValue = -1;
        for (int v = 1; v <= 9; v++) {
            bool canPlace = true;
            for (int k = 0; k < 9; k++) {
                if (game->getValue(emptyRow, k) == v ||
                    game->getValue(k, emptyCol) == v) {
                    canPlace = false;
                    break;
                }
            }

            if (canPlace) {
                int startRow = emptyRow - emptyRow % 3;
                int startCol = emptyCol - emptyCol % 3;
                for (int i = 0; i < 3 && canPlace; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (game->getValue(i + startRow, j + startCol) == v) {
                            canPlace = false;
                            break;
                        }
                    }
                }
            }

            if (canPlace) {
                validValue = v;
                break;
            }
        }

        if (validValue != -1) {
            EXPECT_CALL(*mockObserver, onBoardChanged()).Times(AtLeast(1));
            game->setValue(emptyRow, emptyCol, validValue);
        }
    }
}

// Test 9: Reset restores initial board
TEST_F(SudokuGameTest, ResetRestoresInitialBoard) {
    game->startNewGame();

    int initialValues[9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            initialValues[i][j] = game->getValue(i, j);
        }
    }

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        game->setValue(emptyRow, emptyCol, 5);
    }

    EXPECT_CALL(*mockObserver, onBoardChanged()).Times(1);
    EXPECT_CALL(*mockObserver, onAttemptsChanged(3)).Times(1);
    game->reset();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            EXPECT_EQ(game->getValue(i, j), initialValues[i][j]);
        }
    }

    EXPECT_EQ(game->getRemainingAttempts(), 3);
}

// Test 10: Clear cell (set to 0)
TEST_F(SudokuGameTest, ClearCellSetsValueToZero) {
    game->startNewGame();

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        game->setValue(emptyRow, emptyCol, 5);

        EXPECT_CALL(*mockObserver, onBoardChanged()).Times(1);
        EXPECT_TRUE(game->setValue(emptyRow, emptyCol, 0));
        EXPECT_EQ(game->getValue(emptyRow, emptyCol), 0);
    }
}

// Test 11: Cannot modify fixed cells
TEST_F(SudokuGameTest, CannotModifyFixedCells) {
    game->startNewGame();

    int fixedRow = -1, fixedCol = -1;
    for (int i = 0; i < 9 && fixedRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) != 0) {
                fixedRow = i;
                fixedCol = j;
                break;
            }
        }
    }

    if (fixedRow != -1) {
        int originalValue = game->getValue(fixedRow, fixedCol);
        EXPECT_FALSE(game->setValue(fixedRow, fixedCol, 5));
        EXPECT_EQ(game->getValue(fixedRow, fixedCol), originalValue);
    }
}

// Test 12: Observer is attached correctly
TEST_F(SudokuGameTest, ObserverAttachesCorrectly) {
    MockObserver* secondObserver = new MockObserver();
    game->attachObserver(secondObserver);

    EXPECT_CALL(*mockObserver, onBoardChanged()).Times(1);
    EXPECT_CALL(*secondObserver, onBoardChanged()).Times(1);
    EXPECT_CALL(*mockObserver, onAttemptsChanged(3)).Times(1);
    EXPECT_CALL(*secondObserver, onAttemptsChanged(3)).Times(1);

    game->startNewGame();

    delete secondObserver;
}

// Test 13: Multiple invalid moves
TEST_F(SudokuGameTest, MultipleInvalidMovesDecreaseAttempts) {
    game->startNewGame();
    int initialAttempts = game->getRemainingAttempts();

    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 9 && emptyRow == -1; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getValue(i, j) == 0) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    if (emptyRow != -1) {
        int conflictValue = -1;
        for (int v = 1; v <= 9; v++) {
            bool found = false;
            for (int k = 0; k < 9; k++) {
                if (game->getValue(emptyRow, k) == v) {
                    conflictValue = v;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (conflictValue != -1) {
            game->setValue(emptyRow, emptyCol, conflictValue);
            game->setValue(emptyRow, emptyCol, conflictValue);

            EXPECT_LT(game->getRemainingAttempts(), initialAttempts);
        }
    }
}

// Test 14: Different difficulty levels
TEST_F(SudokuGameTest, DifferentDifficultyLevels) {
    SudokuGame easyGame(Difficulty::EASY);
    SudokuGame mediumGame(Difficulty::MEDIUM);
    SudokuGame hardGame(Difficulty::HARD);

    easyGame.startNewGame();
    mediumGame.startNewGame();
    hardGame.startNewGame();

    int easyEmpty = 0, mediumEmpty = 0, hardEmpty = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (easyGame.getValue(i, j) == 0) easyEmpty++;
            if (mediumGame.getValue(i, j) == 0) mediumEmpty++;
            if (hardGame.getValue(i, j) == 0) hardEmpty++;
        }
    }

    EXPECT_LT(easyEmpty, mediumEmpty);
    EXPECT_LT(mediumEmpty, hardEmpty);
}

// Main function for running tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}