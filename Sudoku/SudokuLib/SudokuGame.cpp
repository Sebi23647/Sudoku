#include "SudokuGame.h"
#include <algorithm>
#include <random>
#include <cstring>
#include <ctime>

SudokuGame::SudokuGame() : currentDifficulty(Difficulty::MEDIUM), remainingAttempts(3),
timerRunning(false), elapsedSeconds(0) {
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

SudokuGame::SudokuGame(Difficulty difficulty) : currentDifficulty(difficulty), remainingAttempts(3),
timerRunning(false), elapsedSeconds(0) {
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void SudokuGame::startNewGame() {
    remainingAttempts = 3;
    resetTimer();
    generatePuzzle();
    startTimer();
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::startNewGame(Difficulty difficulty) {
    currentDifficulty = difficulty;
    startNewGame();
}

void SudokuGame::generatePuzzle() {
    std::memset(board, 0, sizeof(board));
    std::memset(solutionBoard, 0, sizeof(solutionBoard));
    std::memset(initialCells, false, sizeof(initialCells));

    fillBoard();

    saveSolution();

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

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != 0) {
                initialCells[i][j] = true;
            }
        }
    }
}

bool SudokuGame::isSafe(int row, int col, int num) const {
    for (int x = 0; x < 9; x++) {
        if (board[row][x] == num || board[x][col] == num) {
            return false;
        }
    }

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
    if (!isValidPosition(row, col)) {
        return false;
    }

    if (value < 0 || value > 9) {
        return false;
    }

    if (initialCells[row][col]) {
        return false;
    }

    if (value == 0) {
        board[row][col] = 0;
        notifyBoardChanged();
        return true;
    }

    if (isValidMove(row, col, value)) {
        board[row][col] = value;
        notifyBoardChanged();

        if (isComplete()) {
            stopTimer();
            notifyGameComplete();
        }
        return true;
    }
    else {
        remainingAttempts--;
        notifyAttemptsChanged();

        if (remainingAttempts <= 0) {
            stopTimer();
        }

        return false;
    }
}

bool SudokuGame::isValidMove(int row, int col, int value) const {
    int temp = board[row][col];

    const_cast<int&>(board[row][col]) = 0;
    bool safe = isSafe(row, col, value);

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
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }

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
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (!initialCells[i][j]) {
                board[i][j] = 0;
            }
        }
    }

    remainingAttempts = 3;
    resetTimer();
    startTimer();
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

void SudokuGame::startTimer() {
    startTime = std::chrono::steady_clock::now();
    timerRunning = true;
}

void SudokuGame::stopTimer() {
    if (timerRunning) {
        auto now = std::chrono::steady_clock::now();
        elapsedSeconds += std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        timerRunning = false;
    }
}

void SudokuGame::resetTimer() {
    elapsedSeconds = 0;
    timerRunning = false;
}

int SudokuGame::getElapsedTime() const {
    if (timerRunning) {
        auto now = std::chrono::steady_clock::now();
        int current = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        return elapsedSeconds + current;
    }
    return elapsedSeconds;
}