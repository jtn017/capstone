/* 
 * Example was from: 
 *  https://github.com/byu-cpe/ecen427_student/blob/master/userspace/drivers/uio/generic_uio.c
 *
 *****************************************************************************************************/

#include <iostream>
#include "rx_ip.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define UIO_MMAP_SIZE 0x1000 // size of memory to allocate (rx_IP is 64K but don't need all of that)
#define OPEN_ERROR -1        // open() returns -1 on error

enum rx_ip_regs{
    rx_ip_rdy = 0x10,
    rx_ip_rdy_vld = 0x14,
    rx_ip_version = 0x20,
    rx_ip_version_vld = 0x24,
    rx_ip_ouput = 0x100 // (64 * 32b) 0x100-0x1ff
};

/********************************** globals ***********************************/
static int f;     // this is a file descriptor that describes an open UIO device
static char *ptr; // this is the virtual address of the UIO device registers

/********************************* functions **********************************/
// initializes the uio driver, returns -1 if failed in error, 0 otherwise
int32_t generic_init(char devDevice[]) {

  // open the device
  f = open(devDevice, O_RDWR);
  if (f == OPEN_ERROR) {
    printf("uio example init error -- did you forget to sudo?\n");
    return UIO_ERROR;
  }

  // memory map the physical address of the hardware into virtual address space
  ptr = static_cast<char*>(mmap(NULL, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f, MMAP_OFFSET));
  if (ptr == MAP_FAILED) {
    return UIO_ERROR;
  }

  /* put hardware setup here */

  return UIO_SUCCESS;
}

// write to a register of the UIO device
void generic_write(uint32_t offset, uint32_t value) {
  // the address is cast as a pointer so it can be dereferenced
  *((volatile uint32_t *)(ptr + offset)) = value;
}

// read from a register of the UIO device
uint32_t generic_read(uint32_t offset) {
  return *((volatile uint32_t *)(ptr + offset));
}

// close the UIO device
//	this function must be called after all read/write operations are done
//	to properly unmap the memory and close the file descriptor
void generic_exit() {
  munmap(ptr, UIO_MMAP_SIZE);
  close(f);
}

int main()
{
    char rx_ip[] = "/dev/uio0";
    uint32_t rdy = 0;
    uint32_t rdy_vld = 0;
    uint32_t version;
    uint32_t version_vld;
    uint32_t res[64];

    std::cout << "Starting RX IP Interface" << std::endl;
    
    generic_init(rx_ip);


    // Poll Ready Signal
    std::cout << "Polling..." << std::endl;
    while(rdy == 0){
        rdy = generic_read(rx_ip_rdy);
        rdy_vld = generic_read(rx_ip_rdy_vld);
    }
    std::cout << "Rdy: " << std::hex << rdy << " Valid: " << std::hex << rdy_vld << std::endl;

    // Read Version Number (this is mostly just for IP testing)
    version = generic_read(rx_ip_version);
    version_vld = generic_read(rx_ip_version_vld);
    std::cout << "Version: " << std::hex << version << " Valid: " << std::hex << version_vld << std::endl;

    // Read Demodulated Data
    std::cout << "Read Received Data" << std::endl;
    for(int idx = 0; idx <64; idx++)
    {
        res[idx] = generic_read(rx_ip_ouput+(idx*4)); // *4 because byte addressing
        std::cout << "Result[" << idx << "] = " << std::hex << res[idx] << std::endl;
    }

    // Cleanup
    generic_exit();
    std::cout << "Thats all folks!!!" << std::endl;

    return 0;
}