/**
 * @file IRDistanceSensor.ino
 * @brief Reads and converts analog values from an IR distance sensor to distance in centimeters.
 * 
 * This program uses an IR sensor connected to an ESP32's analog pin to detect the distance
 * of objects. It converts the analog signal to distance based on a specific equation and
 * displays the result on the Serial Monitor.
 */

#include <Arduino.h>  // Include Arduino core library for ESP32

/**
 * @brief Pin connected to the IR distance sensor.
 */
#define IR_SENSOR_PIN 35  // A3/IO35 on FireBeetle ESP32

/**
 * @brief Baseline threshold for detecting objects.
 * 
 * Analog values below this threshold are considered as "no object detected."
 * Adjust based on environmental noise and sensor calibration.
 */
#define BASELINE_THRESHOLD 1300

/**
 * @brief Converts an analog value to distance in centimeters.
 * 
 * This function uses a voltage-to-distance conversion formula specific to the
 * GP2Y0A21YK0F IR sensor. If the analog value is below the baseline threshold,
 * it returns -1 to indicate "no object detected."
 * 
 * @param analogValue The analog reading from the IR sensor.
 * @return float The calculated distance in centimeters, or -1 if no object is detected.
 */
float analogToDistance(int analogValue) {
    // If analog value is below the threshold, consider "no object"
    if (analogValue <= BASELINE_THRESHOLD) return -1;

    float voltage = analogValue * (3.3 / 4095.0); ///< Convert analog value to voltage
    float distance = 27.86 / (voltage - 0.42);    ///< Convert voltage to distance
    return distance;
}

/**
 * @brief Arduino setup function.
 * 
 * Initializes serial communication for displaying sensor data.
 */
void setup() {
    Serial.begin(115200); ///< Start the Serial Monitor
    delay(1000);          ///< Wait for the serial monitor to initialize
}

/**
 * @brief Arduino loop function.
 * 
 * Reads the analog value from the IR sensor, converts it to distance,
 * and displays the results on the Serial Monitor. If no object is detected,
 * it prints "No object detected."
 */
void loop() {
    int analogValue = analogRead(IR_SENSOR_PIN); ///< Read analog value from IR sensor
    float distance = analogToDistance(analogValue); ///< Convert analog value to distance

    // Display results with "No object" message if distance is below threshold
    Serial.print("Analog Value: ");
    Serial.print(analogValue);
    Serial.print(" | Distance (cm): ");
    if (distance == -1) {
        Serial.println("No object detected");
    } else {
        Serial.println(distance);
    }

    delay(500);  ///< Delay between readings, adjust as needed
}
