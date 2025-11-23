#include "SudokuUI.h" 
#include "Difficulty.h"
#include "CellState.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

SudokuUI::SudokuUI(ISudokuGame* sudokuGame, sf::RenderWindow& win)
    : game(sudokuGame), window(win), selectedRow(-1), selectedCol(-1),
    gameWon(false), showDifficultyMenu(false), showUsernameInput(true),
    showLeaderboard(false), username(""), inputBuffer("") {
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

void SudokuUI::saveScoreToFile() {
    std::ofstream file("leaderboard.txt", std::ios::app);
    if (file.is_open()) {
        std::string diffStr;
        Difficulty diff = game->getCurrentDifficulty();
        switch (diff) {
        case Difficulty::EASY: diffStr = "Easy"; break;
        case Difficulty::MEDIUM: diffStr = "Medium"; break;
        case Difficulty::HARD: diffStr = "Hard"; break;
        }

        file << username << "," << diffStr << "," << game->getElapsedTime() << "\n";
        file.close();
    }
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
    if (selectedRow >= 0 && selectedRow < 9 && selectedCol >= 0 && selectedCol < 9) {
        int selectedValue = game->getValue(selectedRow, selectedCol);

        if (selectedValue != 0) {
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    if (game->getValue(row, col) == selectedValue) {
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

    // Buton New Game
    sf::RectangleShape resetBtn(sf::Vector2f(100, 35));
    resetBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 220, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10));
    resetBtn.setFillColor(sf::Color(100, 150, 200));
    window.draw(resetBtn);

    sf::Text resetText(font);
    resetText.setString("New Game");
    resetText.setCharacterSize(16);
    resetText.setFillColor(sf::Color::White);
    sf::FloatRect resetBounds = resetText.getLocalBounds();
    resetText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 220 + (100 - resetBounds.size.x) / 2,
        BOARD_OFFSET_Y + CELL_SIZE * 9 + 10 + (35 - resetBounds.size.y) / 2 - 3
    ));
    window.draw(resetText);

    // Buton Leaderboard
    sf::RectangleShape leaderBtn(sf::Vector2f(110, 35));
    leaderBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10));
    leaderBtn.setFillColor(sf::Color(200, 150, 70));
    window.draw(leaderBtn);

    sf::Text leaderText(font);
    leaderText.setString("Leaderboard");
    leaderText.setCharacterSize(16);
    leaderText.setFillColor(sf::Color::White);
    sf::FloatRect leaderBounds = leaderText.getLocalBounds();
    leaderText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 110 + (110 - leaderBounds.size.x) / 2,
        BOARD_OFFSET_Y + CELL_SIZE * 9 + 10 + (35 - leaderBounds.size.y) / 2 - 3
    ));
    window.draw(leaderText);
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

void SudokuUI::drawUsernameInput() {
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(245, 245, 245));
    window.draw(overlay);

    sf::Text title(font);
    title.setString("Enter Your Username");
    title.setCharacterSize(42);
    title.setFillColor(GRID_COLOR);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 250));
    window.draw(title);

    // Input box
    sf::RectangleShape inputBox(sf::Vector2f(400, 60));
    inputBox.setPosition(sf::Vector2f(150, 350));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(100, 150, 200));
    inputBox.setOutlineThickness(3);
    window.draw(inputBox);

    // Text în input
    sf::Text inputText(font);
    inputText.setString(inputBuffer);
    inputText.setCharacterSize(28);
    inputText.setFillColor(GRID_COLOR);
    inputText.setPosition(sf::Vector2f(160, 360));
    window.draw(inputText);

    // Cursor blink
    sf::Text cursor(font);
    cursor.setString("|");
    cursor.setCharacterSize(28);
    cursor.setFillColor(GRID_COLOR);
    sf::FloatRect inputBounds = inputText.getLocalBounds();
    cursor.setPosition(sf::Vector2f(165 + inputBounds.size.x, 360));
    window.draw(cursor);

    // Start button
    sf::RectangleShape startBtn(sf::Vector2f(150, 50));
    startBtn.setPosition(sf::Vector2f(275, 450));
    startBtn.setFillColor(sf::Color(100, 200, 100));
    window.draw(startBtn);

    sf::Text startText(font);
    startText.setString("Start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setPosition(sf::Vector2f(
        275 + (150 - startBounds.size.x) / 2,
        450 + (50 - startBounds.size.y) / 2 - 3
    ));
    window.draw(startText);
}

void SudokuUI::drawLeaderboard() {
    // Overlay semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    // Fereastră leaderboard
    sf::RectangleShape leaderWindow(sf::Vector2f(600, 650));
    leaderWindow.setPosition(sf::Vector2f(50, 75));
    leaderWindow.setFillColor(sf::Color(245, 245, 245));
    leaderWindow.setOutlineColor(GRID_COLOR);
    leaderWindow.setOutlineThickness(4);
    window.draw(leaderWindow);

    // Titlu
    sf::Text title(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(48);
    title.setFillColor(GRID_COLOR);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 100));
    window.draw(title);

    // Header
    sf::Text header(font);
    header.setString("Rank   Username        Difficulty   Time");
    header.setCharacterSize(20);
    header.setFillColor(GRID_COLOR);
    header.setStyle(sf::Text::Bold);
    header.setPosition(sf::Vector2f(100, 180));
    window.draw(header);

    // Citește scorurile din fișier
    std::vector<std::tuple<std::string, std::string, int>> scores;
    std::ifstream file("leaderboard.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string user, diff, timeStr;
            if (std::getline(ss, user, ',') &&
                std::getline(ss, diff, ',') &&
                std::getline(ss, timeStr)) {
                int time = std::stoi(timeStr);
                scores.push_back(std::make_tuple(user, diff, time));
            }
        }
        file.close();
    }

    // Sortează după timp (crescător)
    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) {
            return std::get<2>(a) < std::get<2>(b);
        });

    // Afișează top 10
    float yPos = 230;
    int rank = 1;
    for (size_t i = 0; i < std::min(scores.size(), size_t(10)); ++i) {
        std::ostringstream oss;
        oss << rank << ".     "
            << std::left << std::setw(15) << std::get<0>(scores[i])
            << std::setw(10) << std::get<1>(scores[i])
            << "   " << formatTime(std::get<2>(scores[i]));

        sf::Text entry(font);
        entry.setString(oss.str());
        entry.setCharacterSize(18);
        entry.setFillColor(GRID_COLOR);
        entry.setPosition(sf::Vector2f(100, yPos));
        window.draw(entry);
        yPos += 40;
        rank++;
    }

    // Dacă nu sunt scoruri
    if (scores.empty()) {
        sf::Text noData(font);
        noData.setString("No scores yet! Play a game to start.");
        noData.setCharacterSize(20);
        noData.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect noBounds = noData.getLocalBounds();
        noData.setPosition(sf::Vector2f(350 - noBounds.size.x / 2, 350));
        window.draw(noData);
    }

    // Buton Close
    sf::RectangleShape closeBtn(sf::Vector2f(120, 45));
    closeBtn.setPosition(sf::Vector2f(290, 650));
    closeBtn.setFillColor(sf::Color(200, 70, 70));
    window.draw(closeBtn);

    sf::Text closeText(font);
    closeText.setString("Close");
    closeText.setCharacterSize(22);
    closeText.setFillColor(sf::Color::White);
    closeText.setStyle(sf::Text::Bold);
    sf::FloatRect closeBounds = closeText.getLocalBounds();
    closeText.setPosition(sf::Vector2f(
        290 + (120 - closeBounds.size.x) / 2,
        650 + (45 - closeBounds.size.y) / 2 - 3
    ));
    window.draw(closeText);
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

    if (showLeaderboard) {
        drawSelection();
        drawHighlights();
        drawGrid();
        drawNumbers();
        drawInfo();
        drawLeaderboard();
    }
    else if (showUsernameInput) {
        drawUsernameInput();
    }
    else if (showDifficultyMenu) {
        drawDifficultyMenu();
    }
    else {
        drawSelection();
        drawHighlights();
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
    sf::Vector2f mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));

    // Leaderboard window
    if (showLeaderboard) {
        sf::FloatRect closeRect(sf::Vector2f(290, 650), sf::Vector2f(120, 45));
        if (closeRect.contains(mousePos)) {
            showLeaderboard = false;
        }
        return;
    }

    // Username input
    if (showUsernameInput) {
        sf::FloatRect startRect(sf::Vector2f(275, 450), sf::Vector2f(150, 50));
        if (startRect.contains(mousePos) && !inputBuffer.empty()) {
            username = inputBuffer;
            showUsernameInput = false;
            showDifficultyMenu = true;
        }
        return;
    }

    // Difficulty menu
    if (showDifficultyMenu) {
        float btnWidth = 200;
        float btnHeight = 60;
        float startY = 320;
        float spacing = 30;
        float centerX = 350;

        sf::FloatRect easyRect(sf::Vector2f(centerX - btnWidth / 2, startY), sf::Vector2f(btnWidth, btnHeight));
        sf::FloatRect mediumRect(sf::Vector2f(centerX - btnWidth / 2, startY + btnHeight + spacing), sf::Vector2f(btnWidth, btnHeight));
        sf::FloatRect hardRect(sf::Vector2f(centerX - btnWidth / 2, startY + 2 * (btnHeight + spacing)), sf::Vector2f(btnWidth, btnHeight));

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

    // Game board
    int row, col;
    if (isMouseOverCell(mouseX, mouseY, row, col)) {
        selectedRow = row;
        selectedCol = col;
        return;
    }

    // Buton New Game
    sf::FloatRect resetRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 220, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10),
        sf::Vector2f(100, 35)
    );

    if (resetRect.contains(mousePos)) {
        showUsernameInput = true;
        showDifficultyMenu = false;
        gameWon = false;
        selectedRow = -1;
        selectedCol = -1;
        inputBuffer = "";
        return;
    }

    // Buton Leaderboard
    sf::FloatRect leaderRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, BOARD_OFFSET_Y + CELL_SIZE * 9 + 10),
        sf::Vector2f(110, 35)
    );

    if (leaderRect.contains(mousePos)) {
        showLeaderboard = true;
    }
}

void SudokuUI::handleKeyPress(sf::Keyboard::Key key) {
    if (showUsernameInput) {
        if (key == sf::Keyboard::Key::Enter && !inputBuffer.empty()) {
            username = inputBuffer;
            showUsernameInput = false;
            showDifficultyMenu = true;
        }
        else if (key == sf::Keyboard::Key::Backspace && !inputBuffer.empty()) {
            inputBuffer.pop_back();
        }
        return;
    }

    if (showDifficultyMenu || showLeaderboard || selectedRow < 0 || selectedCol < 0) return;

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

void SudokuUI::handleTextInput(std::uint32_t unicode) {
    if (showUsernameInput) {
        if (unicode >= 32 && unicode < 128 && inputBuffer.length() < 20) {
            inputBuffer += static_cast<char>(unicode);
        }
    }
}

void SudokuUI::onBoardChanged() {
}

void SudokuUI::onGameComplete() {
    gameWon = true;
    saveScoreToFile();
}

void SudokuUI::onAttemptsChanged(int remaining) {
}