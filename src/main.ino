#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <FS.h> // Include the SPIFFS library
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "define.h"

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t receiverMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define SPEAKER_PIN D2 // Define the pin for the speaker

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
    Serial.print((char)incomingData[i]);
  }
  Serial.println();

  String receivedMessage = "";
  for (int i = 0; i < len; i++)
  {
    receivedMessage += (char)incomingData[i];
  }

  if (receivedMessage == "TIME?")
  {
    String response = "TIME:" + String(millis());
    uint8_t data[response.length() + 1];
    response.getBytes(data, response.length() + 1);
    esp_now_send(mac, data, sizeof(data));
  }
  else if (receivedMessage == "TEMP?")
  {
    float temperature = 25.0; // Replace with actual temperature reading
    String response = "TEMP:" + String(temperature);
    uint8_t data[response.length() + 1];
    response.getBytes(data, response.length() + 1);
    esp_now_send(mac, data, sizeof(data));
  }
  else if (receivedMessage == "HUMIDITY?")
  {
    float humidity = 50.0; // Replace with actual humidity reading
    String response = "HUMIDITY:" + String(humidity);
    uint8_t data[response.length() + 1];
    response.getBytes(data, response.length() + 1);
    esp_now_send(mac, data, sizeof(data));
  }
  // Add more conditions for other types of data requests
}

// Callback function that will be executed when data is sent
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  // if (!SPIFFS.begin())
  // {
  //   Serial.println("Failed to mount file system");
  //   return;
  // }
  // File configFile = SPIFFS.open("/config.txt", "r");
  // if (!configFile)
  // {
  //   Serial.println("Failed to open config file");
  //   return;
  // }
  // while (configFile.available())
  // {
  //   String line = configFile.readStringUntil('\n');
  //   Serial.println(line);
  // }
  // configFile.close();

  // WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress()); // Print MAC address for debugging

  // Initialize WiFi in AP+STA mode
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP8266_AP", "password"); // Set your desired SSID and password
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(onDataRecv);
  esp_now_register_send_cb(onDataSent);
  esp_now_add_peer(receiverMac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  pinMode(SPEAKER_PIN, OUTPUT); // Initialize the speaker pin as an output
}

void loop()
{
  // static unsigned long lastSendTime = 0;
  // unsigned long currentTime = millis();
  // if (currentTime - lastSendTime >= 100) // Send data every 1 second
  // {
  //   String message = "Hello " + String(millis());
  //   uint8_t data[message.length() + 1];
  //   message.getBytes(data, message.length() + 1);
  //   esp_now_send(receiverMac, data, sizeof(data));
  //   lastSendTime = currentTime;
  // }
  
  // fireEffect();
  // delay(1000);
  // rainbowEffect();
  // delay(1000);
  // lightningEffect();
  // delay(1000);
  // siriHomePodEffect();
  // delay(1000);
  // confirmEffect();
  // delay(1000);
  // errorEffect();
  // delay(1000);
  // breathEffect();
  // delay(1000);

  fireEffect();
  delay(1000);
}

void fireEffect()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    int flicker = random(0, 80); // Increase the range for more flicker
    int r = 255;
    int g = max(0, 50 - flicker); // Reduce green for a more red effect
    int b = 0;
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // More red and active fire effect
  }
  pixels.show();
  delay(20); // Reduce delay for more active effect
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
      pixels.setPixelColor(i, COLOR_WHITE); // Use defined color
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
    pixels.setPixelColor(i, COLOR_GREEN); // Use defined color
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
    pixels.setPixelColor(i, COLOR_RED); // Use defined color
  }
  pixels.show();
  delay(500);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off
  }
  pixels.show();
}

void breathEffect()
{
  for (int brightness = 0; brightness < 256; brightness++)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness)); // White color breathing effect
    }
    pixels.show();
    delay(20); // Slow down the breathing effect
  }
  for (int brightness = 255; brightness >= 0; brightness--)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness)); // White color breathing effect
    }
    pixels.show();
    delay(20); // Slow down the breathing effect
  }
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