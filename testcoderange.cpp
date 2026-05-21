#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "VL53L0X.h"
#include "hardware/pwm.h"

// Define the pin you connected the servo signal wire to
const uint SERVO_PIN = 15;

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

    while (1) {
        uint16_t dist_int = sensor.readRangeContinuousMillimeters();
        float dist = (dist_int / 25.4f) - 2.0f;
        printf("Distance: %.2f inches\n", dist);
        sleep_ms(50);
        if(dist < 2.0){
            set_servo_angle(slice, -30);
            sleep_ms(100);
        }
        else if (dist < 3.0) {
            set_servo_angle(slice, 60);
            sleep_ms(100);
        }
        else {
            set_servo_angle(slice, 150);
            sleep_ms(100);
        }
    }
}
