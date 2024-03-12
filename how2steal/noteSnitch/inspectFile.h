/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "headers.h"

// Function to check if a filename is a valid .bin file
int isValidBinFile(const char *filename) {
    size_t len = strlen(filename);
    return (len >= 4 && strcmp(filename + len - 4, ".bin") == 0 &&
            !strstr(filename, ".0.bin") && !strstr(filename, ".1.bin"));
}

// Function to check the 4th byte of the file
int checkFourthByte(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Seek to the 4th byte
    fseek(file, 3, SEEK_SET);
    uint8_t byte;
    fread(&byte, sizeof(uint8_t), 1, file);

    fclose(file);

    return byte;
}