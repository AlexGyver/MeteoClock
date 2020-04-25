#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "GyverButton.h"
#include <GyverTimer.h>
#include <MHZ19_uart.h>

#define RESET_CLOCK 0       // Reset clock, do it only once on PC plug
#define SENS_TIME 30000     // Sensors update time
#define LED_MODE 0          // LED pin type
#define AUTO_BRIGHT 0

// LED & LCD bright limits
#define BRIGHT_THRESHOLD 150
#define LED_BRIGHT_MAX 255
#define LED_BRIGHT_MIN 10
#define LCD_BRIGHT_MAX 255
#define LCD_BRIGHT_MIN 10

// Plot limits
#define TMP_MIN 15
#define TMP_MAX 35
#define TMP_STP 5
#define HUM_MIN 0
#define HUM_MAX 100
#define HUM_STP 10
#define PRS_MIN -100
#define PRS_MAX 100
#define PRS_STP 10
#define CO2_MIN 300
#define CO2_MAX 2000
#define CO2_STP 100

// PINS
#define BACKLIGHT_PIN 10

// Addition light sensor
#define BL_PIN 10     // display bright pin
#define PHOTO_PIN 0   // bright sensor pin
#define PHOTO A3

#define MHZ_RX_PIN 2
#define MHZ_TX_PIN 3

#define LED_COM_PIN 7
#define LED_R_PIN 9
#define LED_G_PIN 6
#define LED_B_PIN 5
#define BTN_PIN 4

// Init lcd on 0x27 (or 0x3F)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Init real time module
RTC_DS3231 rtc;
DateTime now;
int8_t secs, mins, hrs;

// Init temperature & humidity & pressure sensor
Adafruit_BME280 bme;

// Init CO2 sensor
MHZ19_uart mhz19;

// Init timers and callbacks
GTimer_ms sensorsTimer(SENS_TIME);
GTimer_ms drawSensorsTimer(SENS_TIME);
GTimer_ms clockTimer(5000);
GTimer_ms hourPlotTimer((long)4 * 60 * 1000);
GTimer_ms dayPlotTimer((long)1.6 * 60 * 60 * 1000);
GTimer_ms plotTimer(240000);
GTimer_ms brightTimer(5000);

// Init single button
GButton button(BTN_PIN, LOW_PULL, NORM_OPEN);

/*
  0 Home screen with time and all sensors
  1 Temperature plot (hours)
  2 Temperature plot (day)
  3 Humidity plot (hours)
  4 Humidity plot (day)
  5 Pressure plot (hours)
  6 Pressure plot (day)
  7 CO2 plot (hours)
  8 CO2 plot (day)
*/
byte display_mode = 0;

float current_tmp;
byte current_hum;
int current_prs;
int current_co2;
static const char *dayNames[] = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};

// Plots
int tmpHour[15], tmpDay[15];
int humHour[15], humDay[15];
int prsHour[15], prsDay[15];
int co2Hour[15], co2Day[15];

#if (LED_MODE == 0)
byte LED_ON = (LED_BRIGHT_MAX);
#else
byte LED_ON = (255 - LED_BRIGHT_MAX);
#endif

void setLED(byte color);
void updateTime();
void updateBrightness();
void readSensors();
void updateAvgSensorsData();
void readInput();
void drawSensors();
void updatePlot();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting"));

  // LCD
  pinMode(BACKLIGHT_PIN, OUTPUT);
  analogWrite(BACKLIGHT_PIN, LCD_BRIGHT_MAX);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.println(F("LCD init - Done"));

  // LED
  pinMode(LED_COM_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  setLED(0);
  digitalWrite(LED_COM_PIN, LED_MODE);
  Serial.println(F("LED init - Done"));

  // CO2
  mhz19.begin(MHZ_TX_PIN, MHZ_RX_PIN);
  mhz19.setAutoCalibration(false);
  Serial.println(F("CO2 init - Done"));

  // Temperature & Humidity & Pressure
  bme.begin(&Wire);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);
  bme.takeForcedMeasurement();
  Serial.println(F("BME init - Done"));

  // Time
  rtc.begin();
  if (RESET_CLOCK || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(F("Time reset - Done"));
  }
  updateTime();
  Serial.println(F("Real time init - Done"));
  
  Serial.println(F("All set - Done"));
}

void loop() {
  if (brightTimer.isReady())
    updateBrightness();

  if (sensorsTimer.isReady())
    readSensors();

  if (clockTimer.isReady())
    updateTime();
  
  updateAvgSensorsData();
  readInput();
  
  if (display_mode == 0) { // Home
    if (drawSensorsTimer.isReady())
      drawSensors();
  } else { // Plot
    if (plotTimer.isReady())
      updatePlot();
  }
}
