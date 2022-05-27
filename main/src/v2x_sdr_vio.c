/* 
 * Example was from: 
 * https://github.com/byu-cpe/ecen427_student/blob/master/userspace/drivers/vio/generic_vio.c
 *
 */

// Standard libraries
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

// Custom files
#include "v2x_sdr_vio.h"

// Local defines
#define OPEN_ERROR -1        // open() returns -1 on error
#define MMAP_OFFSET 0

// ---------------------- Globals ----------------------
static int f;     // this is a file descriptor that describes an open VIO device
static char *ptr; // this is the virtual address of the VIO device registers

// ---------------------- External functions ----------------------
// Initializes the vio driver, returns -1 if failed in error, 0 otherwise
int32_t generic_vio_init(char devDevice[])
{
    // Open the device
    f = open(devDevice, O_RDWR);
    if (f == OPEN_ERROR)
    {
        printf("vio example init error -- did you forget to sudo?\n");
        return VIO_ERROR;
    }

    // Memory map the physical address of the hardware into virtual address space
    ptr = (char*) (mmap(NULL, VIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, MMAP_OFFSET));
    if (ptr == MAP_FAILED)
    {
        return VIO_ERROR;
    }

    // Put hardware setup here

    return VIO_SUCCESS;
}

// Write to a register of the VIO device
void generic_vio_write(uint32_t offset, uint32_t value)
{
    // Address is cast as a pointer so it can be dereferenced
    *((volatile uint32_t *)(ptr + offset)) = value;
}

// Read from a register of the VIO device
uint32_t generic_vio_read(uint32_t offset)
{
    return *((volatile uint32_t *)(ptr + offset));
}

// Close the VIO device 
// This function must be called after all read/write operations are done
// to properly unmap the memory and close the file descriptor
void generic_vio_exit()
{
    munmap(ptr, VIO_MMAP_SIZE);
    close(f);
}
