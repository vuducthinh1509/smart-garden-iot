set(app_src http_server.c)
set(pri_req esp_wifi esp_eth esp-tls esp_http_server)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})

