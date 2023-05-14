#include <Adafruit_DotStar.h>
#include <Adafruit_SSD1327.h>
#include <splash.h>

// Use hardware timer for time-critical events.
#define USING_TIMER_TC3 true
#define TIMER_INTERVAL_MS 1000/60
#include <SAMDTimerInterrupt.h>
SAMDTimer timer(TIMER_TC3);

#define BUTTON_1 A2
#define BUTTON_2 A3
#define BUTTON_3 A4
#define POWER_LED 13

#define WHITE SSD1327_WHITE
#define BLACK SSD1327_BLACK

// Everything is defined in the Board Support Package
// DOTSTAR_NUM        number of onboard DotStars (typically just 1)
// PIN_DOTSTAR_DATA   onboard DotStar data pin
// PIN_DOTSTAR_CLK    onboard DotStar clock pin
Adafruit_DotStar rgbLed(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BGR);
Adafruit_SSD1327 screen(128, 128, &Wire, -1, 1000000);

volatile bool aPrev, aCurr, bPrev, bCurr, cPrev, cCurr;
volatile int counter = 0;
volatile bool aPressed, bPressed, cPressed;

void setup() {
  // Show that we're running
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, HIGH);
  
  rgbLed.begin();
  // This thing is WAY too bright by default
  rgbLed.setBrightness(7);
  rgbLed.setPixelColor(0, 0, 0, 0);
  rgbLed.show();

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);

  screen.begin();
  drawLogo();
  screen.display();
  delay(1000);
  screen.clearDisplay();

  timer.attachInterruptInterval_MS(1000/60, update);
}

void update() {
  aPrev = aCurr;
  bPrev = bCurr;
  cPrev = cCurr;
  aCurr = !digitalRead(BUTTON_1);
  bCurr = !digitalRead(BUTTON_2);
  cCurr = !digitalRead(BUTTON_3);
  aPressed = aPressed || aCurr;
  bPressed = bPressed || bCurr;
  cPressed = cPressed || cCurr;
  counter = counter - (aCurr > aPrev) + (cCurr > cPrev); 
}

void loop() {
  rgbLed.setPixelColor(0, 255 * aPressed, 255 * bPressed, 255 * cPressed);
  rgbLed.show();
  screen.clearDisplay();
  drawHelloWorld();
  drawButtons();
  drawCounter();
  screen.display();
}

void drawLogo() {
  screen.drawBitmap(
    (128 - splash1_width) / 2, 
    (128 - splash1_height) / 2,
    splash1_data, 
    splash1_width, 
    splash1_height, 
    15
  );
}

void drawButtons() {
  if (aPressed) {
    screen.fillCircle(16, 64, 14, WHITE);
    aPressed = false;
  }
  screen.drawCircle(16, 64, 14, WHITE);

  if (bPressed) {
    screen.fillCircle(64, 64, 14, WHITE);
    bPressed = false;
  }
  screen.drawCircle(64, 64, 14, WHITE);
  
  if (cPressed) {
    screen.fillCircle(112, 64, 14, WHITE);
    cPressed = false;
  }
  screen.drawCircle(112, 64, 14, WHITE);
}

void drawHelloWorld() {
  screen.setTextSize(2);
  screen.setTextWrap(true);
  screen.setTextColor(WHITE);
  screen.setCursor(0, 0);
  screen.print("hemlo\nworld");
}

void drawCounter() {
  screen.setTextSize(2);
  screen.setCursor(0, 7 * 16);
  screen.printf("%d", counter);
}