#ifndef __TEST_H_
#define __TEST_H_

#include "esp_event.h"

#define WIFI_SSID "your_username"
#define WIFI_PASSWORD "your_password"
#define MAXIMUM_ENTRY  5

void event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
void wifi_init_station(void);

#endif