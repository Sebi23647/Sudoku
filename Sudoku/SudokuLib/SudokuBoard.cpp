#include "SudokuBoard.h"
#include <cstring>

SudokuBoard::SudokuBoard() {
    Clear();
    std::memset(initialCells, 0, sizeof(initialCells));
    std::memset(solution, 0, sizeof(solution));
}

void SudokuBoard::Clear() { std::memset(board, 0, sizeof(board)); }

int SudokuBoard::Get(int r, int c) const { return board[r][c]; }
void SudokuBoard::Set(int r, int c, int v) { board[r][c] = v; }

bool SudokuBoard::IsInitial(int r, int c) const { return initialCells[r][c]; }
void SudokuBoard::MarkInitial(int r, int c, bool value) { initialCells[r][c] = value; }
void SudokuBoard::MarkAllInitial(bool value) { std::memset(initialCells, value ? 1 : 0, sizeof(initialCells)); }

void SudokuBoard::CopyTo(int out[9][9]) const { std::memcpy(out, board, sizeof(board)); }
void SudokuBoard::SetSolution(const int src[9][9]) { std::memcpy(solution, src, sizeof(solution)); }
int SudokuBoard::GetSolution(int r, int c) const { return solution[r][c]; }

CellState SudokuBoard::GetCellState(int r, int c) const {
    if (board[r][c] == 0) return CellState::EMPTY;
    if (initialCells[r][c]) return CellState::FIXED;
    return CellState::FILLED;
}