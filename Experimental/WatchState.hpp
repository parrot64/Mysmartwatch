#pragma once

class WatchState
{
public:
    WatchState();

    virtual void tick();
    virtual void display();
    virtual void reset();

    virtual void encoderDidClick();
    virtual void encoderDidUp();
    virtual void encoderDidDown();

    WatchState *nextState;
};