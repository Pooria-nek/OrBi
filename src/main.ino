#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FS.h> // Include the SPIFFS library
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "define.h"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Callback function that will be executed when data is received
void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  Serial.print("Received data from: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.print(mac[i], HEX);
    if (i < 5)
      Serial.print(":");
  }
  Serial.print(" Data: ");
  for (int i = 0; i < len; i++)
  {
    Serial.print(incomingData[i]);
  }
  Serial.println();
  // Add your code to handle the received data
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  File configFile = SPIFFS.open("/config.txt", "r");
  if (!configFile)
  {
    Serial.println("Failed to open config file");
    return;
  }
  while (configFile.available())
  {
    String line = configFile.readStringUntil('\n');
    Serial.println(line);
  }
  configFile.close();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onDataRecv);
}

void loop()
{
  // fireEffect();
  // delay(1000);
  // rainbowEffect();
  // delay(1000);
  // lightningEffect();
  // delay(1000);
  siriHomePodEffect();
  delay(1000);
  // confirmEffect();
  // delay(1000);
  // errorEffect();
  // delay(1000);
}

void fireEffect()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, random(100, 255), 0)); // Fire colors
  }
  pixels.show();
  delay(50);
}

void rainbowEffect()
{
  uint16_t i, j;
  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(20);
  }
}

void lightningEffect()
{
  int flashes = random(3, 6); // Number of flashes in one lightning sequence
  for (int j = 0; j < flashes; j++)
  {
    int flashDuration = random(50, 200);
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // White color
    }
    pixels.show();
    delay(flashDuration);
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
    }
    pixels.show();
    delay(flashDuration);
  }
  delay(random(1000, 5000)); // Delay before next lightning sequence
}

void siriHomePodEffect()
{
  for (int j = 0; j < 256; j++)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, Wheel((i * 256 / NUMPIXELS + j) & 255));
    }
    pixels.show();
    delay(20);
  }
}

void confirmEffect()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Green color for confirmation
  }
  pixels.show();
  delay(500);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
  }
  pixels.show();
}

void errorEffect()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red color for error
  }
  pixels.show();
  delay(500);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
  }
  pixels.show();
}

uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}