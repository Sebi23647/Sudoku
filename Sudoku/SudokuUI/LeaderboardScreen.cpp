#include "LeaderboardScreen.h"
#include "ScoreManager.h"
#include <algorithm>
#include <sstream>
#include <SFML/Graphics.hpp>

LeaderboardScreen::LeaderboardScreen(ScreenManager& mgr, sf::RenderWindow& win)
    : manager(mgr), window(win), showLeaderboard(true), leaderboardTab(0) {
}

void LeaderboardScreen::drawLeaderboard() {
    sf::Font& font = manager.getFont();

    sf::RectangleShape overlay(sf::Vector2f(700, 800));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape leaderWindow(sf::Vector2f(600, 650));
    leaderWindow.setPosition(sf::Vector2f(50, 75));
    leaderWindow.setFillColor(sf::Color(245, 245, 245));
    leaderWindow.setOutlineColor(sf::Color(50, 50, 50));
    leaderWindow.setOutlineThickness(4);
    window.draw(leaderWindow);

    sf::Text title(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color(50, 50, 50));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 100));
    window.draw(title);

    float tabY = 165;
    float tabWidth = 120;
    float tabHeight = 40;
    float tabSpacing = 10;
    float tabStartX = 350 - (3 * tabWidth + 2 * tabSpacing) / 2;

    std::string tabNames[] = { "Easy", "Medium", "Hard" };
    sf::Color tabColors[] = { sf::Color(100, 200, 100), sf::Color(255, 200, 50), sf::Color(200, 50, 50) };

    for (int i = 0; i < 3; i++) {
        sf::RectangleShape tab(sf::Vector2f(tabWidth, tabHeight));
        tab.setPosition(sf::Vector2f(tabStartX + i * (tabWidth + tabSpacing), tabY));
        tab.setFillColor(i == leaderboardTab ? tabColors[i] : sf::Color(200, 200, 200));
        window.draw(tab);

        sf::Text tabText(font);
        tabText.setString(tabNames[i]);
        tabText.setCharacterSize(18);
        tabText.setFillColor(i == leaderboardTab ? sf::Color::White : sf::Color(80, 80, 80));
        tabText.setStyle(sf::Text::Bold);
        sf::FloatRect tabBounds = tabText.getLocalBounds();
        tabText.setPosition(sf::Vector2f(
            tabStartX + i * (tabWidth + tabSpacing) + (tabWidth - tabBounds.size.x) / 2,
            tabY + (tabHeight - tabBounds.size.y) / 2 - 3
        ));
        window.draw(tabText);
    }

    sf::Text header(font);
    header.setString("Rank\tUsername\t\tTime");
    header.setCharacterSize(20);
    header.setFillColor(sf::Color(50, 50, 50));
    header.setStyle(sf::Text::Bold);
    header.setPosition(sf::Vector2f(100, 225));
    window.draw(header);

    std::string filterDiff = (leaderboardTab == 0 ? "Easy" : (leaderboardTab == 1 ? "Medium" : "Hard"));
    auto scores = ScoreManager::getScoresForDifficulty(filterDiff);

    float yPos = 275;
    int rank = 1;
    for (size_t i = 0; i < std::min(scores.size(), size_t(10)); ++i) {
        std::ostringstream oss;
        oss << rank << ".\t" << scores[i].first << "\t\t" << ScoreManager::formatTime(scores[i].second);

        sf::Text entry(font);
        entry.setString(oss.str());
        entry.setCharacterSize(18);
        entry.setFillColor(sf::Color(50, 50, 50));
        entry.setPosition(sf::Vector2f(100, yPos));
        window.draw(entry);
        yPos += 35;
        rank++;
    }

    if (scores.empty()) {
        sf::Text noData(font);
        noData.setString("No scores for this difficulty yet!");
        noData.setCharacterSize(20);
        noData.setFillColor(sf::Color(150, 150, 150));
        sf::FloatRect noBounds = noData.getLocalBounds();
        noData.setPosition(sf::Vector2f(350 - noBounds.size.x / 2, 400));
        window.draw(noData);
    }

    sf::RectangleShape closeBtn(sf::Vector2f(120, 45));
    closeBtn.setPosition(sf::Vector2f(290, 660));
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
        660 + (45 - closeBounds.size.y) / 2 - 3
    ));
    window.draw(closeText);
}

void LeaderboardScreen::render(sf::RenderWindow& window) {
    if (!showLeaderboard) return;
    window.clear(sf::Color(240, 242, 245));
    drawLeaderboard();
    window.display();
}

void LeaderboardScreen::handleMouseClick(int x, int y) {
    sf::Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));

    float tabY = 165.f;
    float tabWidth = 120.f;
    float tabHeight = 40.f;
    float tabSpacing = 10.f;
    float tabStartX = 350.f - (3.f * tabWidth + 2.f * tabSpacing) / 2.f;

    for (int i = 0; i < 3; i++) {
        sf::FloatRect tabRect(
            sf::Vector2f(tabStartX + i * (tabWidth + tabSpacing), tabY),
            sf::Vector2f(tabWidth, tabHeight)
        );
        if (tabRect.contains(mousePos)) {
            leaderboardTab = i;
            return;
        }
    }

    sf::FloatRect closeRect(sf::Vector2f(290.f, 660.f), sf::Vector2f(120.f, 45.f));
    if (closeRect.contains(mousePos)) {
        showLeaderboard = false;
        manager.setScreen(ScreenType::Username, window);
    }
}

void LeaderboardScreen::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Escape) {
        showLeaderboard = false;
        manager.setScreen(ScreenType::Username, window);
    }
}

void LeaderboardScreen::handleTextInput(uint32_t unicode) {
}