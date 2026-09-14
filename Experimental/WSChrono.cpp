#include "WSChrono.hpp"

#include <Arduino.h>

WSChrono::WSChrono(Adafruit_SSD1306 *anOledDisplay)
{
    oledDisplay = anOledDisplay;
    reset();
}

void WSChrono::tick()
{
    int nowMillis = millis();
    CurrentMillis = CurrentMillis + (nowMillis - OldMillis);
    OldMillis = nowMillis;

    if (CurrentMillis >= 1000)
    {
        ChronoSecond = ChronoSecond + 1;
        CurrentMillis = 0;
    }

    if (ChronoSecond >= 60)
    {
        ChronoMinute = ChronoMinute + 1;
        ChronoSecond = 0;
    }

    if (ChronoMinute >= 60)
    {
        ChronoHour = ChronoHour + 1;
        ChronoMinute = 0;
    }

    if (ChronoHour >= 96)
    {
        Serial.print("Stack overflow in Days:WSChrono line 36");
    }
}

void WSChrono::reset()
{
    ChronoSecond = 0;
    ChronoHour = 0;
    ChronoMinute = 0;
    ChronoSecond = 0;
    ChronoMillis = 0;
    CurrentMillis = 0;
    OldMillis = 0;
    Hours = 0;
    Minutes = 0;
    Seconds = 0;
}

void WSChrono::display()
{
    oledDisplay->clearDisplay();

    oledDisplay->setTextColor(WHITE);

    String title = "STOPWATCH";

    oledDisplay->setTextSize(2);
    oledDisplay->setCursor(0,0);
    oledDisplay->print(title);

    oledDisplay->setTextSize(4);
    oledDisplay->setCursor(0,18);
    oledDisplay->print(ChronoMinute,10);

    oledDisplay->drawChar(52, 18, ':', WHITE, BLACK, 4);

    oledDisplay->setTextSize(4);
    oledDisplay->setCursor(78,18);
    oledDisplay->print(ChronoSecond,10);

    oledDisplay->setTextSize(2);
    oledDisplay->setCursor(52,50);
    oledDisplay->print(CurrentMillis,10);

    oledDisplay->display();
}

void WSChrono::encoderDidClick()
{
    reset();
}

void WSChrono::encoderDidUp()
{

}

void WSChrono::encoderDidDown()
{

}
