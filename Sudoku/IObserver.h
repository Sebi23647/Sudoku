#ifndef IOBSERVER_H
#define IOBSERVER_H

class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void onBoardChanged() = 0;
    virtual void onGameComplete() = 0;
    virtual void onAttemptsChanged(int remaining) = 0;
};

#endif