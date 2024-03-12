/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "headers.h"

void inspectUnsavedFile(FILE *file) {
    // Calculate the length of bytes excluding the last 5 bytes
    long fileSize = 0;

    // Get file size
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);

    // Start from the 16th byte
    fseek(file, 15, SEEK_SET);

    // Exclude last 7 bytes
    fileSize -= 6;

    // Calculate number of bytes
    long bytesCount = fileSize - 14;

    // Determine the fseek offset based on bytesCount
    long offset = 0;
    if ((bytesCount / 2) < 128) {
        offset = 12;
        bytesCount = fileSize - 11;
    } else if ((bytesCount / 2) < 256) {
        offset = 13;
        bytesCount = fileSize - 12;
    } else if ((bytesCount / 2) < 128*128) { // 16,384 chars.
        offset = 15;
    } else if ((bytesCount / 2) < 128*128*128) { // 2,097,152 chars.
        offset = 18;
        bytesCount = fileSize - 16;
    } else if ((bytesCount / 2) < 128*128*128) { // 268,435,456 chars.
        offset = 21;
        bytesCount = fileSize - 18;        
    } else {
        offset = 24;
        bytesCount = fileSize - 20;         
    }

    fseek(file, offset, SEEK_SET);

    // Show the length to the end of the file
    printf("[=>] Total bytes to EoF (-5)    : %ld\n", bytesCount);

    // Show the length by dividing bytesCount by 2
    printf("[=>] Data Length                : %ld\n", bytesCount / 2);

    // Save the bytes to a file
    printf("[=>] Saving bytes to file...\n");

    // Uncomment the commented code below if
    // you want to print to the console instead, and comment the next.

    /*
    uint8_t byte;
    while (bytesCount > 0 && fread(&byte, sizeof(uint8_t), 1, file) == 1) {
        printf("%c", (char)byte);
        bytesCount--;
    }

    usleep(1500 * 1000); // usleep takes microseconds
    printf("\n[=>] Bytes printed to console.\n");
    */

    //
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char filename[50];
    sprintf(filename, "%04d%02d%02d_%02d%02d%02d.txt", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    uint8_t byte;
    while (bytesCount > 0 && fread(&byte, sizeof(uint8_t), 1, file) == 1) {
        fprintf(outputFile, "%c", (char)byte);
        bytesCount--;
    }

    fclose(outputFile);

    usleep(1500 * 1000); // usleep takes microseconds
    printf("[=>] Bytes saved to file: %s\n", filename);
    //
}