#include "Motors.h"

void Motors::init() {
    motorDriver1.begin();
    motorDriver2.begin();
}

void Motors::update() {
    // Example: set motor speed
    motorDriver1.setPWM(1, 150); // Assuming the first argument is the motor number
    motorDriver2.setPWM(2, 150); // Assuming the first argument is the motor number
}
