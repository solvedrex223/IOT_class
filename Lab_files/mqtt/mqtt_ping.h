#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>

struct mqtt_ping
{
  int8_t fixed_header [2];
};
