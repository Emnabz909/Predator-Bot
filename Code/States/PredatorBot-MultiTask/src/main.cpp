#include <Arduino.h>
#include "Mastermind.h"

Mastermind mastermind;

void setup() {
    Serial.begin(115200);
    mastermind.init();
}

void loop() {
    mastermind.run();
}
