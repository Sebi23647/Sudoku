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
    bool gameLost;
    bool showDifficultyMenu;
    bool showUsernameInput;
    bool showLeaderboard;
    int leaderboardTab;
    std::string username;
    std::string inputBuffer;

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

    void drawGrid();
    void drawNumbers();
    void drawSelection();
    void drawHighlights();
    void drawInfo();
    void drawDifficultyMenu();
    void drawUsernameInput();
    void drawLeaderboard();
    void drawGameOverlay();

    bool isMouseOverCell(int mouseX, int mouseY, int& row, int& col);
    std::string formatTime(int seconds) const;
    void saveScoreToFile();
    void updateOrAddScore(const std::string& user, const std::string& diff, int time);

public:
    explicit SudokuUI(ISudokuGame* sudokuGame, sf::RenderWindow& win);

    bool loadFont(const std::string& fontPath);
    void render();
    void handleMouseClick(int mouseX, int mouseY);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleTextInput(std::uint32_t unicode);

    void onBoardChanged() override;
    void onGameComplete() override;
    void onAttemptsChanged(int remaining) override;
};