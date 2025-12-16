#include "SudokuBoard.h"
#include <algorithm>
#include <cstring>

SudokuBoard::SudokuBoard() {
    Clear();
    std::fill(&initialCells[0][0], &initialCells[0][0] + 81, false);
    std::fill(&solution[0][0], &solution[0][0] + 81, 0);
}

void SudokuBoard::Clear() {
    std::fill(&board[0][0], &board[0][0] + 81, 0);
}

int SudokuBoard::Get(int r, int c) const {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return 0;
    return board[r][c];
}

void SudokuBoard::Set(int r, int c, int v) {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return;
    board[r][c] = v;
}

bool SudokuBoard::IsInitial(int r, int c) const {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return false;
    return initialCells[r][c];
}

void SudokuBoard::MarkInitial(int r, int c, bool value) {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return;
    initialCells[r][c] = value;
}

void SudokuBoard::MarkAllInitial(bool value) {
    std::fill(&initialCells[0][0], &initialCells[0][0] + 81, value);
}

void SudokuBoard::CopyTo(int out[9][9]) const {
    std::memcpy(out, board, sizeof(board));
}

void SudokuBoard::SetSolution(const int src[9][9]) {
    std::memcpy(solution, src, sizeof(solution));
}

int SudokuBoard::GetSolution(int r, int c) const {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return 0;
    return solution[r][c];
}

CellState SudokuBoard::GetCellState(int r, int c) const {
    if (r < 0 || r >= 9 || c < 0 || c >= 9) return CellState::EMPTY;
    if (board[r][c] == 0) return CellState::EMPTY;
    if (initialCells[r][c]) return CellState::FIXED;
    return CellState::FILLED;
}