#pragma once

typedef void (*DidClickCallback)();

class ButtonController
{
public:
    ButtonController(int aButtonPin);
    void setup();
    void process();

    DidClickCallback didClickCallback;

private:
    bool isButtonDown();

    int buttonPin;

    bool buttonDidUp;
};
