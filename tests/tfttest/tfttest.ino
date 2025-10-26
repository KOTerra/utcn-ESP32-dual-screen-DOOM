// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7789.h>
// #include <SPI.h>

// #define TFT_CS   16
// #define TFT_DC   17
// #define TFT_RST   5

// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// void setup() {
//   Serial.begin(115200);

//   // Reset the display
//   pinMode(TFT_RST, OUTPUT);
//   digitalWrite(TFT_RST, LOW);
//   delay(50);
//   digitalWrite(TFT_RST, HIGH);
//   delay(50);

//   SPI.begin();          // hardware SPI: SCLK=18, MOSI=23
//   tft.init(240, 320);   // 240x320 display
//   tft.setRotation(1);
//   tft.fillScreen(ST77XX_BLACK);

//   tft.setTextColor(ST77XX_GREEN);
//   tft.setTextSize(3);
//   tft.setCursor(40, 120);
//   tft.println("Hello!");
// }

// void loop() {}


#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

// Pin configuration
#define TFT_CS    22
#define TFT_DC    21
#define TFT_RST   18
#define TFT_BL     5

// SPI pins (ESP32-S3)
#define TFT_SCLK  19
#define TFT_MOSI  13

// Create display object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-S3 + ILI9341 Test");

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // turn backlight on

  // Initialize SPI bus
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Initialize display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  // Test output
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.setCursor(40, 120);
  tft.println("Hello Doom!");
}

void loop() {
  // Simple test animation
  tft.fillScreen(ILI9341_RED);
  delay(500);
  tft.fillScreen(ILI9341_GREEN);
  delay(500);
  tft.fillScreen(ILI9341_BLUE);
  delay(500);
}
