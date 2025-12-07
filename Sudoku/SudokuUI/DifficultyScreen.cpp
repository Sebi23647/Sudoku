#include "DifficultyScreen.h"
#include "Difficulty.h"
#include <SFML/Graphics.hpp>
#include "../SudokuLib/ISudokuGame.h"
#include "ScreenManager.h"

DifficultyScreen::DifficultyScreen(ScreenManager& mgr, sf::RenderWindow& win)
    : manager(mgr), window(win), showDifficultyMenu(true) {
}

void DifficultyScreen::drawDifficultyMenu() {
    sf::Font& font = manager.getFont();

    sf::Text title(font);
    title.setString("Pick a difficulty");
    title.setCharacterSize(52);
    title.setFillColor(sf::Color(30, 30, 40));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(sf::Vector2f(350 - titleBounds.size.x / 2, 120));
    window.draw(title);

    sf::Text subtitle(font);
    subtitle.setString("Select your challenge level");
    subtitle.setCharacterSize(18);
    subtitle.setFillColor(sf::Color(100, 100, 120));
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition(sf::Vector2f(350 - subBounds.size.x / 2, 180));
    window.draw(subtitle);

    float btnWidth = 250.f;
    float btnHeight = 70.f;
    float startY = 260.f;
    float spacing = 30.f;
    float centerX = 350.f;

    struct Button {
        std::string text;
        std::string desc;
        sf::Color color;
        float y;
    };

    Button buttons[] = {
        {"Easy", "Perfect for beginners", sf::Color(100, 200, 120), startY},
        {"Medium", "A good challenge", sf::Color(255, 165, 0), startY + btnHeight + spacing},
        {"Hard", "For experts only", sf::Color(220, 80, 80), startY + 2 * (btnHeight + spacing)}
    };

    for (const auto& btn : buttons) {
        sf::RectangleShape shadow(sf::Vector2f(btnWidth + 6.f, btnHeight + 6.f));
        shadow.setPosition(sf::Vector2f(centerX - btnWidth / 2.f + 3.f, btn.y + 3.f));
        shadow.setFillColor(sf::Color(0, 0, 0, 30));
        window.draw(shadow);

        sf::RectangleShape rect(sf::Vector2f(btnWidth, btnHeight));
        rect.setPosition(sf::Vector2f(centerX - btnWidth / 2.f, btn.y));
        rect.setFillColor(btn.color);
        window.draw(rect);

        sf::Text text(font);
        text.setString(btn.text);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        text.setPosition(sf::Vector2f(centerX - 90.f, btn.y + 12.f));
        window.draw(text);

        sf::Text desc(font);
        desc.setString(btn.desc);
        desc.setCharacterSize(14);
        desc.setFillColor(sf::Color(255, 255, 255, 200));
        desc.setPosition(sf::Vector2f(centerX - 90.f, btn.y + 40.f));
        window.draw(desc);
    }
}

void DifficultyScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 242, 245));
    if (showDifficultyMenu) {
        drawDifficultyMenu();
    }
    window.display();
}

void DifficultyScreen::handleMouseClick(int x, int y) {
    float btnWidth = 250.f;
    float btnHeight = 70.f;
    float startY = 260.f;
    float spacing = 30.f;
    float centerX = 350.f;

    sf::FloatRect easyRect(sf::Vector2f(centerX - btnWidth / 2.f, startY), sf::Vector2f(btnWidth, btnHeight));
    sf::FloatRect mediumRect(sf::Vector2f(centerX - btnWidth / 2.f, startY + btnHeight + spacing), sf::Vector2f(btnWidth, btnHeight));
    sf::FloatRect hardRect(sf::Vector2f(centerX - btnWidth / 2.f, startY + 2 * (btnHeight + spacing)), sf::Vector2f(btnWidth, btnHeight));

    sf::Vector2f clickPos(static_cast<float>(x), static_cast<float>(y));

    if (easyRect.contains(clickPos)) {
        manager.createGame(Difficulty::EASY, window);
    }
    else if (mediumRect.contains(clickPos)) {
        manager.createGame(Difficulty::MEDIUM, window);
    }
    else if (hardRect.contains(clickPos)) {
        manager.createGame(Difficulty::HARD, window);
    }
}

void DifficultyScreen::handleKeyPress(sf::Keyboard::Key key) {}
void DifficultyScreen::handleTextInput(uint32_t unicode) {}