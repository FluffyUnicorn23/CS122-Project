#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "VL53L0X.h"
#include "hardware/pwm.h"

// Define the pin you connected the servo signal wire to
const uint SERVO_PIN = 15;

#define constrain(val, low, high) \
    ((val) < (low) ? (low) : ((val) > (high) ? (high) : (val)))

void setup_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // Set 50Hz PWM cycle (20ms) -> clkdiv / wrap 
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); 
    pwm_config_set_wrap(&config, 19999);
    
    pwm_init(slice_num, &config, true);
}

void set_servo_angle(uint slice_num, float angle) {
    // Convert 0-180 degrees to pulse widths (1ms to 2ms)
    // 1000 = 0 deg, 2000 = 180 deg
    uint pulse_width = 1000 + (angle / 180.0f) * 1000;
    pwm_set_gpio_level(SERVO_PIN, pulse_width);
}


int main() {
    stdio_init_all();
    // I2C init (example for Pico 2W - adjust pins if needed)
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(16, GPIO_FUNC_I2C);
    gpio_set_function(17, GPIO_FUNC_I2C);
    gpio_pull_up(16);
    gpio_pull_up(17);

    VL53L0X sensor(i2c0, 0x29);

    if (!sensor.init()) {
        printf("Sensor init failed!\n");
        while (1);
    }

    sensor.startContinuous();
    setup_servo();
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

    float previous_error = 0.0; 
    float integral = 0.0;
    float target = 3.0; // in inches
    float Kp = 0.5; // most sensititve factor, change this maybe statically 
    float Ki = 0;
    float Kd = 0;
    
    float DT = 0.025;
    float error = 0.0;
    float angle = 90.0;
    set_servo_angle(slice, angle);
    int count = 0;

    int c = getchar();
    if (c == 'c') {
            

    while (1) {
        uint16_t dist_int = sensor.readRangeContinuousMillimeters(); // stop reading correctly at around an inch
        float dist = (dist_int / 25.40f) - 1.63f; // change minus value for callibration, the higher the number the lower the number
        printf("Distance: %.2f inches\n", dist);
        sleep_ms(25);
        error = target - dist;
        integral += error * DT;
        float derivative = (error - previous_error) / DT;
        float output = Kp*error + Kd*derivative + Ki*integral;
        //output = constrain(output, -6.5f, 6.5f);
        printf("output: %.2f \n", output);
        angle =  90 - output * 10.0;
      
        printf("Kp: %.2f, Kd: %.2f, Ki: %.2f, Angle: %.2f \n", Kp, Kd, Ki, angle);
        previous_error = error;
        
        // if (abs(error) > 0.5 && Kp < 0.8) {
        //     Kp += 0.05;
        //     }
        //     else if (abs(error) < 0.2 && Kp > 0.3) {
        //         Kp -= 0.05;
        //     }
            Ki = Kp * 0.1;
            Kd = Kp * 0.3;
            // ++count;
            set_servo_angle(slice, angle);
        }      

        
     }   
}   
