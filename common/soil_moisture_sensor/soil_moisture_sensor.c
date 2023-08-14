#include "soil_moisture_sensor.h"

static const char *TAG = "SOIL_MOISTURE_SENSOR";
static esp_adc_cal_characteristics_t adc1_chars;
static adc_atten_t adc_atten = ADC_ATTEN_DB_11;
static adc_unit_t adc_unit = ADC_UNIT_1;
static adc_bits_width_t bit_width = ADC_WIDTH_BIT_DEFAULT;
static adc_channel_t adc_channel = ADC1_CHANNEL_6; // GPIO 34
static int max_voltage = 3129;
static int min_voltage = 1200;

static struct soil_reading last_read;

static uint32_t voltage;

void soil_moisture_init(){
    esp_adc_cal_characterize(adc_unit, adc_atten, bit_width, 0, &adc1_chars);
    ESP_ERROR_CHECK(adc1_config_width(bit_width));
    ESP_ERROR_CHECK(adc1_config_channel_atten(adc_channel, adc_atten));
}

void set_channel(adc1_channel_t channel){
    adc_channel = channel;
}

void soil_moisture_set_characterize(adc_unit_t unit,adc_atten_t atten,adc_bits_width_t width ){
    adc_unit = unit;
    adc_atten = atten;
    bit_width = width;
}

uint32_t read_voltage_moisture(void){
    voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(adc_channel), &adc1_chars);
    return voltage;
}

struct soil_reading soil_moisture_read(){
    read_voltage_moisture();
    if(voltage < 1200){
        last_read.percent = 0;
        last_read.status = -1;
    } else {
        last_read.percent = 100 * (min_voltage - (const int)voltage) / (max_voltage - min_voltage) + 100;
        last_read.status = 0;
    }
    return last_read;
}

void set_max_min_voltage(int max,int min){
    max_voltage = max;
    min_voltage = min;
}
