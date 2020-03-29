/*
Unit tests for the Arduino 
*/

#include "Arduino.h"

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.print(millis());
    Serial.print(",");
    Serial.println(random(0, 10));

    delay(100);
}