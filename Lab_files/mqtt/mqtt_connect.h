#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct mqtt_connect{
    int8_t fixed_header [2];
    int8_t variable_header [8];
    int8_t keep_alive [2];
    int8_t client_identifier [23];
    char* will_message,user_name,password;
};