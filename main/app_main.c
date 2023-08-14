/*
    Bài tập lớn: Xây dựng hệ thống tưới tiêu tự động cho vườn cây tiêu
    Được thực hiện bởi các thành viên nhóm 11 trong môn học Vi xử lí do thầy Hàn Huy Dũng hướng dẫn
    Các thành viên bao gồm: 
    Vũ Đức Thịnh 20200617, 
    Trần Ngọc Minh 20203513
    Ngô Vũ Trường Sơn 20203560, 
    Đặng Hữu Công Hiếu 20203417
    
    -----------------------------------------------------------------------------

    Final Project: Building an Automatic Irrigation System for Pepper Plantation

    This project was carried out by members of group 11 in the course of Microprocessors, guided by Professor Han Huy Dung.

    The team members are:

    Vu Duc Thinh - Student ID: 20200617
    Tran Ngoc Minh - Student ID: 20203513
    Ngo Vu Truong Son - Student ID: 20203560
    Dang Huu Cong Hieu - Student ID: 20203417

    -----------------------------------------------------------------------------
    
    version espressif v5.0.1
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/adc.h"
#include <inttypes.h>
#include <time.h>

#include "http_server.h"
#include "dht11.h"
#include "wifi_station.h"
#include "output_iot.h"
#include "esp_adc_cal.h"
#include "soil_moisture_sensor.h"

// nếu có 3 lỗi liên tiếp thì thông báo trạng thái của cảm biến là lỗi
#define ERRORS 3

// giá trị ngưỡng mặc định
static int tempThreshold = 32;
static int humThreshold = 70;
static int soilThreshold = 60;
// chế độ tự động tưới (bật = 1; tắt = 0)
static int autoPump = 1;

struct post_data {
    int temperature;
    int humidity;
    int soil;
    int status1;
    int status2;
};


static struct post_data data;

static struct dht11_reading dht_curr_data;

static struct soil_reading soil_curr_data;

static const char *TAG = "MAIN"; 

void read_data(void *pvParameters)
{
    int countError = 0;
    int countErrorSoilSensor = 0;
    for (;;)
    {
        dht_curr_data = DHT11_read();
        soil_curr_data = soil_moisture_read();
        if (dht_curr_data.status != 0 && countError < ERRORS)
        {
            countError++;
        }
        else if (countError >= ERRORS)
        {
            data.status1 = -1;
        }

        if (dht_curr_data.status == 0)
        {
            countError = 0;
            data.temperature = dht_curr_data.temperature;
            data.humidity = dht_curr_data.humidity;
            data.status1 = dht_curr_data.status;
        }

        if(soil_curr_data.status == -1 && countErrorSoilSensor<ERRORS){
            countErrorSoilSensor++;
        } else if(countErrorSoilSensor >= ERRORS){
            data.status2 = -1;
        }
        if(soil_curr_data.status == 0 ){
            data.status2 = soil_curr_data.status;
            data.soil = soil_curr_data.percent;
            countErrorSoilSensor = 0;
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void auto_on_off_relay(void *pvParameters)
{
    int count = 0;
    for (;;)
    {
        int curr_level = output_io_get_level(GPIO_NUM_18);
        if(autoPump == 1){
            if(data.status1 == 0 && data.status2 == 0){
            if(data.temperature >= tempThreshold && data.humidity < humThreshold){
                if(++count == 3){
                    if(curr_level == 0){
                        output_io_set_level(GPIO_NUM_18,1);
                        output_io_set_level(2, 1);
                    }
                    count=0;
                }
            } else if(data.temperature >= tempThreshold && data.humidity >= humThreshold && data.soil < soilThreshold){
                if(++count == 3){
                    if(curr_level == 0){
                        output_io_set_level(GPIO_NUM_18,1);
                        output_io_set_level(2, 1);
                    }
                    count=0;
                }
            } else if(data.temperature >= tempThreshold && data.humidity >= humThreshold && data.soil >= soilThreshold){
                if(++count == 3){
                    if(curr_level == 1){
                        output_io_set_level(GPIO_NUM_18,0);
                        output_io_set_level(2, 0);
                    }
                    count=0;
                }
            } else if(data.temperature < tempThreshold && data.humidity < humThreshold){
                if(++count == 3){
                    if(curr_level == 0){
                        output_io_set_level(GPIO_NUM_18,1);
                        output_io_set_level(2, 1);
                    }
                    count=0;
                }
            }
            else if(data.temperature < tempThreshold && data.humidity >= humThreshold && data.soil < soilThreshold){
                if(++count == 3){
                    if(curr_level == 0){
                        output_io_set_level(GPIO_NUM_18,1);
                        output_io_set_level(2, 1);
                    }
                    count=0;
                }
            }
            else if(data.temperature < tempThreshold && data.humidity >= humThreshold && data.soil >= soilThreshold){
                if(++count == 3){
                    if(curr_level == 1){
                        output_io_set_level(GPIO_NUM_18,0);
                        output_io_set_level(2, 0);
                    }
                    count=0;
                }
            } else {
                count = 0;
            }
        } else {
            output_io_set_level(GPIO_NUM_18,0);
            output_io_set_level(2, 0);
        }
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void data_callback(void)
{
    char resp[200];
    sprintf(resp, "{\"temperature\": %d,\"humidity\":%d,\"soil\":%d,\"status1\":%d,\"status2\":%d}", data.temperature, data.humidity, data.soil,data.status1, data.status2);
    data_response(resp, strlen(resp));
}

void sw1_status_data_callback(void)
{
    char resp[100];
    int level = output_io_get_level(GPIO_NUM_18);
    sprintf(resp, "{\"status\": %d,\"autopump\":%d}", level,autoPump);
    sw1_response(resp, strlen(resp));
}

void get_threshold_callback(void)
{
    char resp[100];
    sprintf(resp, "{\"temperature\": %d,\"humidity\":%d,\"soil\":%d}", tempThreshold,humThreshold,soilThreshold);
    threshold_response(resp, strlen(resp));
}

void post_threshold_callback(char *data, int len)
{
    if(data){
        int value1, value2, value3;
        sscanf(data, "%d&%d&%d&", &value1, &value2, &value3);
        tempThreshold = value1;
        humThreshold = value2;
        soilThreshold = value3;
    }
}

void sw1_data_callback(char *data, int len)
{
    if (data[0] == '1' && data[1] == '0')
    {
        autoPump = 0;
        output_io_set_level(GPIO_NUM_18, 1);
        output_io_set_level(2, 1);
    }
    else if (data[0] == '0' && data[1] == '0')
    {
        autoPump = 0;
        output_io_set_level(GPIO_NUM_18, 0);
        output_io_set_level(2, 0);
    } else if (data[0] == '1' && data[1] == '1')
    {
        autoPump = 1;
        output_io_set_level(GPIO_NUM_18, 1);
        output_io_set_level(2, 1);
    } else if (data[0] == '0' && data[1] == '1')
    {
        autoPump = 1;
        output_io_set_level(GPIO_NUM_18, 0);
        output_io_set_level(2, 0);
    }
}

void app_main(void)
{
    // Khởi tạo NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    // connect wifi
    wifi_init_station();
    // // start webserver
    start_webserver();
    // // khởi tạo dht11
    DHT11_init(GPIO_NUM_4);
    // // khởi tạo soil moisture
    set_channel(ADC1_CHANNEL_6);
    soil_moisture_init();
    //khởi tạo relay
    output_io_create(GPIO_NUM_2);
    output_io_set_level(GPIO_NUM_2, 0);
    output_io_create(GPIO_NUM_18);
    output_io_set_level(GPIO_NUM_18, 0);

    http_set_sw1_callback(sw1_data_callback);
    http_set_data_callback(data_callback);
    http_set_status_sw1_callback(sw1_status_data_callback);
    http_set_threshold_callback(get_threshold_callback);
    http_set_post_threshold_callback(post_threshold_callback);
    xTaskCreate(read_data, "Read Data", 2048, NULL, 4, NULL);
    xTaskCreate(auto_on_off_relay, "On/Off relay", 2048, NULL, 4, NULL);
}