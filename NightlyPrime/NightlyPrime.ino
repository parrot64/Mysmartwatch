#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <ezButton.h>

#define OLED_RESET     -1
#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 4
#define DIRECTION_CW 0  // clockwise direction
#define DIRECTION_CCW 1 // counter-clockwise direction

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

ezButton button(SW_PIN); // create ezButton object for pin 7;

unsigned long OldMillis;
unsigned long NewMillis;

// TODO: const -> #define
const int buttonPin = 2;

int buttonState = 0;
int MenuState = 0;
int ChronoHour = 0;
int ChronoMinute = 0;
int ChronoSecond = 0;
int ChronoMillis = 0;
int Hours;
int Minutes;
int Seconds;
int Millis;
int counter = 0;
int direction = DIRECTION_CW;
int CLK_state;
int prev_CLK_state;

//enum MenuState
//{
//    DISPLAY_TIME,
//    DISPLAY_CHRONO,
//    DISPLAY_SETTINGS,
//
//    MAX_MENUSTATE
//}

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

    if (button.isPressed()) 
    {
        MenuState = 0;
        delay(200);
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

    // TODO: is buttonState used?
    buttonState = digitalRead(buttonPin);

    // TODO: separate displaying logic and controling logic
    if (button.isPressed())
    {
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
    pinMode(CLK_PIN, INPUT);
     pinMode(DT_PIN, INPUT);
    button.setDebounceTime(50);
    prev_CLK_state = digitalRead(CLK_PIN);
}

void loop() 
{
    ButtonEncoderChange();
    ChronoTick();
    MenuChange();
//    Serial.println(NewMillis);
}



// TODO: Function name should describe what this function do
void ButtonEncoderChange()
{
    // TODO: dont mix button logic and encoder logic
     button.loop(); // MUST call the loop() function first

     // read the current state of the rotary encoder's CLK pin
     CLK_state = digitalRead(CLK_PIN);

     // If the state of CLK is changed, then pulse occurred
     // React to only the rising edge (from LOW to HIGH) to avoid double count
     if (CLK_state != prev_CLK_state && CLK_state == HIGH)
     {
         // if the DT state is HIGH
         // The encoder is rotating in counter-clockwise direction => decrease the counter
         if (digitalRead(DT_PIN) == HIGH)
        {
             counter--;
             direction = DIRECTION_CCW;
         }
         else
         {
             // The encoder is rotating in clockwise direction => increase the counter
             counter++;
             direction = DIRECTION_CW;
         }

// TODO: No Serial.print should be in loop while reading encoder
         Serial.print("Rotary Encoder:: direction: ");
         if (direction == DIRECTION_CW)
             Serial.print("Clockwise");
         else
             Serial.print("Counter-clockwise");

         Serial.print(" - count: ");
         Serial.println(counter);
     }

     // save last CLK state
     prev_CLK_state = CLK_state;
}
