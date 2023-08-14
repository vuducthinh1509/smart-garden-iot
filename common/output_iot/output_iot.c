#include <stdio.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "output_iot.h"

void output_io_create(gpio_num_t gpio_num)
{
    esp_rom_gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT_OUTPUT);

}

void output_io_set_level(gpio_num_t gpio_num, int level)
{  
    gpio_set_level(gpio_num, level);
}

void output_io_toggle(gpio_num_t gpio_num)
{  
    int old_level = gpio_get_level(gpio_num);
    gpio_set_level(gpio_num, 1 - old_level);
}

int output_io_get_level(gpio_num_t gpio_num){
    return gpio_get_level(gpio_num);
}


