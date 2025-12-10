#pragma once
#include "IScreen.h"
#include "IObserver.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../SudokuLib/Hint.h"

class ScreenManager;
class ISudokuGame;

class GameScreen : public IObserver, public IScreen {
private:
    ScreenManager& manager;
    ISudokuGame* game;
    sf::RenderWindow& window;
    sf::Font& font;
    int selectedRow;
    int selectedCol;
    bool gameWon;
    bool gameLost;

    std::optional<Hint> currentHint;

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
    sf::Color PRIMARY_COLOR = sf::Color(99, 102, 241);
    sf::Color SUCCESS_COLOR = sf::Color(34, 197, 94);
    sf::Color WARNING_COLOR = sf::Color(251, 146, 60);
    sf::Color DANGER_COLOR = sf::Color(239, 68, 68);
    sf::Color HINT_COLOR = sf::Color(16, 185, 129); // teal green

    void drawGrid();
    void drawNumbers();
    void drawSelection();
    void drawHighlights();
    void drawInfo();
    void drawGameOverlay();
    bool isMouseOverCell(int mouseX, int mouseY, int& row, int& col);

    void drawHintOverlay();

public:
    explicit GameScreen(ScreenManager& mgr, sf::RenderWindow& win);
    void render(sf::RenderWindow& window) override;
    void handleMouseClick(int mouseX, int mouseY) override;
    void handleKeyPress(sf::Keyboard::Key key) override;
    void handleTextInput(std::uint32_t unicode) override;

    void onBoardChanged() override;
    void onGameComplete() override;
    void onAttemptsChanged(int remaining) override;
};