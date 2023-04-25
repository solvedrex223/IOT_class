#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct mqtt_connack {
    int8_t fixed_header [2];
    int8_t variable_header [2];
};


