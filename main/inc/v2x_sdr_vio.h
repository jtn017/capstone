#ifndef V2X_SDR_VIO_H
#define V2X_SDR_VIO_H

#include <stdint.h>

// Defines
#define VIO_ERROR   -1
#define VIO_SUCCESS  0
#define VIO_MMAP_SIZE 0x1000 // size of memory to allocate (rx_IP is 64K but don't need all of that)
#define RX_CTL_DEV "/dev/uio1"

// Enums
enum RX_CTL_REGS
{
    RX_CTL_START_AGC   = 0x10, // Bit 0 = start/restart, Bit 1 = AGC_BYPASS
    RX_CTL_CORR_THRESH = 0x18, // 32 bits
    RX_CTL_AGC_POW_REF = 0x20, // 26 bits
    RX_CTL_STORE_DELAY = 0x28, // 31 bits
    RX_CTL_TIME_SEL    = 0x30  // 3 bits
};

// Function prototypes
int32_t generic_vio_init(char devDevice[]);
void generic_vio_write(uint32_t offset, uint32_t value);
uint32_t generic_vio_read(uint32_t offset);
void generic_vio_exit();

#endif // V2X_SDR_VIO_H
