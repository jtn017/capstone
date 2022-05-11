#ifndef V2X_RX_BASE_WRAPPER_H
#define V2X_RX_BASE_WRAPPER_H

// Includes
#include "rtwtypes.h"
#include "v2x_datatypes.h"

// Include socket/http headers
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <curl/curl.h>

// Function declarations
void rx_bb_init(void);
void get_rx_bb_out(uint32_t* uio_pkt, uint8_T* output_frame);
int parse_payload_packet(uint8_T* in_frame, struct payload_struct * pyld);
void send_packet_esp8266(char * str);
void tx_payload_wifimodule2(struct payload_struct * pyld);
float fix_endianness(float val);


#endif // V2X_RX_BASE_WRAPPER_H


