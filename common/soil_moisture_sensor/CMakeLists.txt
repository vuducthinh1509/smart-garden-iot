set(app_src soil_moisture_sensor.c)
set(pri_req driver hal esp_adc)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})

