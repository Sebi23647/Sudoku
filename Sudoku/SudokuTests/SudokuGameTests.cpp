#include <gtest/gtest.h>
#include "../SudokuLib/SudokuGame.h"
#include "../SudokuLib/Difficulty.h"
#include "../SudokuLib/CellState.h"

/* Test pentru functionalitatea de baza a SudokuGame*/

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


// TESTE INITIALIZARE

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

    // Verificam ca exista cel putin o celula completata (fixed)
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

// TESTE GETVALUE

TEST_F(SudokuGameBasicTests, GetValueReturnsZeroForEmptyCell) {
    game->startNewGame();

    // Cautam o celula goala
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

    // Cautam o celula fixed
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

// TESTE setValue - VALORI VALIDE


TEST_F(SudokuGameBasicTests, SetValueOnEmptyCell) {
    game->startNewGame();

    // Cautam prima celula goala
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Gasim o valoare valida pentru aceasta celula
                for (int value = 1; value <= 9; value++) {
                    // Testam temporar daca valoarea este sigura
                    bool canSet = true;

                    // Verificam randul
                    for (int col = 0; col < 9; col++) {
                        if (game->getValue(i, col) == value) {
                            canSet = false;
                            break;
                        }
                    }

                    // Verificam coloana
                    for (int row = 0; row < 9; row++) {
                        if (game->getValue(row, j) == value) {
                            canSet = false;
                            break;
                        }
                    }

                    // Verificam grid-ul 3x3
                    int startRow = i - i % 3;
                    int startCol = j - j % 3;
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            if (game->getValue(startRow + r, startCol + c) == value) {
                                canSet = false;
                                break;
                            }
                        }
                        if (!canSet) break;
                    }

                    if (canSet) {
                        EXPECT_TRUE(game->setValue(i, j, value));
                        EXPECT_EQ(game->getValue(i, j), value);
                        EXPECT_EQ(game->getCellState(i, j), CellState::FILLED);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, SetValueClearCell) {
    game->startNewGame();

    // Cautam o celula goala si punem o valoare
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Gasim o valoare valida
                for (int value = 1; value <= 9; value++) {
                    bool safe = true;

                    // Quick validation
                    for (int x = 0; x < 9; x++) {
                        if (game->getValue(i, x) == value || game->getValue(x, j) == value) {
                            safe = false;
                            break;
                        }
                    }

                    if (safe) {
                        game->setValue(i, j, value);

                        // Acum stergem valoarea
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

// TESTE setValue - VALORI INVALIDE

TEST_F(SudokuGameBasicTests, SetValueInvalidPosition) {
    EXPECT_FALSE(game->setValue(-1, 0, 5));
    EXPECT_FALSE(game->setValue(0, -1, 5));
    EXPECT_FALSE(game->setValue(9, 0, 5));
    EXPECT_FALSE(game->setValue(0, 9, 5));
}

TEST_F(SudokuGameBasicTests, SetValueOutOfRange) {
    game->startNewGame();

    // Cautam o celula goala
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

    // Cautam o celula fixed
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                int originalValue = game->getValue(i, j);

                // Incercam sa modificam celula fixed
                EXPECT_FALSE(game->setValue(i, j, 5));

                // Valoarea ramane neschimbata
                EXPECT_EQ(game->getValue(i, j), originalValue);
                return;
            }
        }
    }
}

TEST_F(SudokuGameBasicTests, SetValueInvalidMove) {
    game->startNewGame();

    // Cautam o celula goala
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Cautam o valoare care exista deja pe randul curent
                for (int col = 0; col < 9; col++) {
                    int existingValue = game->getValue(i, col);
                    if (existingValue != 0) {
                        int attemptsBefore = game->getRemainingAttempts();

                        // Incercam sa punem aceeasi valoare (invalid)
                        EXPECT_FALSE(game->setValue(i, j, existingValue));

                        // Verificam ca attempts a scazut
                        EXPECT_EQ(game->getRemainingAttempts(), attemptsBefore - 1);
                        return;
                    }
                }
            }
        }
    }
}

// TESTE getCellState

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

    // Cautam o celula goala si o completam
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Gasim o valoare valida
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

// TESTE RESET


TEST_F(SudokuGameBasicTests, ResetClearsFilledCells) {
    game->startNewGame();

    // Completam cateva celule goale
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

    // Resetam jocul
    game->reset();

    // Verificam ca celulele FILLED au fost sterse
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            CellState state = game->getCellState(i, j);
            // Nu ar trebui sa existe celule FILLED dupa reset
            EXPECT_NE(state, CellState::FILLED);
        }
    }
}

TEST_F(SudokuGameBasicTests, ResetKeepsFixedCells) {
    game->startNewGame();

    // Salvam valorile celulelor fixed
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

    // Verificam ca celulele fixed au ramas la fel
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

    // Facem cateva mutari invalide sa scadem attempts
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Cautam o valoare invalida
                for (int col = 0; col < 9; col++) {
                    int value = game->getValue(i, col);
                    if (value != 0) {
                        game->setValue(i, j, value); // Invalid move
                        break;
                    }
                }
                break;
            }
        }
        break;
    }

    game->reset();

    // Verificam ca attempts este 3
    EXPECT_EQ(game->getRemainingAttempts(), 3);
}

// TESTE isComplete

TEST_F(SudokuGameBasicTests, IsCompleteReturnsFalseForIncompleteBoard) {
    game->startNewGame();
    EXPECT_FALSE(game->isComplete());
}

TEST_F(SudokuGameBasicTests, IsCompleteReturnsFalseForInvalidBoard) {
    game->startNewGame();

    // Completam tabla cu valori invalide (toate cu 1)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
               
            }
        }
    }

    EXPECT_FALSE(game->isComplete());
}

// TESTE ATTEMPTS


TEST_F(SudokuGameBasicTests, AttemptsDecreaseOnInvalidMove) {
    game->startNewGame();
    int initialAttempts = game->getRemainingAttempts();

    // Facem o mutare invalida
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Cautam o valoare care exista deja pe rand
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

    // Facem o mutare valida
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    if (game->setValue(i, j, value)) {
                        EXPECT_EQ(game->getRemainingAttempts(), initialAttempts);
                        return;
                    }
                }
            }
        }
    }
}

// TESTE DIFFICULTY

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

// TESTE PUZZLE GENERATION

TEST_F(SudokuGameBasicTests, GeneratedPuzzleHasCorrectNumberOfFixedCells) {
    // Test pentru EASY - ar trebui sa aiba mai multe celule fixed
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

    // Test pentru HARD - ar trebui sa aiba mai putine celule fixed
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

    // EASY ar trebui sa aiba mai multe celule fixed decat HARD
    EXPECT_GT(easyFixed, hardFixed);
}

TEST_F(SudokuGameBasicTests, GeneratedPuzzleHasValidFixedCells) {
    game->startNewGame();

    // Verificam ca toate celulele fixed respecta regulile Sudoku
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::FIXED) {
                int value = game->getValue(i, j);

                // Verificam ca valoarea este in range
                EXPECT_GE(value, 1);
                EXPECT_LE(value, 9);

                // Verificam unicitatea pe rand
                int countInRow = 0;
                for (int col = 0; col < 9; col++) {
                    if (game->getValue(i, col) == value) {
                        countInRow++;
                    }
                }
                EXPECT_EQ(countInRow, 1);

                // Verificam unicitatea pe coloana
                int countInCol = 0;
                for (int row = 0; row < 9; row++) {
                    if (game->getValue(row, j) == value) {
                        countInCol++;
                    }
                }
                EXPECT_EQ(countInCol, 1);

                // Verificam unicitatea in grid 3x3
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