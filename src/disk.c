#include "../include/disk.h"
#include <stdio.h> // For printf
#include <stdlib.h> // For malloc, free
#include <unistd.h> // For close
#include <fcntl.h> // For open
#include <string.h> // For memset

disk_t* disk_open(const char *path, size_t blocks) {
    disk_t *disk = malloc(sizeof(disk_t)); // Allocate memory for the disk structure
    if (!disk) {
        perror("Failed to allocate memory for disk");
        return NULL; // Return NULL if memory allocation fails
    }

    disk->fd = open(path, O_RDWR | O_CREAT, 0644); // Open the disk file
    if (disk->fd < 0) {
        free(disk); // Free allocated memory on failure
        perror("Failed to open disk file");
        return NULL; // Return NULL if opening the file fails
    }

    disk->blocks = blocks; // Set the number of blocks
    disk->reads = 0; // Initialize reads to 0
    disk->writes = 0; // Initialize writes to 0
    disk->mounted = false; // Set mounted to false

    // Ensure the disk file is correct size
    if (ftruncate(disk->fd, blocks * BLOCK_SIZE) < 0) {
        close(disk->fd);
        free(disk); // Free allocated memory on failure
        perror("Failed to set disk size");
        return NULL; // Return NULL if setting the size fails
    }

    return disk; // Return the disk structure
}


void disk_close(disk_t *disk) {
    if (disk) {
        close(disk->fd); // Close the file descriptor
        free(disk); // Free the allocated memory for the disk structure
    }
}

int disk_read(disk_t *disk, size_t block, char *data) {
    if (!disk || block >= disk->blocks) {
        return -1; // Return -1 on invalid parameters
    }

    off_t offset = block * BLOCK_SIZE; // Calculate the offset
    if (lseek(disk->fd, offset, SEEK_SET) < 0) {
        perror("Failed to seek in disk file");
        return -1; // Return -1 if seeking fails
    }

    ssize_t result = read(disk->fd, data, BLOCK_SIZE); // Read data from the disk
    if (result == BLOCK_SIZE) {
        disk->reads++; 
        return 0; // Return 0 on success
    }

    return -1; // Return -1 on read failure
}

int disk_write(disk_t *disk, size_t block, char *data) {
    if (!disk || block >= disk->blocks) {
        return -1; // Return -1 on invalid parameters
    }

    off_t offset = block * BLOCK_SIZE;  
    if (lseek(disk->fd, offset, SEEK_SET) < 0) {
        perror("Failed to seek in disk file");
        return -1; // Return -1 if seeking fails
    }

    ssize_t result = write(disk->fd, data, BLOCK_SIZE); // Write data to the disk
    if (result == BLOCK_SIZE) {
        disk->writes++;
        return 0; // Return 0 on success
    }

    return -1; // Return -1 on write failure
}