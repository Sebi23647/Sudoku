#pragma once

#include "ISudokuGame.h"
#include "IObserver.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class SudokuUI : public IObserver {
private:
    ISudokuGame* game;
    sf::RenderWindow& window;
    sf::Font font;

    int selectedRow;
    int selectedCol;
    bool gameWon;
    bool showDifficultyMenu;

    static constexpr float CELL_SIZE = 60.0f;
    static constexpr float BOARD_OFFSET_X = 50.0f;
    static constexpr float BOARD_OFFSET_Y = 80.0f;
    static constexpr float GRID_THICKNESS = 2.0f;
    static constexpr float THICK_LINE = 4.0f;

    sf::Color BACKGROUND_COLOR = sf::Color(250, 250, 250);
    sf::Color GRID_COLOR = sf::Color(40, 40, 40);
    sf::Color SELECTED_COLOR = sf::Color(180, 220, 255);
    sf::Color FIXED_COLOR = sf::Color(30, 30, 30);
    sf::Color FILLED_COLOR = sf::Color(50, 120, 200);

    void drawGrid();
    void drawNumbers();
    void drawSelection();
    void drawInfo();
    void drawDifficultyMenu();
    void drawGameOverlay();

    bool isMouseOverCell(int mouseX, int mouseY, int& row, int& col);

public:
    explicit SudokuUI(ISudokuGame* sudokuGame, sf::RenderWindow& win);

    bool loadFont(const std::string& fontPath);
    void render();
    void handleMouseClick(int mouseX, int mouseY);
    void handleKeyPress(sf::Keyboard::Key key);

    void onBoardChanged() override;
    void onGameComplete() override;
    void onAttemptsChanged(int remaining) override;
};