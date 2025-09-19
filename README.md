#DS3231 Library for ESP-IDF 5.x

Minimal DS3231 RTC library for ESP32 using the new I2C Master driver (i2c_master.h).
This library provides simple APIs to initialize the DS3231, set the current time, and read the current time.
It is designed for use in PlatformIO with ESP-IDF ≥ 5.0.

##Features

Written for ESP-IDF 5.x I2C Master API

Initialize DS3231 on an existing I2C bus

Set current time (struct tm)

Read current time (struct tm)

Simple, minimal, and easy to extend

Suitable for projects like data loggers, clocks, alarms, etc.

##Requirements

ESP32 board

ESP-IDF 5.x (works best via PlatformIO)

DS3231 RTC module connected via I2C

##Installation

Clone or copy this library into your lib/ folder in a PlatformIO project, or add it as a git dependency in your platformio.ini:

[env:esp32dev]
platform = espressif32
framework = espidf
board = esp32dev
lib_deps = 
    https://github.com/<your-user>/ds3231

##Wiring

Typical I2C wiring (ESP32 default):

SDA → GPIO21

SCL → GPIO22

VCC → 3.3V

GND → GND

(You can configure other pins in i2c_master_bus_config_t.)

##Usage Example
#include "ds3231.h"

ds3231_t rtc;

void app_main(void) {
    // Configure I2C bus
    i2c_master_bus_config_t bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = 21,
        .scl_io_num = 22,
    };
    i2c_master_bus_handle_t bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &bus));

    // Initialize DS3231
    ESP_ERROR_CHECK(ds3231_init(&rtc, bus));

    // Set time
    struct tm set_time = {
        .tm_year = 2025 - 1900,
        .tm_mon  = 8,
        .tm_mday = 19,
        .tm_hour = 14,
        .tm_min  = 30,
        .tm_sec  = 0,
    };
    ds3231_set_time(&rtc, &set_time);

    // Read time
    struct tm now;
    while (1) {
        ds3231_get_time(&rtc, &now);
        ESP_LOGI("MAIN", "Now: %04d-%02d-%02d %02d:%02d:%02d",
                 now.tm_year+1900, now.tm_mon+1, now.tm_mday,
                 now.tm_hour, now.tm_min, now.tm_sec);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

##Notes

Time is handled in UTC using standard struct tm.

DS3231 uses BCD format internally; conversion is handled by the library.

Weekday (tm_wday) is optional and starts from 0 = Sunday.
