#ifndef V2X_SDR_UIO_H
#define V2X_SDR_UIO_H

#include <stdint.h>

// Defines
#define UIO_ERROR   -1
#define UIO_SUCCESS  0
#define MMAP_OFFSET  0
#define UIO_MMAP_SIZE 0x1000 // size of memory to allocate (rx_IP is 64K but don't need all of that)
#define OPEN_ERROR -1        // open() returns -1 on error
#define RX_IP_DEV "/dev/uio0"

// Enums
enum RX_IP_REGS
{
    RX_IP_RDY       = 0x10,
    RX_IP_RDY_VALID = 0x14,
    RX_IP_VER       = 0x20,
    RX_IP_VER_VALID = 0x24,
    RX_IP_OUTPUT    = 0x100 // (64 * 32b) 0x100-0x1ff
};

// Function prototypes
int32_t generic_init(char devDevice[]);
void generic_write(uint32_t offset, uint32_t value);
uint32_t generic_read(uint32_t offset);
void generic_exit();

#endif // V2X_SDR_UIO_H
