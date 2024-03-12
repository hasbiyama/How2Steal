/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "headers.h"

// Function to inspect saved files
void inspectSavedFile(FILE *file, int filePathSize) {
    // Move to the end of the file to get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    // Move back to the beginning of the file
    fseek(file, 0, SEEK_SET);

    // Read the fifth byte to determine the filePathSize
    fseek(file, 4, SEEK_SET);
    uint8_t fifthByte;
    fread(&fifthByte, sizeof(uint8_t), 1, file);
    filePathSize = fifthByte * 2;

    // Read the file path from the specified offset
    fseek(file, 5, SEEK_SET);
    wchar_t wchar;
    printf("[=>] filePath                   : ");
    for (int i = 0; i < filePathSize; ++i) {
        if (fread(&wchar, sizeof(wchar_t), 1, file) && wchar != L'\0') {
            wprintf(L"%lc", wchar);
        }
    }
    printf("\n");
    printf("[=>] sizeof(filePath)           : %d bytes (0x%x/2)\n", filePathSize/2, fifthByte);
    printf("[=>] Offset at which loop stops : 0x%x\n", filePathSize + 4);

    // Find the sequence "05 (or 02) 01" after the specified offset
    int offsetDifference = -1;
    fseek(file, filePathSize + 5, SEEK_SET);
    uint8_t byte1, byte2, byte3, byte4;
    while (fread(&byte1, sizeof(uint8_t), 1, file) && fread(&byte2, sizeof(uint8_t), 1, file)) {
        if ((byte1 == 0x05 && byte2 == 0x01) || (byte1 == 0x02 && byte2 == 0x01)) {
            offsetDifference = ftell(file) - filePathSize - 7; // Calculate the offset difference
            break;
        }
        fseek(file, -1, SEEK_CUR);
    }

    if (offsetDifference >= 0) {
        printf("\n[=>] Offset difference to sequence \"05 (or 02) 01\": 0x%x\n", offsetDifference);

        // Check if the 42nd and 43rd bytes from the "05 (or 02) 01" offset are "00 01"
        fseek(file, filePathSize + 5 + offsetDifference + 1 + 42, SEEK_SET);
        uint8_t byte42, byte43;
        fread(&byte42, sizeof(uint8_t), 1, file);
        fread(&byte43, sizeof(uint8_t), 1, file);

        if (byte42 == 0x00 && byte43 == 0x01) {
            printf("[=>] 42nd byte from \"05 (or 02) 01\" offset is 0x00 and 43rd byte is 0x01.\n");

            // Check for the sequence "01 00 00" after "05 (or 02) 01"
            fseek(file, filePathSize + 5 + offsetDifference + 1, SEEK_SET);
            while (fread(&byte1, sizeof(uint8_t), 1, file) && fread(&byte2, sizeof(uint8_t), 1, file)
                   && fread(&byte3, sizeof(uint8_t), 1, file) && fread(&byte4, sizeof(uint8_t), 1, file)) {
                if (byte1 == 0x01 && byte2 == 0x00 && byte3 == 0x00 && byte4 == 0x00) {
                    // Calculate the offset difference
                    int offsetDiffSecond = ftell(file) - (filePathSize + 5 + offsetDifference + 3 + 44); // +3 bcs it's +1 from "05 (or 02) 01" offset && +44 bcs it's +1 from "00 01" offset
                    printf("[=>] Total Offset difference between \"00 01\" and \"01 00 00\": 0x%x\n", offsetDiffSecond - 2); // -2 bcs removing 1 offset from "+3" and "+1" above

                    if (offsetDiffSecond - 2 == 2) {
                        // Check if bytes are both 0
                        fseek(file, -2, SEEK_CUR); // Move back to check the previous two bytes
                        uint8_t prevByte1, prevByte2;
                        fread(&prevByte1, sizeof(uint8_t), 1, file);
                        fread(&prevByte2, sizeof(uint8_t), 1, file);
                        if (prevByte1 == 0x00 && prevByte2 == 0x00) {
                            // If both bytes are 0, adjust fseek
                            fseek(file, (offsetDifference) - 1, SEEK_CUR);
                            printf("[=>] fseek adjusted.\n");
                        }
                    }

                    // Move to the end of "01 00 00" and add offsetDiffSecond/2
                    fseek(file, (offsetDiffSecond/2) - 1, SEEK_CUR); // -1 since it will be start from ""01 00 00" +1 offset
                    int currentOffset = ftell(file);
                    printf("[=>] Current Offset after adding offsetDiffSecond/2: 0x%x\n", currentOffset);

                    // Calculate bytes to the end of the file
                    int bytesToEnd = fileSize - currentOffset - 5;
                    printf("\n[=>] Total bytes to EoF (-5)    : %d\n", bytesToEnd);

                    // Show the length by dividing bytesCount by 2
                    printf("[=>] Data Length                : %ld\n", bytesToEnd / 2);

                    // Divide the result by 2
                    bytesToEnd / 2;

                    // Print bytes in ANSI format
                    printf("[=>] Bytes in ANSI format       : ");
                    fseek(file, currentOffset, SEEK_SET);
                    for (int i = 0; i < bytesToEnd; ++i) {
                        uint8_t byte;
                        if (fread(&byte, sizeof(uint8_t), 1, file)) {
                            printf("%c", (char)byte);
                        } else {
                            printf("Error reading file.");
                            break;
                        }
                    }
                    printf("\n");
                    break;
                }
                fseek(file, -3, SEEK_CUR);
            }
        } else {
            printf("[=>] 42nd and 43rd bytes from \"05 (or 02) 01\" offset are not 0x00 and 0x01.\n");
        }

    } else {
        printf("Sequence \"05 (or 02) 01\" not found after offset i + 5.\n");
    }
}