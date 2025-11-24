#include "SudokuUI.h"
#include "../SudokuLib/SudokuGame.h"
#include "../SudokuLib/Difficulty.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(700, 720)), "Sudoku");
    window.setFramerateLimit(60);

    SudokuGame game(Difficulty::EASY);
    SudokuUI ui(&game, window);

    if (!ui.loadFont("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Font not found!\n";
        return -1;
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    ui.handleMouseClick(mousePos.x, mousePos.y);
                }
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                ui.handleKeyPress(keyPressed->code);
            }
            else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                ui.handleTextInput(textEntered->unicode);
            }
        }

        ui.render();
        window.display();
    }

    return 0;
}