#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET); //Declaration for the size,setting,etc. of the OLED
unsigned long OldMillis;
unsigned long NewMillis;
const int buttonPin = 2;  // Pin connected to button
int buttonState = 0;      // Variable to store button state
int MenuState = 0;
int ChronoHour = 0;
int ChronoMinute = 0;
int ChronoSecond = 0;
int ChronoMillis = 0;
int Hours;
int Minutes;
int Seconds;
int Millis;

enum MenuState
{
    DISPLAY_TIME,
    DISPLAY_CHRONO,
    DISPLAY_SETTINGS,

    MAX_MENUSTATE
}

void displayChrono()
{
    display.clearDisplay();

    display.setTextColor(WHITE);

    String title = "STOPWATCH";

    int Hours = ChronoHour;
    int Minutes = ChronoMinute;
    int Seconds = ChronoSecond;
    int Millis = ChronoMillis;

    display.setTextSize(2);
    display.setCursor(0,0);
    display.print(title);

    display.setTextSize(4);
    display.setCursor(0,18);
    display.print(ChronoHour,10);

    display.drawChar(52, 18, ':', WHITE, BLACK, 4);

    display.setTextSize(4);
    display.setCursor(78,18);
    display.print(ChronoMinute,10);

    display.setTextSize(2);
    display.setCursor(30,50);
    display.print(ChronoSecond,10);

    display.drawChar(57, 50, ':', WHITE, BLACK, 2);

    display.setTextSize(2);
    display.setCursor(69,50);
    display.print(ChronoMillis,10);

    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) { // LOW means pressed
        MenuState = MENU_TIME;
        delay(200); // Simple debounce delay
    }

    display.display();
}

void ChronoTick()
{
    NewMillis = millis();
    ChronoMillis =  ChronoMillis +(NewMillis - OldMillis);
    OldMillis = NewMillis;

    if (ChronoMillis >= 1000)
    {
        ChronoSecond = ChronoSecond + 1;
        ChronoMillis = 0;
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

    if (ChronoHour >= 99)
    {
        Serial.println("OVERFLOW ERROR");
        ChronoHour = 0;
    }
}

void MenuChange()
{
    if (MenuState == 0)
    {
        displayTime();
    }
    else 
    {
        displayChrono();
    }
}

void displayTime()
{
    display.clearDisplay();

    display.setTextColor(WHITE);

    int Hours = hour();
    int Minutes = minute();
    int Seconds = second();

    String title = "TIME";

    display.setTextSize(2);
    display.setCursor(0,0);
    display.print(title);

    display.setTextSize(4);
    display.setCursor(0,18);
    display.print(Hours,10);

    display.drawChar(52, 18, ':', WHITE, BLACK, 4);

    display.setTextSize(4);
    display.setCursor(78,18);
    display.print(Minutes,10);

    display.setTextSize(2);
    display.setCursor(52,50);
    display.print(Seconds,10);

    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) { // LOW means pressed
        MenuState = 1;
        delay(200); // Simple debounce delay
    }

    display.display();
}

void setup() 
{
    pinMode(buttonPin, INPUT_PULLUP); 
    Serial.begin(9600); // Start serial communication at 9600 baud
    Serial.println("Debugging started...");
    setTime(16, 34, 0, 17, 8, 2026); // hour, min, sec, day, month, year
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
}

void loop() 
{
    ChronoTick();
    MenuChange();
    Serial.println(NewMillis);
}
