/**
 * @file MLX90640Thermal.ino
 * @brief Reads thermal data from the Adafruit MLX90640 IR Thermal Camera.
 * 
 * This program captures thermal data using the MLX90640 IR thermal camera and displays
 * a symbolic representation of the temperature readings on the Serial Monitor.
 * The MLX90640 operates in Chess mode, with an 18-bit resolution and an 8 Hz refresh rate.
 */

#include <Adafruit_MLX90640.h>
#include <Wire.h>
#include <SPI.h>

/** 
 * @brief MLX90640 sensor object.
 */
Adafruit_MLX90640 mlx;

/** 
 * @brief I2C address for the MLX90640 sensor.
 */
#define MLX90640_I2C_ADDR 0x33  // Default I2C address for MLX90640

/** 
 * @brief Width and height of the MLX90640 sensor array.
 */
#define WIDTH 32  ///< Number of horizontal pixels
#define HEIGHT 24 ///< Number of vertical pixels

/**
 * @brief Frame buffer to store temperature data from the MLX90640.
 */
float mlx90640Frame[WIDTH * HEIGHT];

/**
 * @brief Arduino setup function.
 * 
 * Initializes the serial communication, I2C interface, and MLX90640 sensor. Configures
 * the sensor to operate in Chess mode with 18-bit resolution and an 8 Hz refresh rate.
 */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial connection
  }
  Serial.println("Starting MLX90640 setup...");

  // Initialize I2C communication
  Wire.begin(21, 22, 400000); ///< SDA: GPIO 21, SCL: GPIO 22, Clock speed: 400 kHz
  Serial.println("I2C initialized at 400kHz.");

  // Initialize the MLX90640 sensor
  if (!mlx.begin(MLX90640_I2C_ADDR, &Wire)) {
    Serial.println("Failed to find MLX90640. Check connections and address!");
    while (1); // Halt program
  }
  Serial.println("MLX90640 detected.");

  // Configure the sensor
  mlx.setMode(MLX90640_CHESS);           ///< Set Chess mode for reduced noise
  mlx.setResolution(MLX90640_ADC_18BIT); ///< Set highest ADC resolution (18-bit)
  mlx.setRefreshRate(MLX90640_8_HZ);     ///< Set refresh rate to 8 Hz
  Serial.println("MLX90640 configuration complete.");
}

/**
 * @brief Arduino loop function.
 * 
 * Reads a thermal frame from the MLX90640 sensor, processes the temperature data,
 * and prints a symbolic representation of the temperature readings to the Serial Monitor.
 */
void loop() {
  Serial.println("Reading thermal frame...");
  
  // Attempt to read the frame data
  int status = mlx.getFrame(mlx90640Frame);
  if (status != 0) {
    Serial.print("Failed to read from MLX90640. Error code: ");
    Serial.println(status);
    delay(500); ///< Wait before retrying in case of an error
    return;
  }

  // Print symbols based on temperature ranges
  Serial.println("Thermal Data:");
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int index = (y * WIDTH) + x;
      float temp = mlx90640Frame[index];

      // Assign symbols based on temperature ranges
      if (temp > 30) {
        Serial.print("@"); ///< Symbol for temperatures above 30°C
      } else if (temp > 20) {
        Serial.print("x"); ///< Symbol for temperatures between 20°C and 30°C
      } else if (temp > 10) {
        Serial.print("."); ///< Symbol for temperatures between 10°C and 20°C
      } else {
        Serial.print(" "); ///< Symbol for temperatures below 10°C
      }
    }
    Serial.println(); ///< New line for each row
  }

  Serial.println("-----------------------------");
  delay(125); ///< Delay to match the 8 Hz refresh rate (125 ms)
}
