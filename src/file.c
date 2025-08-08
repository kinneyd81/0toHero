#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"
#include "common.h"


int create_db_file(char *filename) {
    // Try to open file, it should not exist
    int fd = open(filename, O_RDONLY);
    if (fd != -1) {
        close(fd); 
        printf("File %s already exists!\n", filename);
        return STATUS_ERROR;
    }

    // OK, try to create file
    fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    }

    // return file descriptor
    return fd;
}

int open_db_file(char *filename) {
    int fd = open(filename, O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    // return file descriptor
    return fd;
}


