#include "ScreenManager.h"
#include "UsernameScreen.h"
#include "DifficultyScreen.h"
#include "GameScreen.h"
#include "LeaderboardScreen.h"

#include <thread>
#include "../SudokuLib/GameFactory.h"
#include "../SudokuLib/HintFactory.h"

ScreenManager::ScreenManager(sf::Font& font)
    : currentScreen(nullptr), currentType(ScreenType::Username),
      gamePtr(nullptr), hintMgr(nullptr), appFont(font), hasPendingScreen(false), hasPendingCreate(false)
{
}

void ScreenManager::setScreen(ScreenType type, sf::RenderWindow& window) {
    currentType = type;

    switch (type) {
    case ScreenType::Username:
        currentScreen = std::make_unique<UsernameScreen>(*this, window);
        break;

    case ScreenType::Difficulty:
        currentScreen = std::make_unique<DifficultyScreen>(*this, window);
        break;

    case ScreenType::Game:
        currentScreen = std::make_unique<GameScreen>(*this, window);
        break;

    case ScreenType::Leaderboard:
        currentScreen = std::make_unique<LeaderboardScreen>(*this, window);
        break;
    }
}

void ScreenManager::createGame(Difficulty difficulty, sf::RenderWindow& window) {
    gamePtr = CreateSudokuGame(difficulty);
    hintMgr = CreateHintManager(difficulty);

    setScreen(ScreenType::Game, window);
    std::thread([this]() {
        if (gamePtr) {
            gamePtr->startNewGame();
        }
    }).detach();
}

void ScreenManager::requestScreen(ScreenType type) {
    hasPendingScreen = true;
    pendingScreen = type;
}

void ScreenManager::requestCreateGame(Difficulty difficulty) {
    hasPendingCreate = true;
    pendingCreateDifficulty = difficulty;
}

void ScreenManager::applyPending(sf::RenderWindow& window) {
    if (hasPendingScreen) {
        setScreen(pendingScreen, window);
        hasPendingScreen = false;
    }

    if (hasPendingCreate) {
        createGame(pendingCreateDifficulty, window);
        hasPendingCreate = false;
    }
}