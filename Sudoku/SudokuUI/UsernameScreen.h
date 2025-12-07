#pragma once
#include "IScreen.h"
#include "ScreenManager.h"
#include <SFML/Graphics.hpp>
#include <string>

class UsernameScreen : public IScreen {
private:
    ScreenManager& manager;
    sf::RenderWindow& window;

    bool showUsernameInput;
    std::string username;
    std::string inputBuffer;

    void drawUsernameInput();

public:
    explicit UsernameScreen(ScreenManager& mgr, sf::RenderWindow& win);
    void render(sf::RenderWindow& window) override;
    void handleMouseClick(int x, int y) override;
    void handleKeyPress(sf::Keyboard::Key key) override;
    void handleTextInput(uint32_t unicode) override;
};