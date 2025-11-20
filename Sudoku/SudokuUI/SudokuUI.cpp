#include "SudokuUI.h"
#include "Difficulty.h"
#include "CellState.h"
#include <sstream>
#include <iomanip>

SudokuUI::SudokuUI(ISudokuGame* sudokuGame, sf::RenderWindow& win)
    : game(sudokuGame), window(win), selectedRow(-1), selectedCol(-1),
    gameWon(false), showDifficultyMenu(true) {
    if (game) {
        game->attachObserver(this);
    }
}

bool SudokuUI::loadFont(const std::string& fontPath) {
    return font.openFromFile(fontPath);
}

std::string SudokuUI::formatTime(int seconds) const {
    int mins = seconds / 60;
    int secs = seconds % 60;
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << mins << ":"
        << std::setfill('0') << std::setw(2) << secs;
    return oss.str();
}

void SudokuUI::drawGrid() {
    for (int i = 0; i <= 9; i++) {
        float thickness = (i % 3 == 0) ? THICK_LINE : GRID_THICKNESS;

        sf::RectangleShape hLine(sf::Vector2f(CELL_SIZE * 9, thickness));
        hLine.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE));
        hLine.setFillColor(GRID_COLOR);
        window.draw(hLine);

        sf::RectangleShape vLine(sf::Vector2f(thickness, CELL_SIZE * 9));
        vLine.setPosition(sf::Vector2f(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y));
        vLine.setFillColor(GRID_COLOR);
        window.draw(vLine);
    }
}

void SudokuUI::drawSelection() {
    if (selectedRow >= 0 && selectedRow < 9 && selectedCol >= 0 && selectedCol < 9) {
        sf::RectangleShape selection(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        selection.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + selectedCol * CELL_SIZE,
            BOARD_OFFSET_Y + selectedRow * CELL_SIZE
        ));
        selection.setFillColor(SELECTED_COLOR);
        window.draw(selection);
    }
}

void SudokuUI::drawHighlights() {
    // Evidențiază celulele cu aceeași valoare ca cea selectată
    if (selectedRow >= 0 && selectedRow < 9 && selectedCol >= 0 && selectedCol < 9) {
        int selectedValue = game->getValue(selectedRow, selectedCol);

        if (selectedValue != 0) {
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    if (game->getValue(row, col) == selectedValue) {
                        // Nu evidenția celula selectată (e deja evidențiată altfel)
                        if (row == selectedRow && col == selectedCol) continue;

                        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                        highlight.setPosition(sf::Vector2f(
                            BOARD_OFFSET_X + col * CELL_SIZE,
                            BOARD_OFFSET_Y + row * CELL_SIZE
                        ));
                        highlight.setFillColor(HIGHLIGHT_COLOR);
                        window.draw(highlight);
                    }
                }
            }
        }
    }
}

void SudokuUI::drawNumbers() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int value = game->getValue(row, col);
            if (value != 0) {
                CellState state = game->getCellState(row, col);

                sf::Text text(font);
                text.setString(std::to_string(value));
                text.setCharacterSize(32);

                if (state == CellState::FIXED) {
                    text.setFillColor(FIXED_COLOR);
                    text.setStyle(sf::Text::Bold);
                }
                else {
                    text.setFillColor(FILLED_COLOR);
                }

                sf::FloatRect bounds = text.getLocalBounds();
                text.setPosition(sf::Vector2f(
                    BOARD_OFFSET_X + col * CELL_SIZE + (CELL_SIZE - bounds.size.x) / 2,
                    BOARD_OFFSET_Y + row * CELL_SIZE + (CELL_SIZE - bounds.size.y) / 2 - 5
                ));

                window.draw(text);
            }
        }
    }
}

void SudokuUI::drawInfo() {
    sf::Text title(font);
    title.setString("SUDOKU");
    title.setCharacterSize(36);
    title.setFillColor(GRID_COLOR);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(BOARD_OFFSET_X, 20));
    window.draw(title);

    std::string diffStr;
    Difficulty diff = game->getCurrentDifficulty();
    switch (diff) {
    case Difficulty::EASY: diffStr = "Easy"; break;
    case Difficulty::MEDIUM: diffStr = "Medium"; break;
    case Difficulty::HARD: diffStr = "Hard"; break;
    }

    sf::Text info(font);
    info.setString(diffStr + " | Attempts: " + std::to_string(game->getRemainingAttempts()));
    info.setCharacterSize(18);
    info.setFillColor(GRID_COLOR);
    info.setPosition(sf::Vector2f(BOARD_OFFSET_X + 250, 30));
    window.draw(info);

    // Timer în dreapta sus
    sf::Text timer(font);
    timer.setString("Time: " + formatTime(game->getElapsedTime()));
    timer.setCharacterSize(20);
    timer.setFillColor(sf::Color(70, 130, 180));
    timer.setStyle(sf::Text::Bold);
    sf::FloatRect timerBounds = timer.getLocalBounds();
    timer.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - timerBounds.size.x - 10,
        25
    ));
    window.draw(timer);

    sf::Text controls(font);
    controls.setString("Press 1-9 to fill | 0/Delete to clear | Arrow keys to move");
    controls.setCharacterSize(14);
    controls.setFillColor(sf::Color(120, 120, 120));
    controls.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + CELL_SIZE * 9 + 15));
    window.draw(controls);

    sf::RectangleShape resetBtn(sf::Vector2f(100, 35));
    resetBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 100, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10));
    resetBtn.setFillColor(sf::Color(100, 150, 200));
    window.draw(resetBtn);

    sf::Text resetText(font);
    resetText.setString("New Game");
    resetText.setCharacterSize(16);
    resetText.setFillColor(sf::Color::White);
    sf::FloatRect resetBounds = resetText.getLocalBounds();
    resetText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 100 + (100 - resetBounds.size.x) / 2,
        BOARD_OFFSET_Y + CELL_SIZE * 9 + 10 + (35 - resetBounds.size.y) / 2 - 3
    ));
    window.draw(resetText);
}

void SudokuUI::drawDifficultyMenu() {
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(245, 245, 245));
    window.draw(overlay);

    sf::Text title(font);
    title.setString("Select Difficulty");
    title.setCharacterSize(48);
    title.setFillColor(GRID_COLOR);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 200));
    window.draw(title);

    float btnWidth = 200;
    float btnHeight = 60;
    float startY = 320;
    float spacing = 30;

    struct Button {
        std::string text;
        sf::Color color;
        float y;
    };

    Button buttons[] = {
        {"Easy", sf::Color(100, 200, 100), startY},
        {"Medium", sf::Color(200, 150, 70), startY + btnHeight + spacing},
        {"Hard", sf::Color(200, 70, 70), startY + 2 * (btnHeight + spacing)}
    };

    for (const auto& btn : buttons) {
        sf::RectangleShape rect(sf::Vector2f(btnWidth, btnHeight));
        rect.setPosition(sf::Vector2f(350 - btnWidth / 2, btn.y));
        rect.setFillColor(btn.color);
        window.draw(rect);

        sf::Text text(font);
        text.setString(btn.text);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(sf::Vector2f(
            350 - textBounds.size.x / 2,
            btn.y + (btnHeight - textBounds.size.y) / 2 - 5
        ));
        window.draw(text);
    }
}

void SudokuUI::drawGameOverlay() {
    if (gameWon) {
        sf::RectangleShape overlay(sf::Vector2f(500, 200));
        overlay.setPosition(sf::Vector2f(100, 300));
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        sf::Text winText(font);
        winText.setString("YOU WON!");
        winText.setCharacterSize(56);
        winText.setFillColor(sf::Color(100, 255, 100));
        winText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = winText.getLocalBounds();
        winText.setPosition(sf::Vector2f(350 - bounds.size.x / 2, 340));
        window.draw(winText);

        // Afișează timpul final
        sf::Text timeText(font);
        timeText.setString("Time: " + formatTime(game->getElapsedTime()));
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        sf::FloatRect timeBounds = timeText.getLocalBounds();
        timeText.setPosition(sf::Vector2f(350 - timeBounds.size.x / 2, 420));
        window.draw(timeText);
    }
}

void SudokuUI::render() {
    window.clear(BACKGROUND_COLOR);

    if (showDifficultyMenu) {
        drawDifficultyMenu();
    }
    else {
        drawSelection();
        drawHighlights();  // Desenează evidențierile ÎNAINTE de grid
        drawGrid();
        drawNumbers();
        drawInfo();
        drawGameOverlay();
    }
}

bool SudokuUI::isMouseOverCell(int mouseX, int mouseY, int& row, int& col) {
    if (mouseX < BOARD_OFFSET_X || mouseX > BOARD_OFFSET_X + CELL_SIZE * 9 ||
        mouseY < BOARD_OFFSET_Y || mouseY > BOARD_OFFSET_Y + CELL_SIZE * 9) {
        return false;
    }

    col = static_cast<int>((mouseX - BOARD_OFFSET_X) / CELL_SIZE);
    row = static_cast<int>((mouseY - BOARD_OFFSET_Y) / CELL_SIZE);

    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

void SudokuUI::handleMouseClick(int mouseX, int mouseY) {
    if (showDifficultyMenu) {
        float btnWidth = 200;
        float btnHeight = 60;
        float startY = 320;
        float spacing = 30;
        float centerX = 350;

        sf::FloatRect easyRect(sf::Vector2f(centerX - btnWidth / 2, startY), sf::Vector2f(btnWidth, btnHeight));
        sf::FloatRect mediumRect(sf::Vector2f(centerX - btnWidth / 2, startY + btnHeight + spacing), sf::Vector2f(btnWidth, btnHeight));
        sf::FloatRect hardRect(sf::Vector2f(centerX - btnWidth / 2, startY + 2 * (btnHeight + spacing)), sf::Vector2f(btnWidth, btnHeight));

        sf::Vector2f mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));

        if (easyRect.contains(mousePos)) {
            game->startNewGame(Difficulty::EASY);
            showDifficultyMenu = false;
        }
        else if (mediumRect.contains(mousePos)) {
            game->startNewGame(Difficulty::MEDIUM);
            showDifficultyMenu = false;
        }
        else if (hardRect.contains(mousePos)) {
            game->startNewGame(Difficulty::HARD);
            showDifficultyMenu = false;
        }
        return;
    }

    int row, col;
    if (isMouseOverCell(mouseX, mouseY, row, col)) {
        selectedRow = row;
        selectedCol = col;
        return;
    }

    sf::FloatRect resetRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 100, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10),
        sf::Vector2f(100, 35)
    );

    if (resetRect.contains(sf::Vector2f(static_cast<float>(mouseX), static_cast<float>(mouseY)))) {
        showDifficultyMenu = true;
        gameWon = false;
        selectedRow = -1;
        selectedCol = -1;
    }
}

void SudokuUI::handleKeyPress(sf::Keyboard::Key key) {
    if (showDifficultyMenu || selectedRow < 0 || selectedCol < 0) return;

    if (key >= sf::Keyboard::Key::Num1 && key <= sf::Keyboard::Key::Num9) {
        int value = static_cast<int>(key) - static_cast<int>(sf::Keyboard::Key::Num0);
        game->setValue(selectedRow, selectedCol, value);
    }
    else if (key >= sf::Keyboard::Key::Numpad1 && key <= sf::Keyboard::Key::Numpad9) {
        int value = static_cast<int>(key) - static_cast<int>(sf::Keyboard::Key::Numpad0);
        game->setValue(selectedRow, selectedCol, value);
    }
    else if (key == sf::Keyboard::Key::Num0 || key == sf::Keyboard::Key::Numpad0 ||
        key == sf::Keyboard::Key::Delete || key == sf::Keyboard::Key::Backspace) {
        game->setValue(selectedRow, selectedCol, 0);
    }
    else if (key == sf::Keyboard::Key::Up && selectedRow > 0) {
        selectedRow--;
    }
    else if (key == sf::Keyboard::Key::Down && selectedRow < 8) {
        selectedRow++;
    }
    else if (key == sf::Keyboard::Key::Left && selectedCol > 0) {
        selectedCol--;
    }
    else if (key == sf::Keyboard::Key::Right && selectedCol < 8) {
        selectedCol++;
    }
}

void SudokuUI::onBoardChanged() {
}

void SudokuUI::onGameComplete() {
    gameWon = true;
}

void SudokuUI::onAttemptsChanged(int remaining) {
}