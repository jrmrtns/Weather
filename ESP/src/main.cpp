#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Arduino.h>
#include "WiFiHandler.h"
#include "Sensor.h"
#include "WebServerHandler.h"
#include "OTAHandler.h"
#include <PID_v1.h>
#include <Fonts/FreeSans9pt7b.h>
#include "font.h"

const char *hostname = "weather";
const char *otapw = "weather";
const char *ssid = "";
const char *password = "";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 16

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {B00000000, B11000000,
     B00000001, B11000000,
     B00000001, B11000000,
     B00000011, B11100000,
     B11110011, B11100000,
     B11111110, B11111000,
     B01111110, B11111111,
     B00110011, B10011111,
     B00011111, B11111100,
     B00001101, B01110000,
     B00011011, B10100000,
     B00111111, B11100000,
     B00111111, B11110000,
     B01111100, B11110000,
     B01110000, B01110000,
     B00000000, B00110000};

Sensor sensor;
WebServerHandler server;
OTAHandler ota;
WiFiHandler wifi;
sensorData lastdata;
double output;
PID pid(&lastdata.temperature, &output, &lastdata.target, 2, 5, 1, DIRECT);
const byte heating_gpio = 32; // the PWM pin the heating is attached to

void setup()
{
  Serial.begin(9600);
  Wire.begin(4, 15);
  Wire.setClock(400000L);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();

  display.clearDisplay();

  display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      (display.height() - LOGO_HEIGHT) / 2,
      logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);

  delay(2000); // Pause for 2 seconds

  wifi.Initialize();
  ota.Initialize();
  //sensor.Initialize();
  server.Initialize();
  lastdata.target = 28.0;
  lastdata.temperature = 21.0;

  ledcAttachPin(heating_gpio, 0); // assign a pwm pin to a channel

  // https://techexplorations.com/guides/esp32/begin/pwm/
  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 4000, 8);

  pid.SetMode(AUTOMATIC);
}

void loop()
{
  ota.Handle();

  server.GetWebSocketHandler()->CleanupClients();

  //lastdata = sensor.Messure();
  server.GetWebSocketHandler()->SendAll(Sensor::ConvertToJson(lastdata));

  pid.Compute();
  ledcWrite(0, output);

  Serial.printf("Input: %f ", lastdata.temperature);
  Serial.printf("Target: %f ", lastdata.target);
  Serial.printf("Output: %f\r\n", output);

  display.clearDisplay();

  display.setFont();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 55);
  display.print("Soll: ");
  display.print(lastdata.target, 1);
  display.print((char)247);
  display.println("C");

  // http://oleddisplay.squix.ch/#/home
  display.setFont(&FreeSans9pt7b);
  display.setCursor(0, 40);
  display.print(lastdata.temperature, 1);
  display.print("*C");
  display.setCursor(100, 8);
  display.print((char)247);

  display.display();

  delay(5000);
}