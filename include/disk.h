#ifndef DISK_H
#define DISK_H

#include <stdint.h>   // For fixed-width integer types 
#include <stdbool.h> // For bool type
#include <stddef.h>  // For size_t

#define BLOCK_SIZE 4096 // Size of a block in bytes
#define MAGIC_NUMBER 0xF0F03410 // Magic number for the disk

typedef struct {
    int fd; // File descriptor for the disk
    size_t blocks;  // Number of blocks
    size_t reads; // Number of reads
    size_t writes; // Number of writes
    bool mounted; // Whether the disk is mounted
} disk_t;

// Function to create a disk
disk_t* disk_open(const char* path, size_t blocks);
void disk_close(disk_t *disk);
int disk_read(disk_t *disk, size_t block, char *data);
int disk_write(disk_t *disk, size_t block, char *data);

#endif