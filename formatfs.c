/*
This code formats and initializes the filesystem on the 
filesystem before anything else happens 
*/

// INCLUDES //
#include "filesystem.h"
#include "softwaredisk.h"
#include <stdint.h> // for uint types
#include <math.h> // for the ceil()
// INCLUDES //


#define MAX_FILE_NAME_CHARACTERS 25

typedef struct FileInternals 
{
    char file_name[MAX_FILE_NAME_CHARACTERS];
    void *first_inode;
} FileInternals;


int main()
{ // main // 

    // Init disk
    bool good_init = init_software_disk();
    uint16_t DISK_SIZE = software_disk_size();
    uint64_t DISK_BYTES_TOTAL = (uint64_t) DISK_SIZE * (uint64_t) SOFTWARE_DISK_BLOCK_SIZE;
    printf("SDBS: %d\nDS: %hu\nTBS: %u", SOFTWARE_DISK_BLOCK_SIZE, DISK_SIZE, DISK_BYTES_TOTAL);
    printf("The size of the software disk (in disk blocks) is: %hu\nThe total bytes size, times the block size, is %u\n", DISK_SIZE, DISK_BYTES_TOTAL);

    // Init the filesystem overhead structure

    uint16_t MAX_FILES = 800;
    uint64_t FILE_INTER_SIZE = sizeof(FileInternals);
    uint64_t MAX_REQUIRED_MAP_SPACE = MAX_FILES * FILE_INTER_SIZE;

    uint64_t MAX_FILE_INTERS_PER_SDB = floor(SOFTWARE_DISK_BLOCK_SIZE / FILE_INTER_SIZE);
    uint64_t SD_BLOCKS_FOR_BITMAP_NEEDED = ceil(MAX_REQUIRED_MAP_SPACE / MAX_FILE_INTERS_PER_SDB);

    printf("Max Files: %hu\nFileInternals struct size: %u\nMax File Internals Stucts be SDB: %u\nReq. Map Space: %u\nSD BLocks for Map Needed: %u", MAX_FILES, FILE_INTER_SIZE, MAX_FILE_INTERS_PER_SDB, MAX_REQUIRED_MAP_SPACE, SD_BLOCKS_FOR_BITMAP_NEEDED);
    // setup the filemap 

    // wrote the s

    for(uint32_t i = 0; i < SD_BLOCKS_FOR_BITMAP_NEEDED; i++)
    {
        FileInternals filemap[MAX_FILE_INTERS_PER_SDB]; // create the part held in this code
        // fill filemap with two example files 
        FileInternals a, b;
        char namea[5] = "file1"; char nameb[6] = "filebb";
        a->file_name = namea;
        filemap[0].file_name = a; filemap[1].file_name = b;
        
        write_sd_block(filemap, i); 
    }

    printf("Initializes every filemap with first two files two files to the first SD block\n");

    printf("Now reading the data back to see if it worked.\n");
    // init all other memory to inodes

    for(uint32_t j = 0; j < SD_BLOCKS_FOR_BITMAP_NEEDED; j++)
    {

        FileInternals abuff, bbuff;
        FileInternals filemapbuff[MAX_FILE_INTERS_PER_SDB];
        read_sd_block(filemapbuff, j);
        printf("\nfile1: %s | file2: %s", abuff->file_name, bbuff->file_name);
    }

    return 0;

} // main // 