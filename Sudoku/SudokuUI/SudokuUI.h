#ifndef SUDOKUUI_H
#define SUDOKUUI_H

#include "ISudokuGame.h"
#include "IObserver.h"
#include <SFML/Graphics.hpp>
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
    static constexpr float BOARD_OFFSET_Y = 100.0f;
    static constexpr float GRID_THICKNESS = 2.0f;
    static constexpr float THICK_LINE = 4.0f;

    const sf::Color BACKGROUND_COLOR = sf::Color(245, 245, 245);
    const sf::Color GRID_COLOR = sf::Color(50, 50, 50);
    const sf::Color SELECTED_COLOR = sf::Color(173, 216, 230, 150);
    const sf::Color HIGHLIGHT_COLOR = sf::Color(200, 230, 255, 100);  // Pentru evidențiere
    const sf::Color FIXED_COLOR = sf::Color(20, 20, 20);
    const sf::Color FILLED_COLOR = sf::Color(70, 130, 180);

    void drawGrid();
    void drawSelection();
    void drawHighlights();  // Nouă funcție pentru evidențiere
    void drawNumbers();
    void drawInfo();
    void drawDifficultyMenu();
    void drawGameOverlay();
    bool isMouseOverCell(int mouseX, int mouseY, int& row, int& col);
    std::string formatTime(int seconds) const;  // Nouă funcție pentru formatare timp

public:
    SudokuUI(ISudokuGame* sudokuGame, sf::RenderWindow& win);
    bool loadFont(const std::string& fontPath);
    void render();
    void handleMouseClick(int mouseX, int mouseY);
    void handleKeyPress(sf::Keyboard::Key key);

    void onBoardChanged() override;
    void onGameComplete() override;
    void onAttemptsChanged(int remaining) override;
};

#endif