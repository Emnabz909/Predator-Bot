# Heat-Seeking Predator Bot

This project involved designing and building an autonomous robot capable of tracking and following a person based on their heat signature. The bot integrates various sensors, actuators, and a custom PCB for hardware interfacing, exemplifying a multidisciplinary approach to mechatronics.

## Table of Contents
1. [Introduction](#introduction)
2. [Specifications](#specifications)
3. [Design Development](#design-development)
   - [Hardware Design](#hardware-design)
   - [Software Design](#software-design)
4. [Results](#results)
5. [Lessons Learned](#lessons-learned)
6. [References](#references)

---

## Introduction
The Heat-Seeking Predator Bot was designed to:
- Detect and follow a person using a thermal camera and IR distance sensor.
- Maintain a proximity range using motor control.
- Demonstrate multitasking capabilities for real-time operation.

The bot was built using an ESP32 FireBeetle microcontroller, with custom PCB and housing, leveraging various sensors and actuators to create a functional prototype.

## Specifications
The bot's key specifications are:
- **Thermal Camera**: Adafruit MLX90640 (20–40°C, ±5°C accuracy).
- **IR Distance Sensor**: Sharp GP2Y0A21YK0F (2–36 inches, ±0.5-inch accuracy).
- **Inertial Measurement Unit (IMU)**: Adafruit BNO055 for motion control.
- **Motors**: N20 DC motors with magnetic encoders and DRV8871 motor drivers.

### Pass/Fail Criteria
- Tracks heat signatures.
- Maintains a 2–36-inch range from objects.
- Streams thermal heatmaps to a web interface.

---

## Design Development

### Hardware Design
The custom PCB was created using KiCAD and included:
- Screw terminals for component connectivity.
- Proper trace sizing for power and heat dissipation.
- Compact layout to minimize weight and improve balance.

The bot’s housing was designed to securely hold all components while minimizing weight-induced friction.

### Software Design
The software was modular, focusing on individual sensor functionality before integration:
- **Thermal Camera**: Detected heat signatures and converted data to ASCII for debugging.
- **IR Distance Sensor**: Measured distance using voltage-to-distance calibration.
- **IMU**: Provided acceleration data to refine motor control.
- **Motor Control**: Implemented PID for speed adjustments.

Due to time constraints, full system integration was not achieved.

---

## Results
While the bot's PCB design required significant revisions, the sensors functioned as expected when tested individually. Challenges included:
- Inadequate motor speed and torque.
- Integration of components due to missing PCB features.

Despite these setbacks, the project provided valuable hands-on experience.

---

## Lessons Learned
Key takeaways include:
1. **Early Planning**: A thorough design phase and literature review are essential.
2. **PCB Design**: Focus on component connectivity, power handling, and adequate debugging options.
3. **Prototyping**: Iterative testing is vital for identifying and resolving design flaws.

---

## References
- [Adafruit MLX90640 GitHub Repository](https://github.com/adafruit/Adafruit_MLX90640)
- [Adafruit BNO055 GitHub Repository](https://github.com/adafruit/Adafruit_BNO055)
- [Predator Bot Repository](https://github.com/Emnabz909/Predator-Bot)

---

## Acknowledgments
This project was completed as part of ME 507 at Cal Poly San Luis Obispo. Special thanks to the Mechatronics Department for providing resources and support.

---
