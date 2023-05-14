#include <Adafruit_DotStar.h>
#include <Adafruit_SSD1327.h>
#include <splash.h>

// these pins are defined by the itsybitsy's board file
Adafruit_DotStar rgbLed(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BGR);

#define OLED_DC A5
#define OLED_RESET -1
#define OLED_CS A4
Adafruit_SSD1327 screen(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);

#define BUTTON_1 9
#define BUTTON_2 10
#define BUTTON_3 11
#define POWER_LED 13

#define WHITE SSD1327_WHITE
#define BLACK SSD1327_BLACK

bool aPrev, aCurr, bPrev, bCurr, cPrev, cCurr;
int counter = 0;
bool aPressed, bPressed, cPressed;
unsigned long lastFrameTime;
#define FRAME_ADVANCE 1000/60

/* arduino entry points: setup, loop */

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
  screen.setRotation(3);
  drawLogo();
  screen.display();
  delay(1000);
  lastFrameTime = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastFrameTime < FRAME_ADVANCE) {
    delay(1);
    return;
  }
  lastFrameTime += FRAME_ADVANCE;
  update();
  draw();
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

void draw() {
  rgbLed.setPixelColor(0, 255 * aPressed, 255 * bPressed, 255 * cPressed);
  rgbLed.show();
  screen.clearDisplay();
  drawHelloWorld();
  drawButtons();
  drawCounter();
  screen.display();
}

/* helper functions */

void drawLogo() {
  screen.drawBitmap(
    (128 - splash1_width) / 2, 
    (128 - splash1_height) / 2,
    splash1_data, 
    splash1_width, 
    splash1_height, 
    WHITE
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