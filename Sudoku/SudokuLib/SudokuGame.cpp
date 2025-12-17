#include "SudokuGame.h"
#include <algorithm>
#include <vector>

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
    if (!isValidPosition(row, col)) return false;
    for (int x = 0; x < 9; ++x) {
        if (x != col && board.Get(row, x) == num) return false;
        if (x != row && board.Get(x, col) == num) return false;
    }
    int sr = row - row % 3, sc = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            int rr = sr + i, cc = sc + j;
            if (rr == row && cc == col) continue;
            if (board.Get(rr, cc) == num) return false;
        }

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
    }
    else {
        remainingAttempts--;
        notifyAttemptsChanged();
        if (remainingAttempts <= 0) {
            timer.Stop();
        }
        return false;
    }
}

bool SudokuGame::isValidMove(int row, int col, int value) const {
    if (!isValidPosition(row, col)) return false;
    if (value < 0 || value > 9) return false;
    if (board.IsInitial(row, col)) return false;
    if (value == 0) return true;
    if (!isSafe(row, col, value)) return false;
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
            int val = board.Get(r, c);
            if (val <= 0 || val > 9) return false;
            if (!isSafe(r, c, val)) return false;
        }

    return true;
}

int SudokuGame::getRemainingAttempts() const { return remainingAttempts; }
Difficulty SudokuGame::getCurrentDifficulty() const { return currentDifficulty; }

void SudokuGame::reset() {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (!board.IsInitial(r, c))
                board.Set(r, c, 0);

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
    if (!observer) return;
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) observers.push_back(observer);
}

void SudokuGame::detachObserver(IObserver* observer) {
    if (!observer) return;
    observers.remove(observer);
}

void SudokuGame::notifyBoardChanged() {
    std::vector<IObserver*> copy;
    copy.reserve(observers.size());
    for (auto* o : observers) if (o) copy.push_back(o);
    for (auto* obs : copy) if (obs) obs->onBoardChanged();
}

void SudokuGame::notifyGameComplete() {
    std::vector<IObserver*> copy;
    copy.reserve(observers.size());
    for (auto* o : observers) if (o) copy.push_back(o);
    for (auto* obs : copy) if (obs) obs->onGameComplete();
}

void SudokuGame::notifyAttemptsChanged() {
    std::vector<IObserver*> copy;
    copy.reserve(observers.size());
    for (auto* o : observers) if (o) copy.push_back(o);
    for (auto* obs : copy) if (obs) obs->onAttemptsChanged(remainingAttempts);
}

int SudokuGame::getSolutionValue(int row, int col) const {
    if (isValidPosition(row, col)) return board.GetSolution(row, col);
    return 0;
}