#include "../include/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Bitmap manipulation functions
static void bitmap_set(unsigned char *bitmap, int bit_num) {
    bitmap[bit_num / 8] |= (1 << (bit_num % 8));
}

static void bitmap_clear(unsigned char *bitmap, int bit_num) {
    bitmap[bit_num / 8] &= ~(1 << (bit_num % 8));
}

static int bitmap_test(unsigned char *bitmap, int bit_num) {
    return (bitmap[bit_num / 8] & (1 << (bit_num % 8))) != 0;
}

static int bitmap_find_free(unsigned char *bitmap, int bitmap_size) {
    for (int i = 0; i < bitmap_size * 8; i++) {
        if (!bitmap_test(bitmap, i)) {
            return i; // Return the first free bit index
        }
    }
    return -1; // Return -1 if no free bit is found
}


bool fs_format(disk_t *disk) {
    if (!disk || disk->mounted) {
        return false; // Return false if disk is NULL or already mounted
    }

    char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE); // Clear the block

    // Create superblock
    superblock_t *sb = (superblock_t *)block; // Cast block to superblock_t pointer
    sb->magic_number = MAGIC_NUMBER; // Set magic number
    sb->total_blocks = disk->blocks; // Set total blocks
    sb->inode_blocks = (disk->blocks * 10) / 100; // Set inode blocks to 10% of total blocks
    sb->total_inodes = sb->inode_blocks * INODES_PER_BLOCK; // Calculate total inodes
    sb->data_block_start = 3 + sb->inode_blocks; // Data block start after superblock + 2 bitmaps + inodes


    // write superblock to disk
    if (disk_write(disk, 0, block) < 0) {
        return false; // Return false if writing superblock fails
    }

    // clear inode and data bitmaps
    memset(block, 0, BLOCK_SIZE); // Clear the block for bitmaps
    if (disk_write(disk, 1, block) < 0) {
        return false; // Return false if writing inode bitmap fails
    }

    if (disk_write(disk, 2, block) < 0) {
        return false; // Return false if writing data bitmap fails
    }

    // clear inode table
    for (uint32_t i = 0; i < sb->inode_blocks; i++) {
        if (disk_write(disk, 3 + i, block) < 0) return false;
    }

    return true;
}

bool fs_mount(filesystem_t *fs, disk_t *disk) {
    if (!fs || !disk || disk->mounted) return false;

    char block[BLOCK_SIZE];

    // read superblock
    if (disk_read(disk, 0, block) < 0) return false;

    memcpy(&fs->sb, block, sizeof(superblock_t));

    // verify bitmap memory
    if (fs->sb.magic_number != MAGIC_NUMBER) return false;

    
}


