/*
This code formats and initializes the filesystem on the 
filesystem before anything else happens 
*/

// INCLUDES //
#include "filesystem.h"
// comment out below, just for current errors here right now
#include "softwaredisk.h"
//#include <stdint.h> // for uint types
//#include <math.h> // for the ceil()
// INCLUDES //

int main()
{ // main // 

    // INIT DISK // 
    bool good_init = init_software_disk();
    
    if(!good_init)
    {
        fserror = FS_IO_ERROR; fs_print_error(); 
        printf("\n[DBG] failed initialization of software disk!!\n");
        return 1;
    } else { printf("\nSoftware disk formatted!\n"); }
    // INIT DISK // 

    // INIT MUTEXES // 

    bool good_mutexes = init_mutexes();
    if(!good_mutexes)
    {
        fserror = FS_IO_ERROR; fs_print_error();
        printf("\n[DBG] failed initialization of mutexes!!\n");
        return 1;
    } else { printf("\nMutexes initialized!\n"); }
    // INIT MUTEXES // 

    // CHECK STRUCTURE ALIGNMENT // 
    bool is_aligned = check_structure_alignment();

    if(!is_aligned)
    {
        printf("\nStructure alignment check failed!\n");
        return 1;
    } else { printf("\nStructure alignment check passed!\n"); }


    // everything below will be done in time or in filesystem.c, so while the 
    // structure is nice, formatfs.c is complete - asked GLDN in class
    /*

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
    uint32_t REQUIRED_INODE_BLOCKS = sizeof(InodeBlock) / SOFTWARE_DISK_BLOCK_SIZE;
    for(int k = 0; k < REQUIRED_INODE_BLOCKS; k++)
    {
        InodeBlock inode_block_init;
        write_sd_block(&inode_block_init, INODE_BLOCK_SD_START_LOCATION + k);
    }
    // INIT INODE_BLOCKS // 

    // INIT DIRECTORY_BLOCK //
    uint32_t REQUIRED_DIR_BLOCKS = sizeof(DirBlock) / SOFTWARE_DISK_BLOCK_SIZE;
    for(int l = 0; l < REQUIRED_DIR_BLOCKS; l++)
    {
        DirBlock directory_block_init;
        write_sd_block(&directory_block_init, DIR_BLOCK_SD_START_LOCATION + l);
    }
    // INIT DIRECTORY_BLOCK // 

    // No Need to Init Data Blocks - Init as We Go
    */

    return 0;

} // main // 