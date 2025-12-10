#include "GameScreen.h"
#include "Difficulty.h"
#include "CellState.h"
#include "ScreenManager.h"
#include "ScoreManager.h"
#include <iostream>
#include <cstdint>

GameScreen::GameScreen(ScreenManager& mgr, sf::RenderWindow& win)
    : manager(mgr), game(mgr.getGame()), window(win), font(mgr.getFont()),
    selectedRow(-1), selectedCol(-1),
    gameWon(false), gameLost(false)
{
    if (game) game->attachObserver(this);
}

void GameScreen::render(sf::RenderWindow&) {
    window.clear(BACKGROUND_COLOR);

    drawGrid();
    drawHighlights();
    drawNumbers();
    drawSelection();
    drawInfo();
    drawGameOverlay();
    drawHintOverlay();

    window.display();
}

void GameScreen::drawGrid() {
    sf::RectangleShape boardBg(sf::Vector2f(CELL_SIZE * 9 + 8.f, CELL_SIZE * 9 + 8.f));
    boardBg.setPosition(sf::Vector2f(BOARD_OFFSET_X - 4.f, BOARD_OFFSET_Y - 4.f));
    boardBg.setFillColor(sf::Color::White);
    window.draw(boardBg);

    sf::RectangleShape shadow(sf::Vector2f(CELL_SIZE * 9 + 12.f, CELL_SIZE * 9 + 12.f));
    shadow.setPosition(sf::Vector2f(BOARD_OFFSET_X - 6.f, BOARD_OFFSET_Y - 4.f));
    shadow.setFillColor(sf::Color(0, 0, 0, 20));
    window.draw(shadow);

    for (int i = 0; i <= 9; ++i) {
        float thick = (i % 3 == 0) ? THICK_LINE : GRID_THICKNESS;
        sf::Color color = (i % 3 == 0) ? sf::Color(30, 30, 30) : sf::Color(200, 200, 200);

        sf::RectangleShape hLine(sf::Vector2f(CELL_SIZE * 9.f, thick));
        hLine.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE));
        hLine.setFillColor(color);
        window.draw(hLine);

        sf::RectangleShape vLine(sf::Vector2f(thick, CELL_SIZE * 9.f));
        vLine.setPosition(sf::Vector2f(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y));
        vLine.setFillColor(color);
        window.draw(vLine);
    }
}

void GameScreen::drawSelection() {
    if (selectedRow < 0 || selectedCol < 0) return;

    sf::RectangleShape selBg(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    selBg.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + selectedCol * CELL_SIZE + 1.f,
        BOARD_OFFSET_Y + selectedRow * CELL_SIZE + 1.f
    ));
    selBg.setFillColor(sf::Color(99, 102, 241, 120));
    window.draw(selBg);

    sf::RectangleShape border(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    border.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + selectedCol * CELL_SIZE,
        BOARD_OFFSET_Y + selectedRow * CELL_SIZE
    ));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(-5.f);
    border.setOutlineColor(sf::Color(99, 102, 241));
    window.draw(border);
}

void GameScreen::drawHighlights() {
    if (!game) return;
    if (selectedRow < 0 || selectedCol < 0) return;

    int selVal = game->getValue(selectedRow, selectedCol);
    if (selVal == 0) return;

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (game->getValue(r, c) == selVal && !(r == selectedRow && c == selectedCol)) {
                sf::RectangleShape h(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
                h.setPosition(sf::Vector2f(
                    BOARD_OFFSET_X + c * CELL_SIZE + 1.f,
                    BOARD_OFFSET_Y + r * CELL_SIZE + 1.f
                ));
                h.setFillColor(sf::Color(251, 146, 60, 120));
                window.draw(h);

                sf::RectangleShape outline(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                outline.setPosition(sf::Vector2f(
                    BOARD_OFFSET_X + c * CELL_SIZE,
                    BOARD_OFFSET_Y + r * CELL_SIZE
                ));
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineThickness(-3.f);
                outline.setOutlineColor(sf::Color(251, 146, 60));
                window.draw(outline);
            }
        }
    }
}

void GameScreen::drawNumbers() {
    if (!game) return;
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

void GameScreen::drawInfo() {
    if (!game) return;

    sf::RectangleShape headerBg(sf::Vector2f(750, 70));
    headerBg.setPosition(sf::Vector2f(0, 0));
    headerBg.setFillColor(sf::Color(30, 30, 40));
    window.draw(headerBg);

    sf::Text title(font);
    title.setString("SUDOKU");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(BOARD_OFFSET_X, 10));
    window.draw(title);

    std::string diffStr;
    sf::Color diffColor;
    Difficulty diff = game->getCurrentDifficulty();
    switch (diff) {
    case Difficulty::EASY:
        diffStr = "Easy";
        diffColor = SUCCESS_COLOR;
        break;
    case Difficulty::MEDIUM:
        diffStr = "Medium";
        diffColor = WARNING_COLOR;
        break;
    case Difficulty::HARD:
        diffStr = "Hard";
        diffColor = DANGER_COLOR;
        break;
    }

    sf::RectangleShape diffBadge(sf::Vector2f(80, 30));
    diffBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + 230, 28));
    diffBadge.setFillColor(diffColor);
    window.draw(diffBadge);

    sf::Text diffText(font);
    diffText.setString(diffStr);
    diffText.setCharacterSize(16);
    diffText.setFillColor(sf::Color::White);
    diffText.setStyle(sf::Text::Bold);
    sf::FloatRect diffBounds = diffText.getLocalBounds();
    diffText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + 270 - diffBounds.size.x / 2,
        33
    ));
    window.draw(diffText);

    sf::RectangleShape attemptsBadge(sf::Vector2f(120, 30));
    attemptsBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + 330, 28));
    attemptsBadge.setFillColor(sf::Color(50, 50, 60));
    window.draw(attemptsBadge);

    sf::Text attemptsText(font);
    attemptsText.setString("Lives: " + std::to_string(game->getRemainingAttempts()));
    attemptsText.setCharacterSize(16);
    attemptsText.setFillColor(sf::Color::White);
    attemptsText.setStyle(sf::Text::Bold);
    attemptsText.setPosition(sf::Vector2f(BOARD_OFFSET_X + 345, 33));
    window.draw(attemptsText);

    sf::RectangleShape timerBadge(sf::Vector2f(100, 30));
    timerBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 100, 28));
    timerBadge.setFillColor(PRIMARY_COLOR);
    window.draw(timerBadge);

    sf::Text timer(font);
    timer.setString(ScoreManager::formatTime(game->getElapsedTime()));
    timer.setCharacterSize(18);
    timer.setFillColor(sf::Color::White);
    timer.setStyle(sf::Text::Bold);
    sf::FloatRect timerBounds = timer.getLocalBounds();
    timer.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 50 - timerBounds.size.x / 2,
        30
    ));
    window.draw(timer);

    sf::Text controls(font);
    controls.setString("1-9: Fill | 0/Del: Clear | Arrows: Move | H: Hint");
    controls.setCharacterSize(13);
    controls.setFillColor(sf::Color(120, 120, 120));
    controls.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + CELL_SIZE * 9 + 20));
    window.draw(controls);

    float buttonY = BOARD_OFFSET_Y + CELL_SIZE * 9 + 50;

    sf::RectangleShape mainMenuBtn(sf::Vector2f(110, 38));
    mainMenuBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 350, buttonY));
    mainMenuBtn.setFillColor(sf::Color(100, 100, 120));
    window.draw(mainMenuBtn);

    sf::Text mainMenuText(font);
    mainMenuText.setString("Main Menu");
    mainMenuText.setCharacterSize(15);
    mainMenuText.setFillColor(sf::Color::White);
    mainMenuText.setStyle(sf::Text::Bold);
    sf::FloatRect mainMenuBounds = mainMenuText.getLocalBounds();
    mainMenuText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 295 - mainMenuBounds.size.x / 2,
        buttonY + (38 - mainMenuBounds.size.y) / 2 - 3
    ));
    window.draw(mainMenuText);

    sf::RectangleShape resetBtn(sf::Vector2f(110, 38));
    resetBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 230, buttonY));
    resetBtn.setFillColor(PRIMARY_COLOR);
    window.draw(resetBtn);

    sf::Text resetText(font);
    resetText.setString("New Game");
    resetText.setCharacterSize(15);
    resetText.setFillColor(sf::Color::White);
    resetText.setStyle(sf::Text::Bold);
    sf::FloatRect resetBounds = resetText.getLocalBounds();
    resetText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 175 - resetBounds.size.x / 2,
        buttonY + (38 - resetBounds.size.y) / 2 - 3
    ));
    window.draw(resetText);

    sf::RectangleShape hintBtn(sf::Vector2f(110, 38));
    hintBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, buttonY));
    hintBtn.setFillColor(HINT_COLOR);
    window.draw(hintBtn);

    sf::Text hintText(font);
    hintText.setString("Hint");
    hintText.setCharacterSize(15);
    hintText.setFillColor(sf::Color::White);
    hintText.setStyle(sf::Text::Bold);
    sf::FloatRect hintBounds = hintText.getLocalBounds();
    hintText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 55 - hintBounds.size.x / 2,
        buttonY + (38 - hintBounds.size.y) / 2 - 3
    ));
    window.draw(hintText);
}

void GameScreen::drawGameOverlay() {
    if (!game) return;

    if (gameWon) {
        sf::RectangleShape overlay(sf::Vector2f(500, 180));
        overlay.setPosition(sf::Vector2f(100, 340));
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        sf::Text winText(font);
        winText.setString("YOU WON!");
        winText.setCharacterSize(56);
        winText.setFillColor(sf::Color(100, 255, 100));
        winText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = winText.getLocalBounds();
        winText.setPosition(sf::Vector2f(350 - bounds.size.x / 2, 360));
        window.draw(winText);

        sf::Text timeText(font);
        timeText.setString("Time: " + ScoreManager::formatTime(game->getElapsedTime()));
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        sf::FloatRect timeBounds = timeText.getLocalBounds();
        timeText.setPosition(sf::Vector2f(350 - timeBounds.size.x / 2, 435));
        window.draw(timeText);

        sf::Text clickText(font);
        clickText.setString("Click anywhere to continue");
        clickText.setCharacterSize(16);
        clickText.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect clickBounds = clickText.getLocalBounds();
        clickText.setPosition(sf::Vector2f(350 - clickBounds.size.x / 2, 480));
        window.draw(clickText);
    }
    else if (gameLost) {
        sf::RectangleShape overlay(sf::Vector2f(500, 180));
        overlay.setPosition(sf::Vector2f(100, 340));
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        sf::Text loseText(font);
        loseText.setString("GAME OVER!");
        loseText.setCharacterSize(56);
        loseText.setFillColor(sf::Color(255, 100, 100));
        loseText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = loseText.getLocalBounds();
        loseText.setPosition(sf::Vector2f(350 - bounds.size.x / 2, 360));
        window.draw(loseText);

        sf::Text infoText(font);
        infoText.setString("No attempts remaining!");
        infoText.setCharacterSize(24);
        infoText.setFillColor(sf::Color::White);
        sf::FloatRect infoBounds = infoText.getLocalBounds();
        infoText.setPosition(sf::Vector2f(350 - infoBounds.size.x / 2, 435));
        window.draw(infoText);

        sf::Text clickText(font);
        clickText.setString("Click to return to main menu");
        clickText.setCharacterSize(16);
        clickText.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect clickBounds = clickText.getLocalBounds();
        clickText.setPosition(sf::Vector2f(350 - clickBounds.size.x / 2, 480));
        window.draw(clickText);
    }
}

void GameScreen::drawHintOverlay() {
    if (!currentHint.has_value()) return;
    const Hint& h = *currentHint;

    // Highlight hinted cell
    if (h.row >= 0 && h.col >= 0) {
        sf::RectangleShape sel(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
        sel.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + h.col * CELL_SIZE + 1.f,
            BOARD_OFFSET_Y + h.row * CELL_SIZE + 1.f
        ));
        sel.setFillColor(sf::Color(HINT_COLOR.r, HINT_COLOR.g, HINT_COLOR.b, 120));
        window.draw(sel);

        sf::RectangleShape outline(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        outline.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + h.col * CELL_SIZE,
            BOARD_OFFSET_Y + h.row * CELL_SIZE
        ));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(-3.f);
        outline.setOutlineColor(HINT_COLOR);
        window.draw(outline);
    }

    // Message box
    sf::RectangleShape box(sf::Vector2f(500.f, 70.f));
    box.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y - 70.f));
    box.setFillColor(sf::Color(20, 20, 30, 230));
    window.draw(box);

    sf::Text msg(font);
    msg.setString(h.message);
    msg.setCharacterSize(16);
    msg.setFillColor(sf::Color::White);
    msg.setPosition(sf::Vector2f(BOARD_OFFSET_X + 10.f, BOARD_OFFSET_Y - 60.f));
    window.draw(msg);

    if (!h.candidates.empty()) {
        std::string candStr = "Candidates: ";
        for (size_t i = 0; i < h.candidates.size(); ++i) {
            candStr += std::to_string(h.candidates[i]);
            if (i + 1 < h.candidates.size()) candStr += ", ";
        }
        sf::Text cand(font);
        cand.setString(candStr);
        cand.setCharacterSize(14);
        cand.setFillColor(sf::Color(180, 180, 200));
        cand.setPosition(sf::Vector2f(BOARD_OFFSET_X + 10.f, BOARD_OFFSET_Y - 38.f));
        window.draw(cand);
    }

    if (h.value > 0) {
        sf::RectangleShape applyBtn(sf::Vector2f(90.f, 30.f));
        applyBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + 500.f - 100.f, BOARD_OFFSET_Y - 60.f));
        applyBtn.setFillColor(HINT_COLOR);
        window.draw(applyBtn);

        sf::Text applyText(font);
        applyText.setString("Apply");
        applyText.setCharacterSize(14);
        applyText.setFillColor(sf::Color::White);
        sf::FloatRect t = applyText.getLocalBounds();
        applyText.setPosition(sf::Vector2f(BOARD_OFFSET_X + 500.f - 100.f + (90.f - t.size.x)/2.f, BOARD_OFFSET_Y - 60.f + (30.f - t.size.y)/2.f - 3.f));
        window.draw(applyText);
    }
}

bool GameScreen::isMouseOverCell(int mx, int my, int& r, int& c) {
    if (mx < BOARD_OFFSET_X || my < BOARD_OFFSET_Y) return false;
    mx -= BOARD_OFFSET_X;
    my -= BOARD_OFFSET_Y;
    c = mx / CELL_SIZE;
    r = my / CELL_SIZE;
    return (r >= 0 && r < 9 && c >= 0 && c < 9);
}

void GameScreen::handleMouseClick(int x, int y) {
    if (gameWon || gameLost) {
        manager.setScreen(ScreenType::Username, window);
        return;
    }

    sf::Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));
    float buttonY = BOARD_OFFSET_Y + CELL_SIZE * 9 + 50;

    sf::FloatRect mainMenuRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 350, buttonY),
        sf::Vector2f(110, 38)
    );

    if (mainMenuRect.contains(mousePos)) {
        manager.setScreen(ScreenType::Username, window);
        return;
    }

    sf::FloatRect resetRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 230, buttonY),
        sf::Vector2f(110, 38)
    );

    if (resetRect.contains(mousePos)) {
        manager.setScreen(ScreenType::Difficulty, window);
        return;
    }

    sf::FloatRect hintRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, buttonY),
        sf::Vector2f(110, 38)
    );

    if (hintRect.contains(mousePos)) {
        currentHint.reset();
        if (manager.getHintManager() && game) {
            currentHint = manager.getHintManager()->next(*game);
        }
        return;
    }

    // Apply button in overlay if present and direct value
    if (currentHint.has_value() && currentHint->value > 0) {
        sf::FloatRect applyRect(
            sf::Vector2f(BOARD_OFFSET_X + 500.f - 100.f, BOARD_OFFSET_Y - 60.f),
            sf::Vector2f(90.f, 30.f)
        );
        if (applyRect.contains(mousePos)) {
            const Hint& h = *currentHint;
            if (game && game->getCellState(h.row, h.col) == CellState::EMPTY) {
                game->setValue(h.row, h.col, h.value);
            }
            currentHint.reset();
            return;
        }
    }

    int r, c;
    if (isMouseOverCell(x, y, r, c)) {
        selectedRow = r;
        selectedCol = c;
    }
}

void GameScreen::handleKeyPress(sf::Keyboard::Key key) {
    if (!game) return;
    if (gameWon || gameLost) return;

    if (key == sf::Keyboard::Key::H) {
        currentHint.reset();
        if (manager.getHintManager()) {
            currentHint = manager.getHintManager()->next(*game);
        }
        return;
    }

    if (selectedRow < 0 || selectedCol < 0) return;

    if (key == sf::Keyboard::Key::Backspace || key == sf::Keyboard::Key::Delete) {
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

void GameScreen::handleTextInput(std::uint32_t unicode) {
    if (!game) return;
    if (selectedRow < 0 || selectedCol < 0) return;
    if (gameWon || gameLost) return;

    if (unicode >= '1' && unicode <= '9') {
        int val = static_cast<int>(unicode - '0');
        game->setValue(selectedRow, selectedCol, val);
    }
    else if (unicode == '0') {
        game->setValue(selectedRow, selectedCol, 0);
    }
}

void GameScreen::onBoardChanged() {}
void GameScreen::onGameComplete() {
    gameWon = true;

    if (game) {
        std::string diffStr;
        Difficulty diff = game->getCurrentDifficulty();
        switch (diff) {
        case Difficulty::EASY: diffStr = "Easy"; break;
        case Difficulty::MEDIUM: diffStr = "Medium"; break;
        case Difficulty::HARD: diffStr = "Hard"; break;
        }

        std::string username = manager.getUsername();
        if (!username.empty()) {
            ScoreManager::updateOrAddScore(username, diffStr, game->getElapsedTime());
        }
    }
}

void GameScreen::onAttemptsChanged(int remaining) { if (remaining <= 0) gameLost = true; }