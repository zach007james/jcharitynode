/* 
Assignment 3 Parnership between...
Zachary R. James (zjames8@lsu.edu) and
Joshua C. Harris (jhar284@lsu.edu)
*/

// INCLUDES //
#include <pthread.h> // for thread safety
#include "filesystem.h"
#include "softwaredisk.h" // for SOFTWARE_DISK_BLOCK_SIZE
// INCLUDES // 

// MUTEX FOR SHARED RESOURCES //
pthread_mutex_t data_bitmap_mutex;
pthread_mutex_t inode_bitmap_mutex;
pthread_mutex_t inode_blocks_mutex;
pthread_mutex_t indirect_inode_blocks_mutex;
pthread_mutex_t dir_blocks_mutex;
// MUTEX FOR SHARED RESOURCES //

// filesystem error code set (set by each filesystem function)
FSError fserror = FS_NONE; // used in the fs_print_error()

// UNDER THE HOOD SETUP //
// are these the right types 
// GLDN DEFINED BLOCKS //
#define DATA_BITMAP_BLOCK 0 // FreeBitmap
#define INODE_BITMAP_BLOCK 1 // FreeBitmap
#define FIRST_INODE_BLOCK 2  // InodeBlock
#define LAST_INODE_BLOCK 55 // InodeBlock 
#define MAX_NUM_INODES 1728 // 54 blocks * 32 inodes per block
#define FIRST_DIR_ENTRY_BLOCK 56 // DirEntry
#define LAST_DIR_ENTRY_BLOCK 905 // DirEntry
#define FIRST_DATA_BLOCK 906 // DataBlock
#define LAST_DATA_BLOCK 8192 // DataBlock
#define MAX_FILENAME_SIZE 1020 // to make DirEntry struct be 1024 bytes
#define NUM_DIRECT_INODE_BLOCKS 13 // given in PDF
#define NUM_SINGLE_INDIRECT_INODE_BLOCKS (SOFTWARE_DISK_BLOCK_SIZE / sizeof(uint16_t)) // 128
// GLDN DEFINED BLOCKS //

#define MAX_FILES 800
#define INODES_PER_INODE_BLOCK (SOFTWARE_DISK_BLOCK_SIZE / sizeof(Inode))
#define MAX_DIR_ENTRIES_PER_BLOCK (SOFTWARE_DISK_BLOCK_SIZE / sizeof(DirEntry))
#define BITMAP_SD_LOCATION 0
#define INODE_BLOCK_SD_START_LOCATION 1

// OVERALL SD: [bitmap][inode blocks][Indirect Inode Block(s)][dir blocks[data blocks]

// SD: [bitmap (0)][dir entry (1)][inode blocks (2-33)][indirect ]
/*
typedef struct InodeLocation
{
    uint16_t inode_block_index;
    uint16_t inode_index;
} InodeLocation;

InodeLocation get_inode_location(uint16_t inode_index)
{ // get_inode_location() //
    InodeLocation ret_inode_location;
    if(inode_index < MAX_NUM_INODES)
    { // i //
        ret_inode_location.inode_block_index = FIRST_INODE_BLOCK + (inode_index / INODES_PER_INODE_BLOCK);
        ret_inode_location.inode_index = inode_index % INODES_PER_INODE_BLOCK;
    } // i //
    else if(inode_index == MAX_NUM_INODES)
    { // ei //
        ret_inode_location.inode_block_index = LAST_INODE_BLOCK;
        ret_inode_location.inode_index = inode_index % INODES_PER_INODE_BLOCK;
        printf("\n[DBG] inode_index == MAX_NUM_INODES (just used last inode block)\n");
    } // ei //
    else 
    { // e //
        printf("\n[DBG] inode_index out of range or other error in get_inode_location()\n");
        fserror = FS_ILLEGAL_FILENAME;
        fs_print_error();
    } // e //
    return ret_inode_location;
} // get_inode_location() //
*/

// pointer struct for all files and directories 
typedef struct Inode 
{
    uint32_t size; // size of the poetry 
    uint16_t b[NUM_DIRECT_INODE_BLOCKS + 1];
} Inode;

// implemenation needs to be adjusted and create file and find file 
// need to be updated to use indices and not pointers
typedef struct DirEntry
{
    bool file_is_open;
    uint16_t inode_index;
    char file_name[MAX_FILENAME_SIZE];
} DirEntry;

typedef struct FreeBitmap // 1024 bytes in size 
{
    uint8_t bytes[SOFTWARE_DISK_BLOCK_SIZE]; // works out bc we have 8 * 1k total SD blocks but each byte has 8 bits so it works out 
} FreeBitmap;

// blocks to hold the creation of the files 
typedef struct InodeBlock
{
    Inode inodes[INODES_PER_INODE_BLOCK];
} InodeBlock;
// Coppied Structs from GLDN in class

typedef struct IndirectBlock
{
    uint16_t idx_to_more_space[NUM_SINGLE_INDIRECT_INODE_BLOCKS];
} IndirectBlock;

typedef struct FileInternals 
{
    char file_name[MAX_FILENAME_SIZE];
    uint32_t inode_index; // only one inode per file 
    bool is_open;
} FileInternals;

// BITMAP HELPER METHODS //
// TODO: need to re-learn the internals of these later

// set jth bit in a bitmap composed of 8-bit integers 
void set_bit(unsigned char *bitmap, uint64_t j)
{ bitmap[j / 8] |= (1 << (j % 8)); } 

// clear jth bit in a bitmap composed of 8-bit integers 
void clear_bit(unsigned char *bitmap, uint64_t j)
{ bitmap[j / 8] &= ~(1 << (j % 8)); }

// returns true if jth bit is set in a bitmap of 8-bit integers,
// otherwise false 
bool is_bit_set(unsigned char *bitmap, uint64_t j)
{ return bitmap[j / 8] & (1 << (j & 8)); }

// marks a SD block as free or used (true = )
static bool mark_block(uint16_t blk, bool flag)
{ // mark_block() //
    FreeBitmap f;
    blk -= FIRST_DATA_BLOCK; // sets the relative data block number
    if(! read_sd_block(&f, DATA_BITMAP_BLOCK)) { return false; }
    else
    {
        if(flag) { set_bit(f.bytes, blk); }
        else { clear_bit(f.bytes, blk); }
    }

    return true;
} // mark_block() //

// BITMAP HELPER METHODS //


// IMPLEMENTING GLDN's .h TEMPLATE //

// open existing file with pathname 'name' and access mode 'mode'.
// Current file position is set to byte 0.  Returns NULL on
// error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode)
{ // open_file() //
    if(mode == READ_ONLY)
    { // READ_ONLY //

    } // READ_ONLY // 
    else if(mode == READ_WRITE)
    { // READ_WRITE // 

    } // READ_WRITE //
    else { printf("\nInvalid FileMode selection.\n"); }
    return NULL; // I don't think I am allowed to do this 
} // open_file() //

// create and open new file with pathname 'name' and (implied) access
// mode READ_WRITE.  Current file position is set to byte 0.  Returns
// NULL on error. Always sets 'fserror' global.
File create_file(char *name)
{ // create_file() //
    // threadsafe call here??

    File ret_file; // init file
    ret_file = malloc(sizeof(struct FileInternals)); // malloc local RAM / program storage for this
    return ret_file;
} // create_file() //


// close 'file'.  Always sets 'fserror' global.
void close_file(File file);

// read at most 'numbytes' of data from 'file' into 'buf', starting at
// the current file position.  Returns the number of bytes read. If
// end of file is reached, then a return value less than 'numbytes'
// signals this condition. Always sets 'fserror' global.
uint64_t read_file(File file, void *buf, uint64_t numbytes);

// write 'numbytes' of data from 'buf' into 'file' at the current file
// position.  Returns the number of bytes written. On an out of space
// error, the return value may be less than 'numbytes'.  Always sets
// 'fserror' global.
uint64_t write_file(File file, void *buf, uint64_t numbytes);

// sets current position in file to 'bytepos', always relative to the
// beginning of file.  Seeks past the current end of file should
// extend the file. Returns true on success and false on failure.
// Always sets 'fserror' global.
bool seek_file(File file, uint64_t bytepos);

// returns the current length of the file in bytes. Always sets
// 'fserror' global.
uint64_t file_length(File file); // saw the dir entry for the other things 

// deletes the file named 'name', if it exists. Returns true on
// success, false on failure.  Always sets 'fserror' global.
bool delete_file(char *name); 

// determines if a file with 'name' exists and returns true if it
// exists, otherwise false.  Always sets 'fserror' global.
bool file_exists(char *name)
{ // file_exists() //
    // check filename
    if(name == NULL || name[0] == '\0')
    { fserror = FS_ILLEGAL_FILENAME; return false; }
    
    /*
    // seach through inode blocks
    for(int i = 0; i < NUM_INODE_BLOCKS; i++)
    {
        InodeBlock inode_block_buff;

        // read in while tesing 
        if(!(read_sd_block(&inode_block_buff, i + INODE_BLOCK_SD_START_LOCATION)))
        {
            fserror = FS_IO_ERROR;
            printf("\n[DBG] Could not read a block while searching\n");
            return false;
        } 

        for(int j = 0; j < INODES_PER_INODE_BLOCK; j++)
        {
            if(strcmp(inode_block_buff.inodes[j].file_internals->file_name, name) == 0)
            { fserror = FS_NONE; fs_print_error(); true; }
        }
    }
    */

    fserror = FS_FILE_NOT_FOUND;
    fs_print_error();
    return false; 
}

// describe current filesystem error code by printing a descriptive
// message (qualifies - exactly as descriptive as the instructions gave )) to standard error.
void fs_print_error(void) 
{ // fs_print_error() // 
    switch(fserror)
    { // s // 
        case FS_NONE:
        { printf("Great job, no error!"); break; }
        case FS_OUT_OF_SPACE: 
        { printf("[ERR] the operaation caused the software disk to fill up"); break; } 
        case FS_FILE_NOT_OPEN: 
        { printf("[ERR] attempted read / write / close / &c on file that isn't open"); break; }
        case FS_FILE_OPEN:
        { printf("[ERR] file is already open. Concurrent opens are not supported and neither is deleting a file that is open."); break; }
        case FS_FILE_NOT_FOUND:
        { printf("[ERR] attempted open or delete of file that doesn't exist"); break; }
        case FS_FILE_READ_ONLY:
        { printf("[ERR] attempted write to file opened for READ_ONLY"); break; }
        case FS_FILE_ALREADY_EXISTS:
        { printf("attempted creation of file with existing name"); break; }
        case FS_EXCEEDS_MAX_FILE_SIZE:
        { printf("seek or write would exceed max file size"); break; }
        case FS_ILLEGAL_FILENAME: 
        { printf("[ERR] filename begins with a null character"); break; }
        case FS_IO_ERROR: 
        { printf("[ERR] something really bad happened"); break; } 
    } // s //
} // fs_print_error() //

// extra function to make sure structure alignment, data structure
// sizes, etc. on target platform are correct.  Returns true on
// success, false on failure.
bool check_structure_alignment(void)
{ // check_structure_alignment() // 
    printf("Expecting sizeof(inode) = 32, actual = %lu\n", sizeof(Inode));
    printf("Expecing sizeof(IndirectBlock) = %d, acutal = %lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(IndirectBlock));
    printf("Expecing sizeof(InodeBlock) = %d, acutal = %lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(InodeBlock));
    printf("Expecing sizeof(DirEntry) = %d, acutal = %lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(DirEntry));
    printf("Expecing sizeof(FreeBitmap) = %d, acutal = %lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(FreeBitmap)); 
    if(sizeof(Inode) != 32 || 
       sizeof(IndirectBlock) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(InodeBlock) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(DirEntry) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(FreeBitmap) != SOFTWARE_DISK_BLOCK_SIZE)
    { return false; } else { return true; }
} // check_structure_alignment() //
