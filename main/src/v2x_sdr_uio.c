/* 
 * Example was from: 
 * https://github.com/byu-cpe/ecen427_student/blob/master/userspace/drivers/uio/generic_uio.c
 *
 */

// Standard libraries
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// Custom files
#include "v2x_sdr_uio.h"

// ---------------------- Globals ----------------------
static int f;     // this is a file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

// ---------------------- External functions ----------------------
// Initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t generic_init(char devDevice[])
{
    // Open the device
    f = open(devDevice, O_RDWR);
    if (f == OPEN_ERROR)
    {
        printf("uio example init error -- did you forget to sudo?\n");
        return UIO_ERROR;
    }

    // Memory map the physical address of the hardware into virtual address space
    ptr = (char*) (mmap(NULL, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, MMAP_OFFSET));
    if (ptr == MAP_FAILED)
    {
        return UIO_ERROR;
    }

    // Put hardware setup here

    return UIO_SUCCESS;
}

// Write to a register of the UIO device
void generic_write(uint32_t offset, uint32_t value)
{
    // Address is cast as a pointer so it can be dereferenced
    *((volatile uint32_t *)(ptr + offset)) = value;
}

// Read from a register of the UIO device
uint32_t generic_read(uint32_t offset)
{
    return *((volatile uint32_t *)(ptr + offset));
}

// Close the UIO device 
// This function must be called after all read/write operations are done
// to properly unmap the memory and close the file descriptor
void generic_exit()
{
    munmap(ptr, UIO_MMAP_SIZE);
    close(f);
}
