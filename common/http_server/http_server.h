#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

typedef void (*http_get_callback_t) (void);
typedef void (*http_post_callback_t) (char* data, int len);

void start_webserver(void);
void stop_webserver(void);
void http_set_sw1_callback(void *cb);
void http_set_status_sw1_callback(void *cb);
void http_set_data_callback(void *cb);
void http_set_threshold_callback(void *cb);
void http_set_post_threshold_callback(void *cb);
void sw1_response(char* data, int len);
void data_response(char* data, int len);
void threshold_response(char* data, int len);


#endif
