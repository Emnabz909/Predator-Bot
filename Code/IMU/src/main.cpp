/**
 * @file BNO055Accelerometer.ino
 * @brief Reads and displays acceleration data from the Adafruit BNO055 sensor.
 * 
 * This program uses the Adafruit BNO055 IMU sensor to capture acceleration
 * data and prints the values to the Serial Monitor. The sensor communicates
 * over I2C.
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/** 
 * @brief Delay in milliseconds between samples from the BNO055 sensor.
 */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

/** 
 * @brief Initialize the BNO055 sensor with ID and I2C address.
 * 
 * By default, the I2C address is either 0x28 or 0x29.
 * Make sure to verify the sensor's actual address in your setup.
 */
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

/**
 * @brief Prints acceleration data to the Serial Monitor.
 * 
 * @param event Pointer to a `sensors_event_t` structure containing acceleration data.
 */
void printEvent(sensors_event_t* event) {
  double x = event->acceleration.x; ///< Acceleration in the X-axis (m/s^2)
  double y = event->acceleration.y; ///< Acceleration in the Y-axis (m/s^2)
  double z = event->acceleration.z; ///< Acceleration in the Z-axis (m/s^2)

  Serial.print("Accl:\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}

/**
 * @brief Arduino setup function.
 * 
 * Initializes the serial communication and the BNO055 sensor.
 * Prints an error message if the sensor is not detected.
 */
void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);  // Wait for serial port to open

  Serial.println("Accelerometer Data Test"); 
  Serial.println("");

  // Initialize the BNO055 sensor
  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000); ///< Delay to allow the sensor to stabilize
}

/**
 * @brief Arduino loop function.
 * 
 * Captures acceleration data from the BNO055 sensor and prints it to the Serial Monitor
 * at intervals defined by `BNO055_SAMPLERATE_DELAY_MS`.
 */
void loop(void) {
  sensors_event_t accelerometerData; ///< Structure to store accelerometer data

  // Retrieve only acceleration data
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);

  // Print the acceleration data
  printEvent(&accelerometerData);

  // Delay between samples
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
