#include "WSClock.hpp"

#include <Arduino.h>

WSClock::WSClock(Adafruit_SSD1306 *anOledDisplay)
    : currentSelection(NONE)
{
    oledDisplay = anOledDisplay;
    setTime(16, 34, 0, 17, 8, 2026); // hour, min, sec, day, month, year // delete from here
    isVisible = true;
}

void WSClock::tick()
{
    hours = hour();
    minutes = minute();
    seconds = second();
}

void WSClock::displayBlinked(int aValue)
{
    if (isVisible)
    {
      oledDisplay->print(aValue,10);
      isVisible = false;
    }
    else
    {
      isVisible = true;
    }
}

void WSClock::display()
{
    oledDisplay->clearDisplay();

    oledDisplay->setTextColor(WHITE);

    String title = "TIME";

    oledDisplay->setTextSize(2);
    oledDisplay->setCursor(0,0);
    oledDisplay->print(title);

    oledDisplay->setTextSize(4);
    oledDisplay->setCursor(0,18);

    if (currentSelection == Selection::HOURS)
    {
        displayBlinked(hours);
    }
    else
    {
      oledDisplay->print(hours,10);
    }

    oledDisplay->drawChar(52, 18, ':', WHITE, BLACK, 4);

    oledDisplay->setTextSize(4);
    oledDisplay->setCursor(78,18);

    if (currentSelection == Selection::MINUTES)
    {
        displayBlinked(minutes);
    }
    else
    {
      oledDisplay->print(minutes,10);
    }

    oledDisplay->setTextSize(2);
    oledDisplay->setCursor(52,50);
    oledDisplay->print(seconds,10);

    oledDisplay->display();
}

void WSClock::encoderDidClick()
{
    currentSelection = currentSelection + 1;
    if (currentSelection == MAX_SELECTION)
    {
        currentSelection = Selection::NONE;
    }
}

void WSClock::encoderDidUp()
{
    int localHours = hour();
    int localMinutes = minute();

    switch(currentSelection)
    {
        case HOURS:
        {
            localHours++;
            if (localHours > 23) 
            {
                localHours = 0;
            }
            break;
        }
        
        case MINUTES:
        {
            localMinutes++;
            if (localMinutes > 59) 
            {
                localMinutes = 0;
            }
            break;
        }
        default: break;
    }

    setTime(localHours, localMinutes, seconds, 17, 8, 2026);
}

void WSClock::encoderDidDown()
{
    int localHours = hour();
    int localMinutes = minute();

    switch(currentSelection)
    {
        case HOURS:
        {
            localHours--;
            if (localHours < 0) 
            {
                localHours = 23;
            }
            break;
        }
        
        case MINUTES:
        {
            localMinutes--;
            if (localMinutes < 0) 
            {
                localMinutes = 59;
            }
            break;
        }

        default: break;
    }

    setTime(localHours, localMinutes, seconds, 17, 8, 2026);
}
