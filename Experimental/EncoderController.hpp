#pragma once

typedef void (*EncoderCallback)();

class EncoderController
{
public:
    EncoderController(int aCLKPin, int aDTPin);

    void setup();
    void process();

    EncoderCallback didUpCallback;
    EncoderCallback didDownCallback;

private:
    int clkPin;
    int dtPin;

    int lastClk;
};
