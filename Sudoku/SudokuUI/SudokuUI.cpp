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
    sf::RectangleShape boardBg(sf::Vector2f(CELL_SIZE * 9 + 8, CELL_SIZE * 9 + 8));
    boardBg.setPosition(sf::Vector2f(BOARD_OFFSET_X - 4, BOARD_OFFSET_Y - 4));
    boardBg.setFillColor(sf::Color(255, 255, 255));
    window.draw(boardBg);

    sf::RectangleShape shadow(sf::Vector2f(CELL_SIZE * 9 + 12, CELL_SIZE * 9 + 12));
    shadow.setPosition(sf::Vector2f(BOARD_OFFSET_X - 6, BOARD_OFFSET_Y - 4));
    shadow.setFillColor(sf::Color(0, 0, 0, 20));
    window.draw(shadow);

    boardBg.setPosition(sf::Vector2f(BOARD_OFFSET_X - 4, BOARD_OFFSET_Y - 4));
    window.draw(boardBg);

    for (int i = 0; i <= 9; i++) {
        float thickness = (i % 3 == 0) ? THICK_LINE : GRID_THICKNESS;
        sf::Color lineColor = (i % 3 == 0) ? sf::Color(30, 30, 30) : sf::Color(200, 200, 200);

        sf::RectangleShape hLine(sf::Vector2f(CELL_SIZE * 9, thickness));
        hLine.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE));
        hLine.setFillColor(lineColor);
        window.draw(hLine);

        sf::RectangleShape vLine(sf::Vector2f(thickness, CELL_SIZE * 9));
        vLine.setPosition(sf::Vector2f(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y));
        vLine.setFillColor(lineColor);
        window.draw(vLine);
    }
}

void SudokuUI::drawSelection() {
    if (selectedRow >= 0 && selectedRow < 9 && selectedCol >= 0 && selectedCol < 9) {
        
        sf::RectangleShape selectionBg(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        selectionBg.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + selectedCol * CELL_SIZE + 1,
            BOARD_OFFSET_Y + selectedRow * CELL_SIZE + 1
        ));
        selectionBg.setFillColor(sf::Color(99, 102, 241, 120)); 
        window.draw(selectionBg);

        
        sf::RectangleShape selection(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        selection.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + selectedCol * CELL_SIZE,
            BOARD_OFFSET_Y + selectedRow * CELL_SIZE
        ));
        selection.setFillColor(sf::Color::Transparent);
        selection.setOutlineThickness(-5);  
        selection.setOutlineColor(sf::Color(99, 102, 241, 255));  
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

                       
                        sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                        highlight.setPosition(sf::Vector2f(
                            BOARD_OFFSET_X + col * CELL_SIZE + 1,
                            BOARD_OFFSET_Y + row * CELL_SIZE + 1
                        ));
                        highlight.setFillColor(sf::Color(251, 146, 60, 120));  
                        window.draw(highlight);

                        
                        sf::RectangleShape highlightOutline(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                        highlightOutline.setPosition(sf::Vector2f(
                            BOARD_OFFSET_X + col * CELL_SIZE,
                            BOARD_OFFSET_Y + row * CELL_SIZE
                        ));
                        highlightOutline.setFillColor(sf::Color::Transparent);
                        highlightOutline.setOutlineThickness(-3);
                        highlightOutline.setOutlineColor(sf::Color(251, 146, 60, 200));
                        window.draw(highlightOutline);
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
    sf::RectangleShape headerBg(sf::Vector2f(700, 80));
    headerBg.setPosition(sf::Vector2f(0, 0));
    headerBg.setFillColor(sf::Color(30, 30, 40));
    window.draw(headerBg);

    sf::Text title(font);
    title.setString("SUDOKU");
    title.setCharacterSize(42);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(BOARD_OFFSET_X, 18));
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
    diffBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + 230, 25));
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
        30
    ));
    window.draw(diffText);

    sf::RectangleShape attemptsBadge(sf::Vector2f(120, 30));
    attemptsBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + 330, 25));
    attemptsBadge.setFillColor(sf::Color(50, 50, 60));
    window.draw(attemptsBadge);

    sf::Text attemptsText(font);
    attemptsText.setString("Lives: " + std::to_string(game->getRemainingAttempts()));
    attemptsText.setCharacterSize(16);
    attemptsText.setFillColor(sf::Color::White);
    attemptsText.setStyle(sf::Text::Bold);
    attemptsText.setPosition(sf::Vector2f(BOARD_OFFSET_X + 345, 30));
    window.draw(attemptsText);

    sf::RectangleShape timerBadge(sf::Vector2f(100, 30));
    timerBadge.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 100, 25));
    timerBadge.setFillColor(PRIMARY_COLOR);
    window.draw(timerBadge);

    sf::Text timer(font);
    timer.setString(formatTime(game->getElapsedTime()));
    timer.setCharacterSize(18);
    timer.setFillColor(sf::Color::White);
    timer.setStyle(sf::Text::Bold);
    sf::FloatRect timerBounds = timer.getLocalBounds();
    timer.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 50 - timerBounds.size.x / 2,
        28
    ));
    window.draw(timer);

    sf::Text controls(font);
    controls.setString("1-9: Fill | 0/Del: Clear | Arrows: Move");
    controls.setCharacterSize(13);
    controls.setFillColor(sf::Color(120, 120, 120));
    controls.setPosition(sf::Vector2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + CELL_SIZE * 9 + 18));
    window.draw(controls);

    
    sf::RectangleShape resetBtn(sf::Vector2f(110, 38));
    resetBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 230, BOARD_OFFSET_Y + CELL_SIZE * 9 + 12));
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
        BOARD_OFFSET_Y + CELL_SIZE * 9 + 12 + (38 - resetBounds.size.y) / 2 - 3
    ));
    window.draw(resetText);

    sf::RectangleShape leaderBtn(sf::Vector2f(110, 38));
    leaderBtn.setPosition(sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, BOARD_OFFSET_Y + CELL_SIZE * 9 + 12));
    leaderBtn.setFillColor(WARNING_COLOR);
    window.draw(leaderBtn);

    sf::Text leaderText(font);
    leaderText.setString("Leaderboard");
    leaderText.setCharacterSize(14);
    leaderText.setFillColor(sf::Color::White);
    leaderText.setStyle(sf::Text::Bold);
    sf::FloatRect leaderBounds = leaderText.getLocalBounds();
    leaderText.setPosition(sf::Vector2f(
        BOARD_OFFSET_X + CELL_SIZE * 9 - 55 - leaderBounds.size.x / 2,
        BOARD_OFFSET_Y + CELL_SIZE * 9 + 12 + (38 - leaderBounds.size.y) / 2 - 3
    ));
    window.draw(leaderText);
}

void SudokuUI::drawDifficultyMenu() {
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(240, 242, 245));
    window.draw(overlay);

    sf::Text title(font);
    title.setString("Choose Difficulty");
    title.setCharacterSize(52);
    title.setFillColor(sf::Color(30, 30, 40));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 180));
    window.draw(title);

    sf::Text subtitle(font);
    subtitle.setString("Select your challenge level");
    subtitle.setCharacterSize(18);
    subtitle.setFillColor(sf::Color(100, 100, 120));
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition(sf::Vector2f(350 - subBounds.size.x / 2, 245));
    window.draw(subtitle);

    float btnWidth = 250;
    float btnHeight = 70;
    float startY = 310;
    float spacing = 25;

    struct Button {
        std::string text;
        std::string desc;
        sf::Color color;
        float y;
    };

    Button buttons[] = {
        {"Easy", "Perfect for beginners", SUCCESS_COLOR, startY},
        {"Medium", "A good challenge", WARNING_COLOR, startY + btnHeight + spacing},
        {"Hard", "For experts only", DANGER_COLOR, startY + 2 * (btnHeight + spacing)}
    };

    for (const auto& btn : buttons) {
        sf::RectangleShape shadow(sf::Vector2f(btnWidth + 4, btnHeight + 4));
        shadow.setPosition(sf::Vector2f(350 - btnWidth / 2 + 2, btn.y + 2));
        shadow.setFillColor(sf::Color(0, 0, 0, 30));
        window.draw(shadow);

        sf::RectangleShape rect(sf::Vector2f(btnWidth, btnHeight));
        rect.setPosition(sf::Vector2f(350 - btnWidth / 2, btn.y));
        rect.setFillColor(btn.color);
        window.draw(rect);

        sf::Text text(font);
        text.setString(btn.text);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(sf::Vector2f(
            350 - textBounds.size.x / 2,
            btn.y + 12
        ));
        window.draw(text);

        sf::Text desc(font);
        desc.setString(btn.desc);
        desc.setCharacterSize(14);
        desc.setFillColor(sf::Color(255, 255, 255, 200));
        sf::FloatRect descBounds = desc.getLocalBounds();
        desc.setPosition(sf::Vector2f(
            350 - descBounds.size.x / 2,
            btn.y + 45
        ));
        window.draw(desc);
    }
}

void SudokuUI::drawUsernameInput() {
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(240, 242, 245));
    window.draw(overlay);

    sf::Text icon(font);
    icon.setString("@");
    icon.setCharacterSize(80);
    icon.setFillColor(PRIMARY_COLOR);
    icon.setStyle(sf::Text::Bold);
    sf::FloatRect iconBounds = icon.getLocalBounds();
    icon.setPosition(sf::Vector2f(350 - iconBounds.size.x / 2, 180));
    window.draw(icon);

    sf::Text title(font);
    title.setString("Welcome to Sudoku!");
    title.setCharacterSize(44);
    title.setFillColor(sf::Color(30, 30, 40));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 280));
    window.draw(title);

    sf::Text subtitle(font);
    subtitle.setString("Enter your username to begin");
    subtitle.setCharacterSize(16);
    subtitle.setFillColor(sf::Color(100, 100, 120));
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition(sf::Vector2f(350 - subBounds.size.x / 2, 335));
    window.draw(subtitle);

    sf::RectangleShape inputShadow(sf::Vector2f(404, 64));
    inputShadow.setPosition(sf::Vector2f(148, 382));
    inputShadow.setFillColor(sf::Color(0, 0, 0, 20));
    window.draw(inputShadow);

    sf::RectangleShape inputBox(sf::Vector2f(400, 60));
    inputBox.setPosition(sf::Vector2f(150, 380));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(PRIMARY_COLOR);
    inputBox.setOutlineThickness(2);
    window.draw(inputBox);

    sf::Text inputText(font);
    inputText.setString(inputBuffer.empty() ? "" : inputBuffer);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color(30, 30, 40));
    inputText.setPosition(sf::Vector2f(170, 393));
    window.draw(inputText);

    if (inputBuffer.empty()) {
        sf::Text placeholder(font);
        placeholder.setString("Type your username...");
        placeholder.setCharacterSize(24);
        placeholder.setFillColor(sf::Color(150, 150, 160));
        placeholder.setPosition(sf::Vector2f(170, 393));
        window.draw(placeholder);
    }

    sf::Text cursor(font);
    cursor.setString("|");
    cursor.setCharacterSize(28);
    cursor.setFillColor(PRIMARY_COLOR);
    sf::FloatRect inputBounds = inputText.getLocalBounds();
    cursor.setPosition(sf::Vector2f(175 + inputBounds.size.x, 390));
    window.draw(cursor);

    sf::RectangleShape startShadow(sf::Vector2f(184, 54));
    startShadow.setPosition(sf::Vector2f(260, 482));
    startShadow.setFillColor(sf::Color(0, 0, 0, 30));
    window.draw(startShadow);

    sf::RectangleShape startBtn(sf::Vector2f(180, 50));
    startBtn.setPosition(sf::Vector2f(260, 480));
    startBtn.setFillColor(inputBuffer.empty() ? sf::Color(150, 150, 160) : PRIMARY_COLOR);
    window.draw(startBtn);

    sf::Text startText(font);
    startText.setString("Start Playing");
    startText.setCharacterSize(20);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setPosition(sf::Vector2f(
        350 - startBounds.size.x / 2,
        480 + (50 - startBounds.size.y) / 2 - 3
    ));
    window.draw(startText);
}

void SudokuUI::drawLeaderboard() {
    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape leaderWindow(sf::Vector2f(600, 650));
    leaderWindow.setPosition(sf::Vector2f(50, 75));
    leaderWindow.setFillColor(sf::Color(245, 245, 245));
    leaderWindow.setOutlineColor(GRID_COLOR);
    leaderWindow.setOutlineThickness(4);
    window.draw(leaderWindow);

    sf::Text title(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(48);
    title.setFillColor(GRID_COLOR);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 100));
    window.draw(title);

    
    sf::Text header(font);
    header.setString("Rank   Username        Difficulty   Time");
    header.setCharacterSize(20);
    header.setFillColor(GRID_COLOR);
    header.setStyle(sf::Text::Bold);
    header.setPosition(sf::Vector2f(100, 180));
    window.draw(header);

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

    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) {
            return std::get<2>(a) < std::get<2>(b);
        });

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

    if (scores.empty()) {
        sf::Text noData(font);
        noData.setString("No scores yet! Play a game to start.");
        noData.setCharacterSize(20);
        noData.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect noBounds = noData.getLocalBounds();
        noData.setPosition(sf::Vector2f(350 - noBounds.size.x / 2, 350));
        window.draw(noData);
    }

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
        sf::RectangleShape overlay(sf::Vector2f(500, 250));
        overlay.setPosition(sf::Vector2f(100, 275));
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        sf::Text winText(font);
        winText.setString("YOU WON!");
        winText.setCharacterSize(56);
        winText.setFillColor(sf::Color(100, 255, 100));
        winText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = winText.getLocalBounds();
        winText.setPosition(sf::Vector2f(350 - bounds.size.x / 2, 310));
        window.draw(winText);

        sf::Text timeText(font);
        timeText.setString("Time: " + formatTime(game->getElapsedTime()));
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        sf::FloatRect timeBounds = timeText.getLocalBounds();
        timeText.setPosition(sf::Vector2f(350 - timeBounds.size.x / 2, 390));
        window.draw(timeText);

        sf::RectangleShape continueBtn(sf::Vector2f(150, 45));
        continueBtn.setPosition(sf::Vector2f(275, 450));
        continueBtn.setFillColor(sf::Color(100, 200, 100));
        window.draw(continueBtn);

        sf::Text continueText(font);
        continueText.setString("Continue");
        continueText.setCharacterSize(20);
        continueText.setFillColor(sf::Color::White);
        continueText.setStyle(sf::Text::Bold);
        sf::FloatRect continueBounds = continueText.getLocalBounds();
        continueText.setPosition(sf::Vector2f(
            275 + (150 - continueBounds.size.x) / 2,
            450 + (45 - continueBounds.size.y) / 2 - 3
        ));
        window.draw(continueText);
    }
    else if (gameLost) {
        sf::RectangleShape overlay(sf::Vector2f(500, 250));
        overlay.setPosition(sf::Vector2f(100, 275));
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        sf::Text loseText(font);
        loseText.setString("GAME OVER!");
        loseText.setCharacterSize(56);
        loseText.setFillColor(sf::Color(255, 100, 100));
        loseText.setStyle(sf::Text::Bold);
        sf::FloatRect bounds = loseText.getLocalBounds();
        loseText.setPosition(sf::Vector2f(350 - bounds.size.x / 2, 310));
        window.draw(loseText);

        sf::Text infoText(font);
        infoText.setString("No attempts remaining!");
        infoText.setCharacterSize(24);
        infoText.setFillColor(sf::Color::White);
        sf::FloatRect infoBounds = infoText.getLocalBounds();
        infoText.setPosition(sf::Vector2f(350 - infoBounds.size.x / 2, 390));
        window.draw(infoText);

        sf::RectangleShape tryAgainBtn(sf::Vector2f(150, 45));
        tryAgainBtn.setPosition(sf::Vector2f(275, 450));
        tryAgainBtn.setFillColor(sf::Color(200, 100, 100));
        window.draw(tryAgainBtn);

        sf::Text tryAgainText(font);
        tryAgainText.setString("Try Again");
        tryAgainText.setCharacterSize(20);
        tryAgainText.setFillColor(sf::Color::White);
        tryAgainText.setStyle(sf::Text::Bold);
        sf::FloatRect tryBounds = tryAgainText.getLocalBounds();
        tryAgainText.setPosition(sf::Vector2f(
            275 + (150 - tryBounds.size.x) / 2,
            450 + (45 - tryBounds.size.y) / 2 - 3
        ));
        window.draw(tryAgainText);
    }
}

void SudokuUI::render() {
    window.clear(BACKGROUND_COLOR);

    if (showLeaderboard) {
        drawGrid();
        drawHighlights();
        drawSelection();
        drawNumbers();
        drawInfo();
        drawLeaderboard();
        return;
    }

    if (showUsernameInput) {
        drawUsernameInput();
        return;
    }

    if (showDifficultyMenu) {
        drawDifficultyMenu();
        return;
    }

    drawGrid();
    drawHighlights();
    drawSelection();
    drawNumbers();
    drawInfo();
    drawGameOverlay();
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

    if (gameLost) {
        sf::FloatRect tryAgainRect(sf::Vector2f(275, 450), sf::Vector2f(150, 45));
        if (tryAgainRect.contains(mousePos)) {
            showUsernameInput = true;
            showDifficultyMenu = false;
            gameLost = false;
            gameWon = false;
            selectedRow = -1;
            selectedCol = -1;
            inputBuffer = "";
        }
        return;
    }

    if (gameWon) {
        sf::FloatRect continueRect(sf::Vector2f(275, 450), sf::Vector2f(150, 45));
        if (continueRect.contains(mousePos)) {
            showUsernameInput = true;
            showDifficultyMenu = false;
            gameWon = false;
            gameLost = false;
            selectedRow = -1;
            selectedCol = -1;
            inputBuffer = "";
        }
        return;
    }

    if (showLeaderboard) {
        sf::FloatRect closeRect(sf::Vector2f(290, 650), sf::Vector2f(120, 45));
        if (closeRect.contains(mousePos)) {
            showLeaderboard = false;
        }
        return;
    }

    if (showUsernameInput) {
        sf::FloatRect startRect(sf::Vector2f(260, 480), sf::Vector2f(180, 50));
        if (startRect.contains(mousePos) && !inputBuffer.empty()) {
            username = inputBuffer;
            showUsernameInput = false;
            showDifficultyMenu = true;
        }
        return;
    }

    if (showDifficultyMenu) {
        float btnWidth = 250;
        float btnHeight = 70;
        float startY = 310;
        float spacing = 25;
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

    int row, col;
    if (isMouseOverCell(mouseX, mouseY, row, col)) {
        selectedRow = row;
        selectedCol = col;
        return;
    }

    sf::FloatRect resetRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 230, BOARD_OFFSET_Y + CELL_SIZE * 9 + 12),
        sf::Vector2f(110, 38)
    );

    if (resetRect.contains(mousePos)) {
        showUsernameInput = true;
        showDifficultyMenu = false;
        gameWon = false;
        gameLost = false;
        selectedRow = -1;
        selectedCol = -1;
        inputBuffer = "";
        return;
    }

    sf::FloatRect leaderRect(
        sf::Vector2f(BOARD_OFFSET_X + CELL_SIZE * 9 - 110, BOARD_OFFSET_Y + CELL_SIZE * 9 + 12),
        sf::Vector2f(110, 38)
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

    if (showDifficultyMenu || showLeaderboard || gameWon || gameLost || selectedRow < 0 || selectedCol < 0) return;

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
    if (remaining <= 0) {
        gameLost = true;
    }
}