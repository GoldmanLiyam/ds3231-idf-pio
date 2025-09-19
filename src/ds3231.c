#include "ds3231.h"
#include "esp_log.h"

static uint8_t bcd2dec(uint8_t val) { return (val >> 4) * 10 + (val & 0x0F); }
static uint8_t dec2bcd(uint8_t val) { return ((val / 10) << 4) | (val % 10); }

esp_err_t ds3231_init(ds3231_t *rtc, i2c_master_bus_handle_t bus, uint8_t addr) {
    if (!rtc || !bus) return ESP_ERR_INVALID_ARG;
    rtc->bus = bus;
    rtc->i2c_addr = addr;

    i2c_device_config_t dev_cfg = {
        .device_address = addr,
        .scl_speed_hz = 100000,
    };

    return i2c_master_bus_add_device(bus, &dev_cfg, &rtc->dev);
}

esp_err_t ds3231_set_time(ds3231_t *rtc, const struct tm *time) {
    if (!rtc || !time) return ESP_ERR_INVALID_ARG;
    uint8_t data[8];

    data[0] = 0x00; // start register
    data[1] = dec2bcd(time->tm_sec);
    data[2] = dec2bcd(time->tm_min);
    data[3] = dec2bcd(time->tm_hour);
    data[4] = dec2bcd(time->tm_wday + 1); // DS3231 uses 1=Sunday
    data[5] = dec2bcd(time->tm_mday);
    data[6] = dec2bcd(time->tm_mon + 1);
    data[7] = dec2bcd(time->tm_year - 100); // tm_year since 1900

    return i2c_master_transmit(rtc->dev, data, sizeof(data), -1);
}

esp_err_t ds3231_get_time(ds3231_t *rtc, struct tm *time) {
    if (!rtc || !time) return ESP_ERR_INVALID_ARG;

    uint8_t reg = 0x00;
    uint8_t data[7];

    ESP_ERROR_CHECK(i2c_master_transmit(rtc->dev, &reg, 1, -1));
    ESP_ERROR_CHECK(i2c_master_receive(rtc->dev, data, sizeof(data), -1));

    time->tm_sec  = bcd2dec(data[0] & 0x7F);
    time->tm_min  = bcd2dec(data[1]);
    time->tm_hour = bcd2dec(data[2] & 0x3F);
    time->tm_wday = bcd2dec(data[3]) - 1;
    time->tm_mday = bcd2dec(data[4]);
    time->tm_mon  = bcd2dec(data[5]) - 1;
    time->tm_year = bcd2dec(data[6]) + 100;

    return ESP_OK;
}
