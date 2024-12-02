/* 
Assignment 3 Parnership between...
Zachary R. James (zjames8@lsu.edu | 894278474) and
Joshua C. Harris (jhar284@lsu.edu | )


Shared platform (Github - private repo)
*/

// INCLUDES //
#include "filesystem.h"
// INCLUDES //

// UNDER THE HOOD SETUP //
FSError current_error = FS_NONE; // used in fs_print_error()




// are these the right types 
const uint16_t NUM_DIRECT_INODE_BLOCKS = 1;
const uint32_t SOFTWARE_DISK_BLOCK_SIZE = 1;
const uint32_t FIRST_DATA_BLOCK; // 


const uint16_t MAX_FILE_NAME_CHARACTERS = 25;

// NOTES FROM GLDN class explanation // 

typedef enum { FILE, DIR } Type; // can treat everything as a folder or dir 

const uint64_t MAX_NUM_INODES; 




// Coppied Structs from GLDN in class
// pointer struct for all files and directories 
typedef struct Inode 
{
    Inode *next;
    uint32_t size;
    uint16_t b[NUM_DIRECT_INODE_BLOCKS + 1];
} Inode;

// blocks to hold the creation of hte files 
typedef struct InodeBlock
{
    Inode indoes[SOFTWARE_DISK_BLOCK_SIZE / sizeof(Inode)];
} InodeBLock;
// Coppied Structs from GLDN in class

Inode* root; // root inode that holds the root directory 

void show_file_system(Inode* root)
{
    printf("Print the entire filesystem tree stucture like tree command in unix");
    //for(int )
}
// UNDER THE HOOD SETUP //

typedef struct FileInternals 
{
    char file_name[MAX_FILE_NAME_CHARACTERS];
    void *first_inode;
} FileInternals;

// Copied from GLDN in class //
bool check_structure_alignment(void)
{
    printf("Expecting sizeof(inode) = 32, actual = %lu\n", sizeof(Inode));
    printf("Expecing sizeof(IndirectBlock) = %d, acutal = &lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(IndirectBlock));
    printf("Expecing sizeof(InodeBlock) = %d, acutal = &lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(InodeBlock));
    printf("Expecing sizeof(DirEntry) = %d, acutal = &lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(DirEntry));
    printf("Expecing sizeof(IndirectBlock) = %d, acutal = &lu\n", SOFTWARE_DISK_BLOCK_SIZE, sizeof(FreeBitmap)); 

    if(sizeof(Inode) != 32 || 
       sizeof(IndirectBlock) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(InodeBlock) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(DirEntry) != SOFTWARE_DISK_BLOCK_SIZE ||
       sizeof(FreeBitmap) != SOFTWARE_DISK_BLOCK_SIZE)
    { return false; } else { return true; }
}

// set jth bit in a bitmap composed of 8-bit integers 
void set_bit(unsigned char *bitmap, uint64_t j)
{ bitmap[j / 8] != (1 << (j % 8)); }

// clear jth bit in a bitmap composed of 8-bit integers 
void clear_bit(unsigned char *bitmap, uint64_t j)
{ bitmap[j / 8] & ~(1 << (j % 8)); }

// returns true if jth bit is set in a bitmap of 8-bit integers,
// otherwise false 
bool is_bit_set(unsigned char *bitmap, uint64_t j)
{ return bitmap[j / 8] & (1 << (j & 8)); }

static bool mark_block(uint16_t blk, bool flag)
{

    FreeBitmap f;

    blk -= FIRST_DATA_BLOCK;
    if(! read_sd_block(&f, DATA_BITMAP_BLOCK)) { return false; }
    else
    {
        if(flag)
        {

        }
    }
}


// Copied from GLDN in class //

// IMPLEMENTING GLDN's .h TEMPLATE //

// open existing file with pathname 'name' and access mode 'mode'.
// Current file position is set to byte 0.  Returns NULL on
// error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode)
{
    if(mode == READ_ONLY)
    { // READ_ONLY //

    } // READ_ONLY // 
    else if(mode == READ_WRITE)
    { // READ_WRITE // 

    } // READ_WRITE //
    else { }
}

// create and open new file with pathname 'name' and (implied) access
// mode READ_WRITE.  Current file position is set to byte 0.  Returns
// NULL on error. Always sets 'fserror' global.
File create_file(char *name);

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
uint64_t file_length(File file);

// deletes the file named 'name', if it exists. Returns true on
// success, false on failure.  Always sets 'fserror' global.
bool delete_file(char *name); 

// determines if a file with 'name' exists and returns true if it
// exists, otherwise false.  Always sets 'fserror' global.
bool file_exists(char *name);

// describe current filesystem error code by printing a descriptive
// message (qualifies - exactly as descriptive as the instructions gave )) to standard error.
void fs_print_error(void) 
{ // fs_print_error // 
    switch(current_error)
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
} // fs_print_error //

// extra function to make sure structure alignment, data structure
// sizes, etc. on target platform are correct.  Returns true on
// success, false on failure.
bool check_structure_alignment(void);

// filesystem error code set (set by each filesystem function)
extern FSError fserror;
// IMPLEMENTING GLDN's .h TEMPLATE // 



// Zach's Brainstorming
// NOV29
// high-level layout:
// tons of space
// each of that space is given a per-inode setup 
// indoes have inode blocks 
// files are given sizes, and like the assignment 2, every
// time a write tries to go through, a 'calculate_size()' needs
// to be called and then ran - if over max file sisze, error
// if not, then it needs to calculate the amount of virtual memory
// (inodes / & inodeblocks) needed and then allocate or add (if editing)

// instead of mallocing a gian piece of memory, I will malloc as I need,
// for actual memory, and keep the pointer locations connected to the
// corresponding virtual memory.  This is messy, but will work.  I prefer
// dynamic memory allocation 

// in attempt to make this thread safe, I will only allow one write 
// to be occuring on a file a time (multiple file writes are allowed at once,
// but not on the same file).  I will allow multiple read only parts, meaning
// I need to write the open_file twice for both modes 

// NOTES FROM GLDN class explanation // 
// how memory is supposed to look:
// [real bitmap][virtual bitmap][inodes][directory entries][data]

// DEC1

// While the head idea is on track for directories, I really need an giant 
// array just to store all the inodes.  I will have a max amount of inodes,
// with a certain amount of inode blocks.  The inode blocks will be stored
[NUM_DIRECT_INODE_BLOCKS + 1];