#pragma once

#include "WatchState.hpp"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

class WSClock : public WatchState
{
public:
    WSClock(Adafruit_SSD1306 *anOledDisplay);

    void tick() override;
    void display() override;

    void encoderDidClick() override;
    void encoderDidUp() override;
    void encoderDidDown() override;

private:
    Adafruit_SSD1306 *oledDisplay;

    enum Selection
    { 
        NONE, 
        HOURS, 
        MINUTES, 

        MAX_SELECTION
    };

    Selection currentSelection;

    int hours;
    int minutes;
    int seconds;

    bool isVisible;

    void displayBlinked(int aValue);
};
