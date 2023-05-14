#include <Adafruit_DotStar.h>
#include <Adafruit_SSD1327.h>
#include <splash.h>

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
Adafruit_SSD1327 screen(128, 128);

bool a, b, c;

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
}

void loop() {
  a = !digitalRead(BUTTON_1);
  b = !digitalRead(BUTTON_2);
  c = !digitalRead(BUTTON_3);

  rgbLed.setPixelColor(0, 255 * a, 255 * b, 255 * c);
  rgbLed.show();

  screen.clearDisplay();
  drawHelloWorld();
  drawButtons();
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
  if (a) screen.fillCircle(16, 64, 14, WHITE);
  screen.drawCircle(16, 64, 14, WHITE);

  if (b) screen.fillCircle(64, 64, 14, WHITE);
  screen.drawCircle(64, 64, 14, WHITE);
  
  if (c) screen.fillCircle(112, 64, 14, WHITE);
  screen.drawCircle(112, 64, 14, WHITE);
}

void drawHelloWorld() {
  screen.setTextSize(2);
  screen.setTextWrap(true);
  screen.setTextColor(WHITE);
  screen.setCursor(0, 0);
  screen.print("hemlo\nworld");
}