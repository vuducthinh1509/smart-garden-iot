#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>
#include "http_server.h"

static const char *TAG = "HTTP_SERVER";

static http_post_callback_t http_post_sw1_callback = NULL;

static http_post_callback_t http_post_threshold_callback = NULL;

static http_get_callback_t http_get_status_sw1_callback = NULL;

static http_get_callback_t http_get_threshold_callback = NULL;

static http_get_callback_t http_get_data_callback = NULL;

static httpd_req_t *req_sw1;

static httpd_req_t *req_data;

static httpd_req_t *req_threshold;

static httpd_handle_t server = NULL;

extern const uint8_t index_html_start[] asm("_binary_webserver_html_start");
extern const uint8_t index_html_end[] asm("_binary_webserver_html_end");



/* An HTTP GET handler */
static esp_err_t get_dashboard_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start );

    return ESP_OK;
}

static const httpd_uri_t get_dashboard = {
    .uri       = "/dashboard",
    .method    = HTTP_GET,
    .handler   = get_dashboard_handler,
    .user_ctx  = NULL,
};

// get data
void data_response(char* data, int len){
    httpd_resp_send(req_data, data, len);
}

static esp_err_t get_data_handler(httpd_req_t *req)
{
    req_data = req;
    http_get_data_callback();

    return ESP_OK;
}

static const httpd_uri_t get_data = {
    .uri       = "/data",
    .method    = HTTP_GET,
    .handler   = get_data_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL,
};

// get status sw1
static esp_err_t get_status_sw1_handler(httpd_req_t *req)
{
    req_sw1 = req;
    http_get_status_sw1_callback();
    return ESP_OK;
}

void sw1_response(char* data, int len){
    httpd_resp_send(req_sw1, data, len);
}


static const httpd_uri_t get_status_sw1 = {
    .uri       = "/status",
    .method    = HTTP_GET,
    .handler   = get_status_sw1_handler,
    .user_ctx  = NULL,
};

// get threshold
static esp_err_t get_threshold_handler(httpd_req_t *req)
{
    req_threshold = req;
    http_get_threshold_callback();
    return ESP_OK;
}

void threshold_response(char* data, int len){
    httpd_resp_send(req_threshold, data, len);
}


static const httpd_uri_t get_threshold = {
    .uri       = "/threshold",
    .method    = HTTP_GET,
    .handler   = get_threshold_handler,
    .user_ctx  = NULL,
};

// set threshold
static esp_err_t threshold_post_handler(httpd_req_t *req)
{
    char buf[100];
    httpd_req_recv(req, buf,req->content_len);
    http_post_threshold_callback(buf,req->content_len);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_data_threshold = {
    .uri       = "/setthreshold",
    .method    = HTTP_POST,
    .handler   = threshold_post_handler,
    .user_ctx  = NULL
};


// post data bat tat sw1
static esp_err_t sw1_post_handler(httpd_req_t *req)
{
    char buf[100];
    httpd_req_recv(req, buf,req->content_len);
    http_post_sw1_callback(buf,req->content_len);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_data_sw1 = {
    .uri       = "/sw1",
    .method    = HTTP_POST,
    .handler   = sw1_post_handler,
    .user_ctx  = NULL
};

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_dashboard);
        httpd_register_uri_handler(server, &post_data_sw1);
        httpd_register_uri_handler(server, &post_data_threshold);
        httpd_register_uri_handler(server, &get_threshold);
        httpd_register_uri_handler(server, &get_status_sw1);
        httpd_register_uri_handler(server, &get_data);
    } else {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void)
{
    httpd_stop(server);
}

void http_set_sw1_callback(void *cb){
    if(cb){
        http_post_sw1_callback = cb;
    }
}

void http_set_post_threshold_callback(void *cb){
    if(cb){
        http_post_threshold_callback = cb;
    }
}

void http_set_status_sw1_callback(void *cb){
    if(cb){
        http_get_status_sw1_callback = cb;
    }
}


void http_set_data_callback(void *cb){
    if(cb){
        http_get_data_callback = cb;
    }
}

void http_set_threshold_callback(void *cb){
    if(cb){
        http_get_threshold_callback = cb;
    }
}