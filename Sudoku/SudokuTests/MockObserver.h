#pragma once
#include <gmock/gmock.h>
#include "../SudokuLib/IObserver.h"

/**
 * Mock Observer pentru testarea Observer Pattern
 * Folosim GMock pentru a verifica ca metodele sunt apelate corect
 */
class MockObserver : public IObserver {
public:
    MOCK_METHOD(void, onBoardChanged, (), (override));
    MOCK_METHOD(void, onGameComplete, (), (override));
    MOCK_METHOD(void, onAttemptsChanged, (int remaining), (override));
};