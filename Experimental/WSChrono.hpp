#pragma once

#include "WatchState.hpp"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

class WSChrono : public WatchState
{
public:
    WSChrono(Adafruit_SSD1306 *anOledDisplay);

    void tick() override;
    void display() override;
    void reset() override;

    void encoderDidClick() override;
    void encoderDidUp() override;
    void encoderDidDown() override;

private:
    Adafruit_SSD1306 *oledDisplay;

    int ChronoHour;
    int ChronoMinute;
    int ChronoSecond;
    int ChronoMillis;
    int CurrentMillis;
    int OldMillis;
    int Hours;
    int Minutes;
    int Seconds;
};
