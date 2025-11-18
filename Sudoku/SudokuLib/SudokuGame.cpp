#include "SudokuGame.h"
#include <algorithm>
#include <random>
#include <cstring>
#include <ctime>

SudokuGame::SudokuGame() : currentDifficulty(Difficulty::MEDIUM), remainingAttempts(3) {
    std::memset(board, 0, sizeof(board));
    std::memset(initialBoard, 0, sizeof(initialBoard));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

SudokuGame::SudokuGame(Difficulty difficulty) : currentDifficulty(difficulty), remainingAttempts(3) {
    std::memset(board, 0, sizeof(board));
    std::memset(initialBoard, 0, sizeof(initialBoard));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void SudokuGame::startNewGame() {
    remainingAttempts = 3;
    generatePuzzle();
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::generatePuzzle() {
    std::memset(board, 0, sizeof(board));
    fillBoard();
    std::memcpy(initialBoard, board, sizeof(board));
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
    case Difficulty::EASY: cellsToRemove = 30; break;
    case Difficulty::MEDIUM: cellsToRemove = 40; break;
    case Difficulty::HARD: cellsToRemove = 50; break;
    default: cellsToRemove = 40;
    }

    while (cellsToRemove > 0) {
        int row = std::rand() % 9;
        int col = std::rand() % 9;

        if (board[row][col] != 0) {
            board[row][col] = 0;
            cellsToRemove--;
        }
    }
    std::memcpy(initialBoard, board, sizeof(board));
}

bool SudokuGame::isSafe(int row, int col, int num) {
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num || board[x][col] == num) return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) return false;
        }
    }
    return true;
}

bool SudokuGame::setValue(int row, int col, int value) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    if (value < 0 || value > 9) return false;
    if (initialBoard[row][col] != 0) return false;

    if (value == 0) {
        board[row][col] = 0;
        notifyBoardChanged();
        return true;
    }

    if (isValidMove(row, col, value)) {
        board[row][col] = value;
        notifyBoardChanged();

        if (isComplete()) {
            notifyGameComplete();
        }
        return true;
    }
    else {
        remainingAttempts--;
        notifyAttemptsChanged();
        return false;
    }
}

bool SudokuGame::isValidMove(int row, int col, int value) {
    int temp = board[row][col];
    board[row][col] = 0;
    bool safe = isSafe(row, col, value);
    board[row][col] = temp;
    return safe;
}

int SudokuGame::getValue(int row, int col) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return -1;
    return board[row][col];
}

bool SudokuGame::isComplete() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) return false;
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int temp = board[i][j];
            board[i][j] = 0;
            if (!isSafe(i, j, temp)) {
                board[i][j] = temp;
                return false;
            }
            board[i][j] = temp;
        }
    }
    return true;
}

int SudokuGame::getRemainingAttempts() {
    return remainingAttempts;
}

void SudokuGame::reset() {
    std::memcpy(board, initialBoard, sizeof(board));
    remainingAttempts = 3;
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::attachObserver(IObserver* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void SudokuGame::notifyBoardChanged() {
    for (auto observer : observers) {
        observer->onBoardChanged();
    }
}

void SudokuGame::notifyGameComplete() {
    for (auto observer : observers) {
        observer->onGameComplete();
    }
}

void SudokuGame::notifyAttemptsChanged() {
    for (auto observer : observers) {
        observer->onAttemptsChanged(remainingAttempts);
    }
}

void SudokuGame::fillBoard() {
    fillCell(0);
}