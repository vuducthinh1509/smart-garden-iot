#ifndef __SOIL_MOISTURE_SENSOR_H__
#define __SOIL_MOISTURE_SENSOR_H__
#include <stdio.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "esp_err.h"
#include "hal/gpio_types.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

struct soil_reading {
    int percent;
    int status;
};

void soil_moisture_init(void);
void soil_moisture_set_characterize(adc_unit_t unit,adc_atten_t atten,adc_bits_width_t width );
void set_max_min_voltage(int max,int min);
void set_channel(adc1_channel_t channel);
uint32_t read_voltage_moisture(void);
struct soil_reading soil_moisture_read();

#endif
