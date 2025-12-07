#pragma once
#include <SFML/Graphics.hpp>

class IScreen {
public:
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleMouseClick(int x, int y) = 0;
    virtual void handleKeyPress(sf::Keyboard::Key key) = 0;
    virtual void handleTextInput(uint32_t unicode) = 0;

    virtual ~IScreen() = default;
};
