#include "UsernameScreen.h"
#include <SFML/Graphics.hpp>

UsernameScreen::UsernameScreen(ScreenManager& mgr, sf::RenderWindow& win)
    : manager(mgr), window(win), showUsernameInput(true), username(""), inputBuffer("")
{
}

void UsernameScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(240, 242, 245));
    drawUsernameInput();
    window.display();
}

void UsernameScreen::drawUsernameInput() {
    sf::Font& font = manager.getFont();
    const sf::Color PRIMARY_COLOR(99, 102, 241);

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

    sf::RectangleShape leaderBtn(sf::Vector2f(180, 50));
    leaderBtn.setPosition(sf::Vector2f(260, 540));
    leaderBtn.setFillColor(sf::Color(200, 140, 0));
    window.draw(leaderBtn);

    sf::Text leaderText(font);
    leaderText.setString("Leaderboard");
    leaderText.setCharacterSize(18);
    leaderText.setFillColor(sf::Color::White);
    leaderText.setStyle(sf::Text::Bold);
    sf::FloatRect lBounds = leaderText.getLocalBounds();
    leaderText.setPosition(sf::Vector2f(260 + (180 - lBounds.size.x) / 2, 540 + (50 - lBounds.size.y) / 2 - 3));
    window.draw(leaderText);
}

void UsernameScreen::handleMouseClick(int x, int y) {
    sf::Vector2f click(static_cast<float>(x), static_cast<float>(y));
    sf::FloatRect startRect(sf::Vector2f(260.f, 480.f), sf::Vector2f(180.f, 50.f));
    sf::FloatRect leaderRect(sf::Vector2f(260.f, 540.f), sf::Vector2f(180.f, 50.f));

    if (startRect.contains(click) && !inputBuffer.empty()) {
        username = inputBuffer;
        manager.setUsername(username);
        manager.setScreen(ScreenType::Difficulty, window);
        return;
    }

    if (leaderRect.contains(click)) {
        manager.setScreen(ScreenType::Leaderboard, window);
        return;
    }
}

void UsernameScreen::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Enter && !inputBuffer.empty()) {
        username = inputBuffer;
        manager.setUsername(username);
        manager.setScreen(ScreenType::Difficulty, window);
    }
    else if (key == sf::Keyboard::Key::Backspace && !inputBuffer.empty()) {
        inputBuffer.pop_back();
    }
}

void UsernameScreen::handleTextInput(uint32_t unicode) {
    if (unicode >= 32 && unicode < 128 && inputBuffer.length() < 20) {
        inputBuffer += static_cast<char>(unicode);
    }
}