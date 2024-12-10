/**
 * @file PIDMotorControl.ino
 * @brief Implements PID motor control using an encoder for feedback.
 * 
 * This program controls a motor using PID to maintain a target speed.
 * It uses encoder feedback to calculate the motor speed and adjusts
 * the motor PWM signal accordingly.
 */

#include <Arduino.h>

// Motor and Encoder Pins
#define MOTOR_IN1 25   ///< Motor input pin 1 connected to GPIO 25
#define MOTOR_IN2 26   ///< Motor input pin 2 connected to GPIO 26
#define ENCODER_A 27   ///< Encoder A pin connected to GPIO 27
#define ENCODER_B 9    ///< Encoder B pin connected to GPIO 9

// PID Constants
const float Kp = 0.5;  ///< Proportional gain
const float Ki = 0.1;  ///< Integral gain
const float Kd = 0.01; ///< Derivative gain

// Global variables
volatile int encoderCount = 0; ///< Tracks the encoder counts
int targetSpeed = 50;          ///< Desired speed in encoder counts per second
int motorSpeed = 0;            ///< PWM duty cycle for motor
int prevEncoderCount = 0;      ///< Previous encoder count for speed calculation
unsigned long prevTime = 0;    ///< Previous time for speed calculation
float integral = 0;            ///< Accumulated integral error
float prevError = 0;           ///< Previous error for derivative calculation

/**
 * @brief Interrupt Service Routine for the encoder.
 * 
 * This ISR updates the encoder count based on the direction of rotation.
 */
void IRAM_ATTR encoderISR() {
  int stateA = digitalRead(ENCODER_A);
  int stateB = digitalRead(ENCODER_B);
  if (stateA == stateB) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

/**
 * @brief Arduino setup function.
 * 
 * Configures motor and encoder pins, initializes serial communication,
 * and attaches interrupts for the encoder.
 */
void setup() {
  // Configure motor control pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // Configure encoder pins
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);

  // Attach interrupts for the encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderISR, CHANGE);

  // Start Serial Monitor
  Serial.begin(115200);
}

/**
 * @brief Arduino loop function.
 * 
 * Calculates motor speed using encoder feedback, applies PID control,
 * and adjusts the motor's PWM duty cycle to maintain the target speed.
 */
void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - prevTime;

  // Calculate speed in encoder counts per second
  int encoderDelta = encoderCount - prevEncoderCount;
  float speed = (encoderDelta / (float)elapsedTime) * 1000.0; // counts per second
  prevEncoderCount = encoderCount;
  prevTime = currentTime;

  // PID control
  float error = targetSpeed - speed;
  integral += error * (elapsedTime / 1000.0);
  float derivative = (error - prevError) / (elapsedTime / 1000.0);
  motorSpeed += Kp * error + Ki * integral + Kd * derivative;
  prevError = error;

  // Constrain motor speed to valid PWM range (0-255)
  motorSpeed = constrain(motorSpeed, 0, 255);

  // Drive the motor forward: IN1 gets PWM signal, IN2 is LOW
  analogWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, motorSpeed);

  // Print debug information
  Serial.print("Speed: ");
  Serial.print(speed);
  Serial.print(" | Target Speed: ");
  Serial.print(targetSpeed);
  Serial.print(" | Motor PWM: ");
  Serial.println(motorSpeed);

  delay(100); // Loop delay for stability
}
