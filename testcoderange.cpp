#include <stdio.h> //measure how fast my calculation ism
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "VL53L0X.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"

#define SERVO_PIN 15; // motor pin for pwm

#define SPI_PORT spi0
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

#define constrain(val, low, high) \ // can be used for multiple types of values
    ((val) < (low) ? (low) : ((val) > (high) ? (high) : (val)))

uint16_t map_dist_to_screen(float dist) {
    return static_cast<uint16_t>(((dist - 1.0) * (251.0 - 10.0)) / 5.0); 
}

void setup_servo() { // used google search
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // Set 50Hz PWM cycle (20ms) -> clkdiv / wrap 
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); 
    pwm_config_set_wrap(&config, 19999);
    
    pwm_init(slice_num, &config, true);
}

void set_servo_angle(uint slice_num, float angle) { // used google search
    // Convert 0-180 degrees to pulse widths (1ms to 2ms)
    // 1000 = 0 deg, 2000 = 180 deg
    uint pulse_width = 1000 + (angle / 180.0f) * 1000;
    pwm_set_gpio_level(SERVO_PIN, pulse_width);
}

int main() {
    stdio_init_all();
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(20, GPIO_FUNC_I2C);
    gpio_set_function(21, GPIO_FUNC_I2C);
    gpio_pull_up(20);
    gpio_pull_up(21);

    VL53L0X sensor(i2c0, 0x29);

    if (!sensor.init()) {
        printf("Sensor init failed!\n");
        while (1);
    }

    spi_init(SPI_PORT, 125*1000*1000);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    sensor.startContinuous();
    setup_servo();
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

    float previous_error = 0.0; 
    float integral = 0.0;
    float target = 2.5; // in inches
    float Kp = 0.5; // most sensititve factor, change this maybe statically 
    float Ki = Kp * 0.1;
    float Kd = Kp * 0.3;
    
    float DT = 0.025;
    float error = 0.0;
    float angle = 90.0;
    set_servo_angle(slice, angle);
    uint8_t tx[1];        
    uint8_t data = 0;

    gpio_init(14);
    gpio_pull_down(14);

    while(!gpio_get(14)){
        printf("Press the button.\n");
    }
    bool lastButtonState = true;

    while (1) {
        bool currentButtonState = gpio_get(14);
        if (lastButtonState && !currentButtonState) {
            if (target + 0.5f > 4.0f) {
                target = 1.5f;
            } else {
                target += 0.5f;
            }
        }
        lastButtonState = currentButtonState;
        uint16_t dist_int = sensor.readRangeContinuousMillimeters(); // stop reading correctly at around an inch
        float dist = (dist_int / 25.40f) - 1.63f; // change minus value for callibration, the higher the number the lower the number
        dist = constrain(dist, 1.0, 6.0);
        //printf("Distance: %.2f inches\n", dist);
        //printf("Target: %.2f inches\n", target);
        data = map_dist_to_screen(dist);
        gpio_put(PIN_CS, 0);
        tx[0] = 251 - data;
        spi_write_blocking(SPI_PORT, tx, 1);
        gpio_put(PIN_CS, 1);
        sleep_ms(25);
        error = target - dist;
        integral += error * DT;
        float derivative = (error - previous_error) / DT;
        float output = Kp*error + Kd*derivative + Ki*integral;
        output = constrain(output, -2.5f, 2.5f);
        //printf("output: %.2f \n", output);
        angle =  90 - output * 10.0;
      
        //printf("Kp: %.2f, Kd: %.2f, Ki: %.2f, Angle: %.2f \n", Kp, Kd, Ki, angle);
        previous_error = error;

        set_servo_angle(slice, angle);
        }      

        
    }   
