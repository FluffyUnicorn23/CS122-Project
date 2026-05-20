#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "VL53L0X.h"

int main() {
    stdio_init_all();

    // I2C init (example for Pico 2W - adjust pins if needed)
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    VL53L0X sensor(i2c0, 0x29);

    if (!sensor.init()) {
        printf("Sensor init failed!\n");
        while (1);
    }

    sensor.startContinuous();

    while (1) {
        uint16_t dist = sensor.readRangeContinuousMillimeters();
        printf("Distance: %.2f inches\n", ((float)dist / 25.4f) - 2.0f);
        sleep_ms(100);
    }
}