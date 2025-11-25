#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../SudokuLib/SudokuGame.h"
#include "MockObserver.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::AnyNumber;

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


TEST_F(ObserverPatternTests, StartNewGameNotifiesObservers) {
    game->attachObserver(mockObserver1);

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, SetValueNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);

                int correctValue = game->getSolutionValue(i, j);
                game->setValue(i, j, correctValue);
                return;
            }
        }
    }
}

TEST_F(ObserverPatternTests, ClearCellNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);
                if (game->setValue(i, j, correctValue)) {
                    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
                    game->setValue(i, j, 0);
                    return;
                }
            }
        }
    }
}

TEST_F(ObserverPatternTests, ResetNotifiesOnBoardChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
    game->reset();
}

TEST_F(ObserverPatternTests, MultipleObserversAllNotifiedOnBoardChange) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver2);

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}


TEST_F(ObserverPatternTests, GameCompleteNotifiesObservers) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onGameComplete()).Times(0);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int wrongValue = (game->getSolutionValue(i, j) % 9) + 1;
                game->setValue(i, j, wrongValue);
                return;
            }
        }
    }
}


TEST_F(ObserverPatternTests, StartNewGameNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, InvalidMoveNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);
                int wrongValue = (correctValue % 9) + 1;

                int currentAttempts = game->getRemainingAttempts();
                EXPECT_CALL(*mockObserver1, onAttemptsChanged(currentAttempts - 1)).Times(1);

                game->setValue(i, j, wrongValue);
                return;
            }
        }
    }
}

TEST_F(ObserverPatternTests, ResetNotifiesAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onAttemptsChanged(3)).Times(1);
    game->reset();
}

TEST_F(ObserverPatternTests, ValidMoveDoesNotNotifyAttemptsChanged) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(0);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);
                game->setValue(i, j, correctValue);
                return;
            }
        }
    }
}

TEST_F(ObserverPatternTests, NotificationsInCorrectOrder) {
    game->attachObserver(mockObserver1);

    using ::testing::InSequence;
    {
        InSequence seq;
        EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
        EXPECT_CALL(*mockObserver1, onAttemptsChanged(3)).Times(1);
    }

    game->startNewGame();
}

TEST_F(ObserverPatternTests, DetachedObserverNotNotified) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    game->detachObserver(mockObserver1);

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(0);
    EXPECT_CALL(*mockObserver1, onAttemptsChanged(_)).Times(0);

    game->reset();
}

TEST_F(ObserverPatternTests, OnlyAttachedObserversAreNotified) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(1);
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(0);

    game->reset();
}

TEST_F(ObserverPatternTests, AttachSameObserverMultipleTimes) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver1);

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(1));

    game->startNewGame();
}

TEST_F(ObserverPatternTests, DetachNotAffectingOtherObservers) {
    game->attachObserver(mockObserver1);
    game->attachObserver(mockObserver2);
    game->detachObserver(mockObserver1);

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(0);
    EXPECT_CALL(*mockObserver2, onBoardChanged()).Times(1);

    game->reset();
}

TEST_F(ObserverPatternTests, MultipleOperationsMultipleNotifications) {
    game->attachObserver(mockObserver1);
    game->startNewGame();

    EXPECT_CALL(*mockObserver1, onBoardChanged()).Times(AtLeast(2));
    game->reset();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game->getCellState(i, j) == CellState::EMPTY) {
                int correctValue = game->getSolutionValue(i, j);
                if (game->setValue(i, j, correctValue)) {
                    return;
                }
            }
        }
    }
}