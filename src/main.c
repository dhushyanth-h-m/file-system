#include "../include/disk.h"
#include "../include/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_menu() {
    printf("\n===Simple File System===\n");
    printf("1. Format disk\n");
    printf("2. Mount filesystem\n");
    printf("3. Create file\n");
    printf("4. Write to file\n");
    printf("5. Read from file\n");
    printf("6. Remove file\n");
    printf("7. Debug info\n");
    printf("8. Unmount\n");
    printf("9. Exit\n");
    printf("Choice: ");
}

int main() {
    disk_t *disk = disk_open("disk.dat", 100);
    filesystem_t fs;

    if (!disk) {
        printf("Failed to open disk\n");
        return 1;
    }

    int choice;
    bool mounted = false;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch(choice) {
            case 1: 
                if (fs_format(disk)) {
                    printf("Disk formatted successfully\n");
                } else {
                    printf("Format failed\n");
                }
                break;

            case 2: 
                if (fs_mount(&fs, disk)) {
                    printf("Filesystem mounted\n");
                    mounted = true;
                } else  {
                    printf("Mount failed\n");
                }
                break;

            case 3: 
                if (mounted) {
                    int inode = fs_create(&fs);
                    if (inode >= 0) {
                        printf("File created with inode %d\n", inode);
                    } else {
                        printf("File creation failed\n");
                    }
                } else {
                    printf("Filesystem not mounted\n");
                }
                break;

            case 9:
                if (mounted) fs_unmount(&fs);
                disk_close(disk);
                return 0;

            default: 
                printf("Invalid choice\n");
        }
    }

}