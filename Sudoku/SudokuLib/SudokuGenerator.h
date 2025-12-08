#pragma once
#include "Difficulty.h"
#include "SudokuBoard.h"
#include <vector>
#include <random>
#include <algorithm>
#include <cstring>

class SudokuGenerator {
public:
    void Generate(SudokuBoard& board, Difficulty difficulty);

private:
    bool FillCell(SudokuBoard& board, int pos);
    bool IsSafe(const SudokuBoard& board, int row, int col, int num) const;
    void SaveSolution(SudokuBoard& board);
    void RemoveCells(SudokuBoard& board, Difficulty difficulty);

    bool SolveSudoku(int grid[9][9], int pos, int& solutionCount, int limit);
    int CountSolutions(const int src[9][9]);
    bool HasUniqueSolution(const int test[9][9]);
};