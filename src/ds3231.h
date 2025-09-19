#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"
#include <time.h>

#define DS3231_ADDR 0x68

typedef struct {
    i2c_master_bus_handle_t bus;
    i2c_master_dev_handle_t dev;
} ds3231_t;

/**
 * @brief Initialize DS3231 on given I2C bus
 */
esp_err_t ds3231_init(ds3231_t *rtc, i2c_master_bus_handle_t bus);

/**
 * @brief Set time on DS3231 (UTC)
 */
esp_err_t ds3231_set_time(ds3231_t *rtc, const struct tm *time);

/**
 * @brief Get time from DS3231 (UTC)
 */
esp_err_t ds3231_get_time(ds3231_t *rtc, struct tm *time);
