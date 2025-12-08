#pragma once
#include "CellState.h"

class SudokuBoard {
public:
    SudokuBoard();

    void Clear();

    int Get(int r, int c) const;
    void Set(int r, int c, int v);

    bool IsInitial(int r, int c) const;
    void MarkInitial(int r, int c, bool value);
    void MarkAllInitial(bool value);

    void CopyTo(int out[9][9]) const;
    void SetSolution(const int src[9][9]);
    int GetSolution(int r, int c) const;

    CellState GetCellState(int r, int c) const;

private:
    int board[9][9];
    bool initialCells[9][9];
    int solution[9][9];
};