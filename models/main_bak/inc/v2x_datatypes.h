#pragma once

#include "rtwtypes.h"

// #define HTTP_SOCKET
#define IP_ADDR "192.168.1.17";
#define PAYLOAD_VALUES 6
#define PAYLOAD_STR_MAX 100

struct payload_struct {
    char name[4];
    float lat;
    float lon;
    uint8_T speed;
    uint8_T dir;
    float dist_next_step;
};

struct char_strings {

    char name[20];
    char lat[20];
    char lon[20];
    char speed[20];
    char dir[20];
    char dist_next_step[20];
};
