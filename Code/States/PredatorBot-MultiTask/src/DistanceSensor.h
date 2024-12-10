#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
public:
    void init();
    void update();

private:
    int sensorPin = A0; // Example analog pin
    int distance;
};

#endif
