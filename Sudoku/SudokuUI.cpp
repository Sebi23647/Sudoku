#include "SudokuUI.h"
#include <iostream>
#include <iomanip>

SudokuUI::SudokuUI(ISudokuGame* sudokuGame) : game(sudokuGame) {
    game->attachObserver(this);
}

void SudokuUI::render() {
    displayMenu();
    displayGameInfo();
    displayBoard();
}

void SudokuUI::displayMenu() {
    std::cout << "\n========== SUDOKU GAME ==========\n";
    std::cout << "Commands:\n";
    std::cout << "  N - New Game\n";
    std::cout << "  R - Reset Current Game\n";
    std::cout << "  Q - Quit\n";
    std::cout << "  To set value: row col value (e.g., 0 0 5)\n";
    std::cout << "================================\n\n";
}

void SudokuUI::displayGameInfo() {
    std::cout << "Remaining Attempts: " << game->getRemainingAttempts() << "\n\n";
}

void SudokuUI::displayBoard() {
    std::cout << "    0 1 2   3 4 5   6 7 8\n";
    std::cout << "  -------------------------\n";

    for (int i = 0; i < 9; i++) {
        if (i == 3 || i == 6) {
            std::cout << "  -------------------------\n";
        }

        std::cout << i << " | ";
        for (int j = 0; j < 9; j++) {
            if (j == 3 || j == 6) {
                std::cout << "| ";
            }

            int value = game->getValue(i, j);
            if (value == 0) {
                std::cout << ". ";
            }
            else {
                std::cout << value << " ";
            }
        }
        std::cout << "|\n";
    }
    std::cout << "  -------------------------\n\n";
}

void SudokuUI::handleClick(int row, int col) {
    int value;
    std::cout << "Enter value (1-9, 0 to clear): ";
    std::cin >> value;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input!\n";
        return;
    }

    if (!game->setValue(row, col, value)) {
        std::cout << "Invalid move! Attempts remaining: "
            << game->getRemainingAttempts() << "\n";
    }
}

void SudokuUI::onBoardChanged() {
    std::cout << "\n[Board Updated]\n";
}

void SudokuUI::onGameComplete() {
    std::cout << "\n*** CONGRATULATIONS! You completed the puzzle! ***\n";
}

void SudokuUI::onAttemptsChanged(int remaining) {
    std::cout << "[Attempts remaining: " << remaining << "]\n";
    if (remaining == 0) {
        std::cout << "*** GAME OVER! No more attempts! ***\n";
    }
}