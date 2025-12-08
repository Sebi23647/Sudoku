#include "SudokuGame.h"
#include <algorithm>
#include <random>
#include <ctime>

SudokuGame::SudokuGame()
    : currentDifficulty(Difficulty::MEDIUM),
      remainingAttempts(3) {
    timer.Reset();
}

SudokuGame::SudokuGame(Difficulty difficulty)
    : currentDifficulty(difficulty),
      remainingAttempts(3) {
    timer.Reset();
}

void SudokuGame::startNewGame() {
    remainingAttempts = 3;
    timer.Reset();
    generatePuzzle();
    timer.Start();
    notifyBoardChanged();
    notifyAttemptsChanged();
}

void SudokuGame::startNewGame(Difficulty difficulty) {
    currentDifficulty = difficulty;
    startNewGame();
}

void SudokuGame::generatePuzzle() {
    generator.Generate(board, currentDifficulty);
}

bool SudokuGame::isSafe(int row, int col, int num) const {
    for (int x = 0; x < 9; ++x)
        if (board.Get(row, x) == num || board.Get(x, col) == num)
            return false;

    int sr = row - row % 3, sc = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board.Get(i + sr, j + sc) == num)
                return false;

    return true;
}

bool SudokuGame::isValidPosition(int row, int col) const {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

bool SudokuGame::setValue(int row, int col, int value) {
    if (!isValidPosition(row, col)) return false;
    if (value < 0 || value > 9) return false;
    if (board.IsInitial(row, col)) return false;

    if (value == 0) {
        board.Set(row, col, 0);
        notifyBoardChanged();
        return true;
    }

    if (isValidMove(row, col, value)) {
        board.Set(row, col, value);
        notifyBoardChanged();
        if (isComplete()) {
            timer.Stop();
            notifyGameComplete();
        }
        return true;
    } else {
        remainingAttempts--;
        notifyAttemptsChanged();
        if (remainingAttempts <= 0) {
            timer.Stop();
        }
        return false;
    }
}

bool SudokuGame::isValidMove(int row, int col, int value) const {
    const int temp = board.Get(row, col);
    const_cast<SudokuBoard&>(board).Set(row, col, 0);
    const bool followsRules = isSafe(row, col, value);
    const_cast<SudokuBoard&>(board).Set(row, col, temp);
    if (!followsRules) return false;
    return value == board.GetSolution(row, col);
}

int SudokuGame::getValue(int row, int col) const {
    if (!isValidPosition(row, col)) return -1;
    return board.Get(row, col);
}

CellState SudokuGame::getCellState(int row, int col) const {
    if (!isValidPosition(row, col)) return CellState::EMPTY;
    return board.GetCellState(row, col);
}

bool SudokuGame::isComplete() const {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (board.Get(r, c) == 0) return false;

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c) {
            const int temp = board.Get(r, c);
            const_cast<SudokuBoard&>(board).Set(r, c, 0);
            if (!isSafe(r, c, temp)) {
                const_cast<SudokuBoard&>(board).Set(r, c, temp);
                return false;
            }
            const_cast<SudokuBoard&>(board).Set(r, c, temp);
        }

    return true;
}

int SudokuGame::getRemainingAttempts() const { return remainingAttempts; }
Difficulty SudokuGame::getCurrentDifficulty() const { return currentDifficulty; }

void SudokuGame::reset() {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (!board.IsInitial(r, c))
                const_cast<SudokuBoard&>(board).Set(r, c, 0);

    remainingAttempts = 3;
    timer.Reset();
    timer.Start();
    notifyBoardChanged();
    notifyAttemptsChanged();
}

int SudokuGame::getElapsedTime() const {
    return timer.GetElapsedSeconds();
}

void SudokuGame::attachObserver(IObserver* observer) {
    if (observer) observers.push_back(observer);
}

void SudokuGame::detachObserver(IObserver* observer) {
    if (!observer) return;
    observers.remove(observer);
}

void SudokuGame::notifyBoardChanged() {
    for (auto* obs : observers) if (obs) obs->onBoardChanged();
}

void SudokuGame::notifyGameComplete() {
    for (auto* obs : observers) if (obs) obs->onGameComplete();
}

void SudokuGame::notifyAttemptsChanged() {
    for (auto* obs : observers) if (obs) obs->onAttemptsChanged(remainingAttempts);
}

int SudokuGame::getSolutionValue(int row, int col) const {
    if (isValidPosition(row, col)) return board.GetSolution(row, col);
    return 0;
}