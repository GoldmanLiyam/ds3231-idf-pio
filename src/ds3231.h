#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"
#include <time.h>

typedef struct {
    i2c_master_bus_handle_t bus;
    i2c_master_dev_handle_t dev;
    uint8_t i2c_addr;
} ds3231_t;

/**
 * @brief Initialize DS3231 on given I2C bus
 * @param rtc Pointer to DS3231 handle
 * @param bus Existing I2C bus handle
 * @param addr I2C address of DS3231 (usually 0x68)
 */
esp_err_t ds3231_init(ds3231_t *rtc, i2c_master_bus_handle_t bus, uint8_t addr);

/**
 * @brief Set time on DS3231 (UTC)
 */
esp_err_t ds3231_set_time(ds3231_t *rtc, const struct tm *time);

/**
 * @brief Get time from DS3231 (UTC)
 */
esp_err_t ds3231_get_time(ds3231_t *rtc, struct tm *time);
