/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/davidhagan/Documents/github/event/tests/e-vent-tests/pressure-testing/src/pressure-testing.ino"
/*
 * Project pressure-testing
 * Description:
 * Author:
 * Date:
 */


#include "SdFat.h"
#include "oled-wing-adafruit.h"
#include "Adafruit_ADS1X15.h"
#include "AdafruitDataLoggerRK.h"
#include "RTClib.h"
#include "SdCardDataLogger.h"

void startup();
void setup();
void loop();
#line 16 "/Users/davidhagan/Documents/github/event/tests/e-vent-tests/pressure-testing/src/pressure-testing.ino"
const int PIN_HW_1 = 0;
const int PIN_HW_2 = 1;
const int PIN_TE_1 = 2;
const int PIN_TE_2 = 3;

const double BITS_TO_MV = 0.1875F; //milli Volts per bit for ADS1115

// Init functionality on startup
void startup() {
  System.enableFeature(FEATURE_RETAINED_MEMORY);
  System.enableFeature(FEATURE_RESET_INFO);
}

STARTUP(startup());
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;
Adafruit_ADS1115 ads(0x49);  /* Use this for the 16-bit version */
RTCSynchronizer rtcSync;

unsigned long lastMeasurement = 0;
const unsigned long SAMPLE_PERIOD_MS = 1000;

static Logger appLog("app.event");

SerialLogHandler logHandler(LOG_LEVEL_ERROR, {
  {"app.event", LOG_LEVEL_INFO},
});

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);

  pinMode(D7, OUTPUT);

  // Put initialization like pinMode and begin functions here.
  rtcSync.setup();

  display.setup();
  display.clearDisplay();
  display.display();

  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit =       3mV       0.1875mV (DEFAULT)
  ads.begin();

  // setup the SD card shit


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  if (millis() - lastMeasurement >= SAMPLE_PERIOD_MS) {
    lastMeasurement = millis();

    time_t time = Time.now();

    appLog.info("loop");
    
    // read the pressure sensors
    short bits1 = ads.readADC_SingleEnded(PIN_HW_1);
    short bits2 = ads.readADC_SingleEnded(PIN_HW_2);
    short bits3 = ads.readADC_SingleEnded(PIN_TE_1);
    short bits4 = ads.readADC_SingleEnded(PIN_TE_2);

    double volts1 = bits1 * BITS_TO_MV;
    double volts2 = bits2 * BITS_TO_MV;
    double volts3 = bits3 * BITS_TO_MV;
    double volts4 = bits4 * BITS_TO_MV;

    Serial.println(volts1);
    Serial.println(volts2);
    Serial.println(volts3);
    Serial.println(volts4);
    Serial.println("");
    // log to the SD card

    // display
    display.clearDisplay();
    display.setTextSize(.8);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    display.println(Time.format(time, TIME_FORMAT_ISO8601_FULL));
    display.println(String::format("HW = %.1f, %.1f", volts1, volts2));
    display.println(String::format("TE = %.1f, %.1f", volts3, volts4));
    display.display();
  }

  display.loop();
  rtcSync.loop();
}