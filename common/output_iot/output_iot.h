#ifndef __OUTPUT_IO_H
#define __OUTPUT_IO_H
#include "esp_err.h"

void output_io_create(gpio_num_t gpio_num);
void output_io_set_level(gpio_num_t gpio_num, int level);
void output_io_toggle(gpio_num_t gpio_num);
int output_io_get_level(gpio_num_t gpio_num);
#endif
