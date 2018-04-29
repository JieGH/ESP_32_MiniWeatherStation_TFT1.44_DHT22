// This code reveived huge help from www.xtronical.com and Adafruit.

#include <Adafruit_GFX.h>     // Core graphics library https://github.com/adafruit/Adafruit-GFX-Library
#include <XTronical_ST7735.h> // Hardware-specific library http://www.xtronical.com/basics/displays/lcd-tft-colourcolor-display-128x128-pixelst7735-driver/displaying-embedded-colour-graphics/
#include <SPI.h>
#include "DHT.h"

// set up pins we are going to use to talk to the screen
#define TFT_DC 2  // register select (stands for Data Control perhaps!)
#define TFT_RST 4 // Display reset pin, you can also connect this to the ESP32 reset
// in which case, set this #define pin to -1!
#define TFT_CS 5 // Display enable (Chip select), if not enabled will not talk on SPI bus

// initialise the routine to talk to this display with these pin connections (as we've missed off
// TFT_SCLK and TFT_MOSI the routine presumes we are using hardware SPI and internally uses 13 and 11
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void)
{
  tft.init(); // initialize a ST7735S chip,
}

void loop()
{

  #define DHTTYPE DHT22
  const int DHTPin = 17;
  // Initialize DHT sensor.
  DHT dht(DHTPin, DHTTYPE);
  static char celsiusTemp[7];
  static char fahrenheitTemp[7];
  static char humidityTemp[7];
  dht.begin();

  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // We start by connecting to a WiFi network
  Serial.print("Getting humidity and tempurature");
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp, "Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");
  }
  else
  {
    // Computes temperature values in Celsius + Fahrenheit and Humidity
    float hic = dht.computeHeatIndex(t, h, false);
    dtostrf(hic, 6, 2, celsiusTemp);
    float hif = dht.computeHeatIndex(f, h);
    dtostrf(hif, 6, 2, fahrenheitTemp);
    dtostrf(h, 6, 2, humidityTemp);
  }
  Serial.println("The Humd. and Temp. are:");
  Serial.println(h);
  Serial.println(t);

  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 5);
  tft.println("Temp:");
  tft.setCursor(65, 5);
  tft.println(t);
  tft.setCursor(5, 30);
  tft.println("Humd:");
  tft.setCursor(65, 30);
  tft.println(h);
  delay(1000);
}
