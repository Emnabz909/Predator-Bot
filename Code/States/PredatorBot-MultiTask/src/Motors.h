#ifndef MOTORS_H
#define MOTORS_H

#include <Adafruit_MotorShield.h>

class Motors {
public:
    void init();
    void update();

private:
    Adafruit_MotorShield motorDriver1, motorDriver2;
};

#endif
