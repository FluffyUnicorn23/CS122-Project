# CS122A Custom Laboratory Project
## PID Seesaw Balancer

**Authors:** Ricky Vo and Isaiah Tirado

---

## Overview

A custom 3D-printed seesaw balances a ball at a configurable target position using a PID controller. The seesaw angle is adjusted by a servo motor while a VL53L0X Time-of-Flight sensor continuously measures the ball's position.

### PID Configuration

| Parameter | Value |
|-----------|--------|
| Kp | 0.50 |
| Ki | 0.05 |
| Kd | 0.15 |
| Update Interval | 25 ms |

The default target position is **3 inches** from the sensor. A push button starts the balancing process and increments the target distance with rollover behavior. Ball position is displayed graphically on an FPGA-driven LCD screen.

---

## Features

- PID-controlled ball balancing
- Adjustable target position
- Real-time position tracking
- FPGA-powered graphical display
- Custom-designed 3D printed mechanical system
- Event-driven operation through push-button input

---

## Project Complexity

### Control System
- Accurate PID calculations for stable ball balancing
- Real-time feedback control using Time-of-Flight sensing

### Embedded Systems
- Communication between Raspberry Pi Pico and FPGA
- SPI-based display updates
- I2C sensor integration
- PWM servo motor control

### Mechanical Design
- Custom PLA 3D-printed seesaw and base
- Designed using OnShape

---

## User Guide

1. Power on or reset the microcontrollers.
2. Hold the seesaw in place.
3. Place the tennis ball on the seesaw.
4. Press the button to start balancing.
5. Press the button again to increase the target distance.

---

## Hardware Components

| Component | Purpose |
|------------|---------|
| IceSugar / IceSugar Pro FPGA | LCD control and graphics |
| RGB LCD | Position visualization |
| Raspberry Pi Pico | Main controller |
| MG946R Servo Motor | Seesaw actuation |
| VL53L0X Time-of-Flight Sensor | Ball position sensing |
| Custom 3D Printed PLA Parts | Mechanical structure |

---

## Software Libraries

- Modified VL53L0X library by yspreen
- pico_stdlib
- Pico I2C library
- Pico SPI library
- Pico PWM library

---

## Communication Protocols

### SPI
Used to transmit display information from the Raspberry Pi Pico to the FPGA.

Signals used:
- MOSI
- CS
- Clock

### I2C
Used to communicate with the VL53L0X Time-of-Flight sensor.

- Device Address: `0x29`

---

## System Requirements

### FPGA
Responsible for driving the RGB LCD and rendering project information.

### RGB LCD
Displays ball position and system information.

### Raspberry Pi Pico
- Reads sensor data
- Executes PID calculations
- Controls the servo motor
- Sends display updates to the FPGA

### Event-Driven Operation
Balancing begins only after a button press. Subsequent button presses increment the target position.

---

## System Diagrams

### Wiring Diagram
*(Insert wiring diagram image from report here)*

### Design Diagram
*(Insert design diagram image from report here)*

---

## Known Issues

### Software
- System can occasionally become unstable depending on ball dynamics.
- LCD display may occasionally exhibit graphical artifacts or duplicated text.

### Hardware
- The seesaw must be manually held in position due to the mechanical design.

---

## AI Usage

- PWM code assistance obtained through AI-assisted Google Search.
- Constraint code assistance obtained through AI-assisted Google Search.

---

## Acknowledgements

VL53L0X Driver:
https://github.com/yspreen/VL53L0X-driver-pico-sdk-cpp

---

*CS122A Custom Laboratory Project — PID Seesaw Balancer*
