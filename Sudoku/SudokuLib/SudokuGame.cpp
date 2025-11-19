#include "SudokuGame.h"
#include <algorithm>
#include <random>
#include <cstring>
#include <ctime>

SudokuGame::SudokuGame() : currentDifficulty(Difficulty::MEDIUM), remainingAttempts(3) {
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

SudokuGame::SudokuGame(Difficulty difficulty) : currentDifficulty(difficulty), remainingAttempts(3) {
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void SudokuGame::startNewGame() {
    remainingAttempts = 3;
    generatePuzzle();
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::startNewGame(Difficulty difficulty) {
    currentDifficulty = difficulty;
    startNewGame();
}

void SudokuGame::generatePuzzle() {
    // Reset boards
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));

    // Generate complete valid board
    fillBoard();

    // Save the solution
    saveSolution();

    // Remove cells based on difficulty
    removeCells();
}

bool SudokuGame::fillCell(int pos) {
    if (pos == 81) return true;

    int row = pos / 9;
    int col = pos % 9;

    std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::shuffle(nums.begin(), nums.end(), std::default_random_engine(std::rand()));

    for (int num : nums) {
        if (isSafe(row, col, num)) {
            board[row][col] = num;
            if (fillCell(pos + 1)) return true;
            board[row][col] = 0;
        }
    }
    return false;
}

void SudokuGame::removeCells() {
    int cellsToRemove;
    switch (currentDifficulty) {
    case Difficulty::EASY:
        cellsToRemove = 30;
        break;
    case Difficulty::MEDIUM:
        cellsToRemove = 40;
        break;
    case Difficulty::HARD:
        cellsToRemove = 50;
        break;
    default:
        cellsToRemove = 40;
    }

    while (cellsToRemove > 0) {
        int row = std::rand() % 9;
        int col = std::rand() % 9;

        if (board[row][col] != 0) {
            board[row][col] = 0;
            cellsToRemove--;
        }
    }

    // Mark remaining cells as initial/fixed
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) {
                initialCells[i][j] = true;
            }
        }
    }
}

bool SudokuGame::isSafe(int row, int col, int num) const {
    // Check row and column
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num || board[x][col] == num) {
            return false;
        }
    }

    // Check 3x3 box
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

bool SudokuGame::isValidPosition(int row, int col) const {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

bool SudokuGame::setValue(int row, int col, int value) {
    // Validate position
    if (!isValidPosition(row, col)) {
        return false;
    }

    // Validate value range
    if (value < 0 || value > 9) {
        return false;
    }

    // Cannot modify initial cells
    if (initialCells[row][col]) {
        return false;
    }

    // Allow clearing a cell
    if (value == 0) {
        board[row][col] = 0;
        notifyBoardChanged();
        return true;
    }

    // Check if move is valid
    if (isValidMove(row, col, value)) {
        board[row][col] = value;
        notifyBoardChanged();

        if (isComplete()) {
            notifyGameComplete();
        }
        return true;
    }
    else {
        // Invalid move - decrease attempts
        remainingAttempts--;
        notifyAttemptsChanged();

        if (remainingAttempts <= 0) {
            // Game over logic could be added here
        }

        return false;
    }
}

bool SudokuGame::isValidMove(int row, int col, int value) const {
    // Temporarily store current value
    int temp = board[row][col];

    // Temporarily set to 0 to check if new value is safe
    const_cast<int&>(board[row][col]) = 0;
    bool safe = isSafe(row, col, value);

    // Restore original value
    const_cast<int&>(board[row][col]) = temp;

    return safe;
}

int SudokuGame::getValue(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return -1;
    }
    return board[row][col];
}

CellState SudokuGame::getCellState(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return CellState::EMPTY;
    }

    if (initialCells[row][col]) {
        return CellState::FIXED;
    }

    if (board[row][col] != 0) {
        return CellState::FILLED;
    }

    return CellState::EMPTY;
}

bool SudokuGame::isComplete() const {
    // Check if all cells are filled
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }

    // Verify solution is correct
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int temp = board[i][j];
            const_cast<int&>(board[i][j]) = 0;

            if (!isSafe(i, j, temp)) {
                const_cast<int&>(board[i][j]) = temp;
                return false;
            }

            const_cast<int&>(board[i][j]) = temp;
        }
    }

    return true;
}

int SudokuGame::getRemainingAttempts() const {
    return remainingAttempts;
}

Difficulty SudokuGame::getCurrentDifficulty() const {
    return currentDifficulty;
}

void SudokuGame::reset() {
    // Clear all non-initial cells
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (!initialCells[i][j]) {
                board[i][j] = 0;
            }
        }
    }

    remainingAttempts = 3;
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::attachObserver(IObserver* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void SudokuGame::detachObserver(IObserver* observer) {
    if (observer) {
        observers.remove(observer);
    }
}

void SudokuGame::notifyBoardChanged() {
    for (auto observer : observers) {
        if (observer) {
            observer->onBoardChanged();
        }
    }
}

void SudokuGame::notifyGameComplete() {
    for (auto observer : observers) {
        if (observer) {
            observer->onGameComplete();
        }
    }
}

void SudokuGame::notifyAttemptsChanged() {
    for (auto observer : observers) {
        if (observer) {
            observer->onAttemptsChanged(remainingAttempts);
        }
    }
}

void SudokuGame::fillBoard() {
    fillCell(0);
}

void SudokuGame::saveSolution() {
    std::memcpy(solutionBoard, board, sizeof(board));
}