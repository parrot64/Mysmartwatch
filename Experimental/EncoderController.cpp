#include "EncoderController.hpp"

#include <Arduino.h>

EncoderController::EncoderController(int aCLKPin, int aDTPin)
    : clkPin(aCLKPin)
    , dtPin(aDTPin)
    , lastClk(HIGH)
    , didUpCallback(NULL)
    , didDownCallback(NULL)
{
}

void EncoderController::setup()
{
    pinMode(clkPin, INPUT); 
    pinMode(dtPin, INPUT); 
}

void EncoderController::process()
{
    int dtValue = digitalRead(dtPin);
    if (dtValue == HIGH) 
    {
        this->didUpCallback();
    }
    if (dtValue == LOW) 
    {
        this->didDownCallback();
    }
}
