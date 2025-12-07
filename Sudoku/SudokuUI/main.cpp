#include "ScreenManager.h"
#include "../SudokuLib/Difficulty.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(700, 750)), "Sudoku");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Font not found!\n";
        return -1;
    }

    ScreenManager manager(font);

    manager.setScreen(ScreenType::Username, window);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> evt = window.pollEvent()) {
            const auto& event = *evt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            IScreen* current = manager.getScreen();
            if (!current) continue;

            if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    current->handleMouseClick(mousePos.x, mousePos.y);
                }
            }
            else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                current->handleKeyPress(keyPressed->code);
            }
            else if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
                current->handleTextInput(text->unicode);
            }
        }

        manager.applyPending(window);

        IScreen* screen = manager.getScreen();
        if (screen) screen->render(window);
    }

    return 0;
}