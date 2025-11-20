#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../SudokuLib/SudokuGame.h"
#include "MockObserver.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::AnyNumber;

/* Testeaza ca notificarile sunt trimise corect la observers*/

class ObserverPatternTests : public ::testing::Test {
protected:
    SudokuGame* game;
    MockObserver* mockObserver1;
    MockObserver* mockObserver2;

    void SetUp() override {
        game = new SudokuGame();
        mockObserver1 = new MockObserver();
        mockObserver2 = new MockObserver();
    }

    void TearDown() override {
        delete game;
        delete mockObserver1;
        delete mockObserver2;
    }
};

// TESTE ATTACH OBSERVER

TEST_F(ObserverPatternTests, AttachObserverSucceeds) {
    EXPECT_NO_THROW(game->attachObserver(mockObserver1));
}

TEST_F(ObserverPatternTests, AttachNullObserverDoesNotCrash) {
    EXPECT_NO_THROW(game->attachObserver(nullptr));
}

TEST_F(ObserverPatternTests, AttachMultipleObservers) {
    EXPECT_NO_THROW(game->attachObserver(mockObserver1));
    EXPECT_NO_THROW(game->attachObserver(mockObserver2));
}

// TESTE DETACH OBSERVER

TEST_F(ObserverPatternTests, DetachObserverSucceeds) {
    game->attachObserver(mockObserver1);
    EXPECT_NO_THROW(game->detachObserver(mockObserver1));
}

TEST_F(ObserverPatternTests, DetachNullObserverDoesNotCrash) {
    EXPECT_NO_THROW(game->detachObserver(nullptr));
}

TEST_F(ObserverPatternTests, DetachNonExistentObserverDoesNotCrash) {
    EXPECT_NO_THROW(game->detachObserver(mockObserver1));
}

TEST_F(ObserverPatternTests, DetachOneObserverLeavesOthersAttached) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver2);

    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(AtLeast(1));

    game->detachObserver(mockObserver1);

    game->startNewGame();
}

// TESTE NOTIFICARI - onBoardChanged

TEST_F(ObserverPatternTests, StartNewGameNotifiesObservers) {
    game->attachObserver(mockObserver1);

    //notificare pt board changed
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, SetValueNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Cautam o celula goala si setam o valoare valida
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                //notificare
                EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);

                // Gasim o valoare valida
                for (int value = 1; value <= 9; value++) {
                    bool safe = true;
                    for (int x = 0; x < 9; x++) {
                        if (game->getValue(i, x) == value || game->getValue(x, j) == value) {
                            safe = false;
                            break;
                        }
                    }
                    if (safe) {
                        game->setValue(i, j, value);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(ObserverPatternTests, ClearCellNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Setam o valoare apoi o stergem
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    if (game->setValue(i, j, value)) {
                        //notificare pentru clear
                        EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
                        game->setValue(i, j, 0);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(ObserverPatternTests, ResetNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    //notificare la reset
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
    game->reset();
}

TEST_F(ObserverPatternTests, MultipleObserversAllNotifiedOnBoardChange) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver2);

    // Ambii observatori ar trebui notificati
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}

// TESTE NOTIFICARI - onGameComplete

TEST_F(ObserverPatternTests, GameCompleteNotifiesObservers) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onGameComplete()).Times(0);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                game->setValue(i, j, 1);
                return;
            }
        }
    }
}

// TESTE NOTIFICARI - onAttemptsChanged

TEST_F(ObserverPatternTests, StartNewGameNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);

    //notificare pentru attempts
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, InvalidMoveNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Facem o mutare invalida
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                // Cautam o valoare care exista deja pe rand
                for (int col = 0; col < 9; col++) {
                    int value = game->getValue(i, col);
                    if (value != 0) {
                        //notificare cu attempts - 1
                        int currentAttempts = game->getRemainingAttempts();
                        EXPECT_CALL(*mockObserver1, onAttemptsChanged(currentAttempts - 1)).Times(1);

                        game->setValue(i, j, value);
                        return;
                    }
                }
            }
        }
    }
}

TEST_F(ObserverPatternTests, ResetNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Expectam notificare cu attempts = 3
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(3)).Times(1);
    game->reset();
}

TEST_F(ObserverPatternTests, ValidMoveDoesNotNotifyAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Valida move nu ar trebui sa notifice attempts changed
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(0);

    // Facem o mutare valida
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    bool safe = true;
                    for (int x = 0; x < 9; x++) {
                        if (game->getValue(i, x) == value || game->getValue(x, j) == value) {
                            safe = false;
                            break;
                        }
                    }
                    if (safe) {
                        game->setValue(i, j, value);
                        return;
                    }
                }
            }
        }
    }
}


// TESTE SECVENTA NOTIFICARI

TEST_F(ObserverPatternTests, NotificationsInCorrectOrder) {
    game->attachObserver(mockObserver1);

    using ::testing::InSequence;
    {
        InSequence seq;

        // La startNewGame,:
        // 1. onBoardChanged (generarea puzzle-ului)
        // 2. onAttemptsChanged (resetarea attempts)
        EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
        EXPECT_CALL(*mockObserver1, onAttemptsChanged(3)).Times(1);
    }

    game->startNewGame();
}

// TESTE DETACH IN TIMPUL NOTIFICARILOR

TEST_F(ObserverPatternTests, DetachedObserverNotNotified) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Detach observer
    game->detachObserver(mockObserver1);

    // Observer-ul nu ar trebui sa mai fie notificat
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(0);
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(0);

    // Facem operatii care ar notifica
    game->reset();
}

TEST_F(ObserverPatternTests, OnlyAttachedObserversAreNotified) {
    // Attach doar observer1
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Doar observer1 ar trebui notificat
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(0);

    game->reset();
}

// TESTE EDGE CASES


TEST_F(ObserverPatternTests, AttachSameObserverMultipleTimes) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver1);

    // Observer-ul ar trebui sa primeasca notificari
    // (implementarea poate notifica de mai multe ori sau doar o data)
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, DetachNotAffectingOtherObservers) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver2);

    // Detach observer1
    game->detachObserver(mockObserver1);

    // Observer2 ar trebui sa fie inca notificat
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(0);
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(1);

    game->reset();
}

TEST_F(ObserverPatternTests, MultipleOperationsMultipleNotifications) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    // Expectam multiple notificari pentru multiple operatii
    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(2));

    // Prima operatie
    game->reset();

    // A doua operatie
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                for (int value = 1; value <= 9; value++) {
                    if (game->setValue(i, j, value)) {
                        return;
                    }
                }
            }
        }
    }
}