#include "SudokuGenerator.h"

void SudokuGenerator::Generate(SudokuBoard& board, Difficulty difficulty) {
    board.Clear();
    FillCell(board, 0);
    SaveSolution(board);
    RemoveCells(board, difficulty);
}

bool SudokuGenerator::FillCell(SudokuBoard& board, int pos) {
    if (pos == 81) return true;
    int r = pos / 9, c = pos % 9;
    std::vector<int> nums{1,2,3,4,5,6,7,8,9};
    std::shuffle(nums.begin(), nums.end(), std::default_random_engine(std::rand()));
    for (int n : nums) {
        if (IsSafe(board, r, c, n)) {
            board.Set(r, c, n);
            if (FillCell(board, pos + 1)) return true;
            board.Set(r, c, 0);
        }
    }
    return false;
}

bool SudokuGenerator::IsSafe(const SudokuBoard& board, int row, int col, int num) const {
    for (int x = 0; x < 9; ++x) {
        if (board.Get(row, x) == num || board.Get(x, col) == num) return false;
    }
    int sr = row - row % 3, sc = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board.Get(i + sr, j + sc) == num) return false;
    return true;
}

void SudokuGenerator::SaveSolution(SudokuBoard& board) {
    int temp[9][9];
    board.CopyTo(temp);
    board.SetSolution(temp);
}

bool SudokuGenerator::SolveSudoku(int grid[9][9], int pos, int& solutionCount, int limit) {
    if (solutionCount > limit) return true;
    if (pos == 81) { ++solutionCount; return solutionCount > limit; }
    int r = pos / 9, c = pos % 9;
    if (grid[r][c] != 0) return SolveSudoku(grid, pos + 1, solutionCount, limit);
    for (int n = 1; n <= 9; ++n) {
        bool safe = true;
        for (int x = 0; x < 9; ++x) {
            if (grid[r][x] == n || grid[x][c] == n) { safe = false; break; }
        }
        if (safe) {
            int sr = r - r % 3, sc = c - c % 3;
            for (int i = 0; i < 3 && safe; ++i)
                for (int j = 0; j < 3 && safe; ++j)
                    if (grid[i + sr][j + sc] == n) safe = false;
        }
        if (safe) {
            grid[r][c] = n;
            if (SolveSudoku(grid, pos + 1, solutionCount, limit)) {
                grid[r][c] = 0;
                return true;
            }
            grid[r][c] = 0;
        }
    }
    return false;
}

int SudokuGenerator::CountSolutions(const int src[9][9]) {
    int work[9][9];
    std::memcpy(work, src, sizeof(int) * 81);
    int solutionCount = 0;
    SolveSudoku(work, 0, solutionCount, 1);
    return solutionCount;
}

bool SudokuGenerator::HasUniqueSolution(const int test[9][9]) {
    return CountSolutions(test) == 1;
}

void SudokuGenerator::RemoveCells(SudokuBoard& board, Difficulty difficulty) {
    int cellsToRemove = 40;
    switch (difficulty) {
        case Difficulty::EASY: cellsToRemove = 30; break;
        case Difficulty::MEDIUM: cellsToRemove = 40; break;
        case Difficulty::HARD: cellsToRemove = 50; break;
        default: break;
    }

    board.MarkAllInitial(false);

    std::vector<std::pair<int,int>> cells;
    cells.reserve(81);
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            cells.emplace_back(r, c);

    std::shuffle(cells.begin(), cells.end(), std::default_random_engine(std::rand()));

    int removed = 0;
    for (const auto& p : cells) {
        if (removed >= cellsToRemove) break;
        int r = p.first, c = p.second;
        if (board.Get(r, c) == 0) continue;
        int backup = board.Get(r, c);
        board.Set(r, c, 0);

        int test[9][9];
        board.CopyTo(test);

        if (HasUniqueSolution(test)) {
            ++removed;
        } else {
            board.Set(r, c, backup);
        }
    }

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (board.Get(r, c) != 0)
                board.MarkInitial(r, c, true);
}