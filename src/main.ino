#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN D1
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red color
    pixels.show();
    delay(500);
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
    pixels.show();
    delay(500);
  }
}