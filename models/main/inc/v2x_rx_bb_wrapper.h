#ifndef V2X_RX_BASE_WRAPPER_H
#define V2X_RX_BASE_WRAPPER_H

// Includes
#include "rtwtypes.h"
#include "v2x_datatypes.h"

// Function declarations
void rx_bb_init(void);
void get_rx_bb_out(uint8_T* output_frame, int frame_num);
// int parse_payload_packet(uint8_T* in_frame, struct payload_struct * pyld);
int fix_payload_packet(uint8_T* in_frame);
// void tx_payload_wifimodule(struct payload_struct * pyld, int * fd);
#if 0
int socket_connect(char *host, in_port_t port);
void tx_payload_wifimodule(struct payload_struct * pyld);
#endif
// void send_packet_esp8266(char * str);
// void tx_payload_wifimodule2(struct payload_struct * pyld);
float fix_endianness(float val);


#endif // V2X_RX_BASE_WRAPPER_H


