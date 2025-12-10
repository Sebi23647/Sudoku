#pragma once
#include "IScreen.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "../SudokuLib/ISudokuGame.h"
#include "../SudokuLib/Difficulty.h"
#include "../SudokuLib/IHintProvider.h"
#include <string>

enum class ScreenType {
    Username,
    Difficulty,
    Game,
    Leaderboard
};

class ScreenManager {
private:
    std::unique_ptr<IScreen> currentScreen;
    ScreenType currentType;

    std::unique_ptr<ISudokuGame> gamePtr;
    std::unique_ptr<IHintProvider> hintMgr;
    sf::Font& appFont;

    std::string username;

    // pending change applied after event processing
    bool hasPendingScreen = false;
    ScreenType pendingScreen;

    bool hasPendingCreate = false;
    Difficulty pendingCreateDifficulty;

public:
    explicit ScreenManager(sf::Font& font);

    void setScreen(ScreenType type, sf::RenderWindow& window);

    void requestScreen(ScreenType type);
    void requestCreateGame(Difficulty difficulty);
    void applyPending(sf::RenderWindow& window);

    IScreen* getScreen() { return currentScreen.get(); }
    ScreenType getCurrentType() const { return currentType; }

    void createGame(Difficulty difficulty, sf::RenderWindow& window);

    sf::Font& getFont() { return appFont; }
    ISudokuGame* getGame() { return gamePtr ? gamePtr.get() : nullptr; }
    IHintProvider* getHintManager() { return hintMgr ? hintMgr.get() : nullptr; }

    void setUsername(const std::string& name) { username = name; }
    const std::string& getUsername() const { return username; }
};