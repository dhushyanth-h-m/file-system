#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "disk.h"  // Include disk header for disk operations
#include <stdint.h> // For fixed-width integer types
#include <sys/types.h> // For ssize_t

#define DIRECT_POINTERS 12 // Number of direct pointers in an inode
#define INODES_PER_BLOCK 128 // Number of pointers in an indirect block
#define POINTERS_PER_BLOCK 1024 // Number of pointers per block

typedef struct {
    uint32_t magic_number;
    uint32_t total_blocks;
    uint32_t inode_blocks;
    uint32_t total_inodes;
    uint32_t data_block_start;
} superblock_t;

typedef struct {
    uint32_t mode;
    uint32_t size;
    uint32_t uid;  // User ID of the owner
    uint32_t gid; // Group ID of the owner
    uint32_t atime; // Last access time
    uint32_t mtime; // Last modification time
    uint32_t ctime; // Last status change time
    uint32_t direct[DIRECT_POINTERS]; // Direct pointers to data blocks
    uint32_t indirect; // Pointer to an indirect block
    uint32_t valid; // Validity flag
} inode_t;

typedef struct {
    disk_t *disk; // Pointer to the disk structure 
    superblock_t *sb; // Pointer to the superblock structure
    unsigned char *inode_bitmap; // Bitmap for inodes
    unsigned char *data_bitmap; // Bitmap for data blocks
    bool mounted; // Whether the filesystem is mounted
} filesystem_t;

// Function to create a filesystem
bool fs_format(disk_t *disk);
bool fs_mount(filesystem_t *fs, disk_t *disk);
void fs_unmount(filesystem_t *fs);
int fs_create(filesystem_t *fs);
bool fs_remove(filesystem_t *fs, int inode_num);
ssize_t fs_read(filesystem_t *fs, int inode_num, char *buffer, size_t length, off_t offset);
ssize_t fs_write(filesystem_t *fs, int inode_num, const char *buffer, size_t length, off_t offset);
void fs_debug(filesystem_t *fs);
#endif