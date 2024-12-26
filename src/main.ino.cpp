# 1 "/var/folders/kg/vwrj5_pj0db_cf5tz1d8z5k40000gn/T/tmpf00r43gt"
#include <Arduino.h>
# 1 "/Users/mahdi/Documents/PlatformIO/Projects/OrBi/src/main.ino"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN D1
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup();
void loop();
#line 9 "/Users/mahdi/Documents/PlatformIO/Projects/OrBi/src/main.ino"
void setup() {

  Serial.begin(9600);
  Serial.println("Hello World");
  pixels.begin();
  pixels.show();
}

void loop() {

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
    delay(500);
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(500);
  }
}