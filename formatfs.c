/*
This code formats and initializes the filesystem on the 
filesystem before anything else happens 
*/

// INCLUDES //
#include "filesystem.h"
// comment out below, just for current errors here right now
#include "filesystem.c"
#include "softwaredisk.h"
#include <stdint.h> // for uint types
#include <math.h> // for the ceil()
// INCLUDES //

int main()
{ // main // 

    // INIT DISK // 
    bool good_init = init_software_disk();
    
    if(!good_init)
    {
        current_error = FS_IO_ERROR; fs_print_error(); 
        printf("\n[DBG] failed initialization of software disk!!\n");
        return NULL;
    }
    // INIT DISK // 

    // SIZE INFO // 
    uint16_t DISK_SIZE = software_disk_size();
    uint64_t DISK_BYTES_TOTAL = (uint64_t) DISK_SIZE * (uint64_t) SOFTWARE_DISK_BLOCK_SIZE;
    printf("[DBG] SDBS: %d\nDS: %hu\nTBS: %u", SOFTWARE_DISK_BLOCK_SIZE, DISK_SIZE, DISK_BYTES_TOTAL);
    printf("\n[DBG] The size of the software disk (in disk blocks) is: %hu\nThe total bytes size, times the block size, is %u\n", DISK_SIZE, DISK_BYTES_TOTAL);
    // SIZE INFO //

    // INIT BITMAP // 
    //NOTE: NEED TO FIX / CHECK ..._bit() IMPLEMENATION BUT LOGIC RIGHT !!!!
    // mark bit map closed (SDB 0) to not open (0)
    set_bit(bitmap, 0); 
    // mark inode bits open
    for(int i = INODE_BLOCK_SD_START_LOCATION; i < NUM_INODE_BLOCKS; i++)
    {
        set_bit(bitmap, i); // open, to 1
    }
    //mark data blocks open
    for(int j = NUM_INODE_BLOCKS; j < NUM_BLOCKS; j++)
    {
        set_bit(bitmap, j); // open, to 1 to start
    }
    // INIT BITMAP // 

    // INIT INODE_BLOCKS // 

    // INIT INODE_BLOCKS // 

    // No Need to Init Data Blocks - Init as We Go



    return 0;

} // main // 