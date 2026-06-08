# CS122A Custom Laboratory Project
PID Seesaw Balancer
Ricky Vo and Isaiah Tirado

# Introduction
A custom printed seesaw attempts to keep a ball as stationary as possible at the target through PID. The seesaw is tilted through a servo motor. Our PID constants are the following, Kp = 0.5, Kd = 0.15, Ki = 0.05, interval time = 25 ms. The default target distance to keep the ball stationary is set to 3 inches away from a time of flight sensor. The target distance can be incremented through a button that also starts balancing the button, with implemented value rollover. The distance of the ball to the sensor tracked through a graph on a screen powered by the FPGA. Overall, we found that constants stated above as the most consistent values that do not cause the ball to go into an unstable state. The only software issues within the project are the potential for the system to become unstable and visual bugs with the screen such as double printed information. The only hardware issue is that you have to hold down the seesaw because it’s tilted
Elements of Complexity
Accurate PID calculations that cause ball to balance at desired target
Custom designed graph 
Custom PLA 3D prints designed in OnShape, was first time using software






# User Guide
1. Reset/ Power MCU’s
2. Hold seesaw down
3. Place tennis ball on seesaw
4. Press Button
5. Let it do its magic, you can press button again to increase target distance

# Hardware Components Used
1. Icesugar/Icesugar-pro FPGA
2. RGB LCD from the required lab components
3. Raspberry Pi Pico
4. MG946R Something Servo Motor
5. VL53L0X time of flight sensor
6. Custom 3D printed PLA seesaw and complementary base

# Software Libraries Used
1. Modified VL53L0X library by yspreen
2. pico _std_lib
3. Pico I2C library
4. Pico SPI library
5. Pico PWM library
# Protocols Used
1. SPI: Used to send to information to FPGA to  display necessary data to RGB LCD, uses only MOSI, CS, and clock
2. I2C: Used for time of flight sensor, has ID of 0x29
# Requirements
Icesugar/Icesugar-pro FPGA: Used to power and control the information displayed on the RGB LCD screen.
RGB LCD: displays positional information of ball
Raspberry Pi Pico: controls servo motor, obtains positional information from time of flight sensor and sends information to FPGA
Event driven execution: has button that turns on balancing procedure and increments target distance if and only if pressed


# Wiring Diagram


# Design Diagram


# AI Usage
No AI was used in the making of this project.
# Acknowledgements
List any resources used whether you used code from it or if it was just for information
https://github.com/yspreen/VL53L0X-driver-pico-sdk-cpp/blob/main/VL53L0X.h.

