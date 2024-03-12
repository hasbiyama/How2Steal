/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <dirent.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define UNSAVED_MARKER "unsaved"
#define SAVED_MARKER "saved"
#define MAX_REPEAT_COUNT 4

// Function prototypes
int isValidBinFile(const char *filename);
int checkFourthByte(const char *filename);
void inspectSavedFile(FILE *file, int filePathSize);
void inspectUnsavedFile(FILE *file);