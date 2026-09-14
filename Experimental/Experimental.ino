#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

#define ENCODER_DO_NOT_USE_INTERRUPS
#include <Encoder.h>

#include "WatchState.hpp"
#include "WSClock.hpp"

#include "WSChrono.hpp"

#include "ButtonController.hpp"


#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET); //Declaration for the size,setting,etc. of the OLED

WatchState *currentState = NULL;

WSClock clockState(&display);
WSChrono chronoState(&display);

ButtonController buttonController(4);
ButtonController encoderButtonController(5);

#define ENCODER_CLK 2
#define ENCODER_DT 3
Encoder encoderController(ENCODER_CLK, ENCODER_DT);

void initStates()
{
    chronoState.nextState = &clockState;
    clockState.nextState = &chronoState;
    currentState = &clockState;
}

void switchState()
{
    Serial.println("switch state");
    currentState = currentState->nextState;
    currentState->reset();
}

void encoderDidClick()
{
    Serial.println("encoder did click");
    currentState->encoderDidClick();
}

void encoderDidUp()
{
    // Serial.println("encoder did up");
    currentState->encoderDidUp();
}

void encoderDidDown()
{
    // Serial.println("encoder did down");
    currentState->encoderDidDown();
}

void setup() 
{
    Serial.begin(9600); // Start serial communication at 9600 baud
    Serial.println("Debugging started...");

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    initStates();

    buttonController.setup();
    encoderButtonController.setup();

    buttonController.didClickCallback = switchState;
    encoderButtonController.didClickCallback = encoderDidClick;
}

void encoderProcess()
{
    static int pos = 0;

    int newPos = encoderController.read();
    if (pos < newPos) 
    {
        encoderDidUp();
        pos = newPos;
    }
    else if (pos > newPos)
    {
        encoderDidDown();
        pos = newPos;
    }
}

void loop() 
{
    buttonController.process();
    encoderButtonController.process();
    encoderProcess();

    currentState->tick();
    currentState->display();
}
