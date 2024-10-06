#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void errExit(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){

    int fdSrc;
    int fdDest;
    char *addrSrc;
    char *addrDest;
    struct stat sb;

    if (argc != 3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fdSrc = open(argv[1], O_RDONLY);
    if (fdSrc == -1)
        errExit("fd");

    if (fstat(fdSrc, &sb) == -1)
        errExit("fstat");

    addrSrc = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fdSrc, 0);
    if (addrSrc == MAP_FAILED)
        errExit("mmap");

    fdDest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (ftruncate(fdDest, sb.st_size) == -1)
        errExit("ftruncate");

    addrDest = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdDest, 0);
    if (addrDest == MAP_FAILED)
        errExit("mmap");

    memcpy(addrDest, addrSrc, sb.st_size);
    if (msync(addrDest, sb.st_size, MS_SYNC) == -1)
        errExit("msync");

    if (munmap(addrSrc, sb.st_size) == -1)
        errExit("munmap");
    if (munmap(addrDest, sb.st_size) == -1)
        errExit("munmap");

    exit(EXIT_SUCCESS);
}
