/*
*/
#include "SdFat.h"

const int SD_CS = 10;

const int PIN_MS4525    = 0;        // MS4525
const int PIN_SSC100MD  = 1;        // SSCDRRN100MDAA5

int FREQ                = 500;      // interval sampling frequency in millis
const double MV_CONV    = 0.0049;   // convert from bits to volts for 10-bit ADC

// random variable inititalization
unsigned long LAST_ITER;

SdFat sd(1);
SdFile file;
char filename[16];


void setup() {
    Serial.begin(115200);

    pinMode(SD_CS, OUTPUT);

    // setup the SD card
    if (!sd.begin(SD_CS, SD_SCK_MHZ(50))) {
        sd.initErrorHalt();
    }

    // find an open the next available file
    int fileNum = 0;
    char *s = "DATA_";

    snprintf(filename, sizeof(filename), "%s%03f.csv", s, fileNum);

    while (sd.exists(filename)) {
        fileNum++;

        Serial.print("Trying file "); Serial.println(filename);

        snprintf(filename, sizeof(filename), "%s%03f.csv", s, fileNum);
    } 
    
    
    LAST_ITER = millis();
}


void loop() {
    if ((millis() - LAST_ITER) > FREQ) {
        LAST_ITER = millis();

        // sample
        int MEAS1 = analogRead(PIN_MS4525);
        int MEAS2 = analogRead(PIN_SSC100MD);

        // convert to volts
        double VOLTS1 = MEAS1 * MV_CONV;
        double VOLTS2 = MEAS2 * MV_CONV;

        // convert to pressure in cm H2O
        double P1 = SSC100MD(VOLTS1);
        double P2 = MS4525(VOLTS2);

        // Output
        Serial.print("Honeywell SSC100MD = ");
        Serial.println(P2);

        Serial.print("TE Connectivity MS4525 = ");
        Serial.println(P1);

        Serial.println("");

        // Save to file
    }
}

double SSC100MD(double volts) {
    double PMIN = -101.9451;
    double PMAX = 101.9451;
    double VS = 5.0;

    double rv = (volts - 0.1*VS) * (PMAX - PMIN)/(0.8*VS) + PMIN;

    return rv;
}

double MS4525(double volts) {
    double PMIN = -70.307;  // cm H2O
    double PMAX = 70.307;   // cm H2O
    double VS = 5.0;        // voltage supply in volts

    double rv = (volts - 0.5) * (PMAX - PMIN)/(0.8*VS) + PMIN;
 
    return rv;
}