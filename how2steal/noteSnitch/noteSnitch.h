/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "inspectFile.h"
#include "inspectSavedFile.h"
#include "inspectUnsavedFile.h"

int noteSnitch() {
    DIR *dir;
    struct dirent *entry;
    char *localappdata;
    char path[256];
    int count = 0;

    // Get the value of the environment variable %localappdata%
    localappdata = getenv("localappdata");
    if (localappdata == NULL) {
        perror("\n[-] Error getting environment variable");
        return 1;
    }

    // Construct the full path
    snprintf(path, sizeof(path), "%s\\Packages\\Microsoft.WindowsNotepad_8wekyb3d8bbwe\\LocalState\\TabState", localappdata);

    dir = opendir(path);
    if (dir == NULL) {
        printf("\n[-] No such file or directory | Notepad instance(s) not found!\n");
        return 1;
    }

    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (isValidBinFile(entry->d_name)) {
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "%s\\%s", path, entry->d_name);

            // Check the fourth byte
            int fourthByte = checkFourthByte(filepath);
            if (fourthByte < 0) {
                // Error reading file, skip to the next one
                continue;
            }

            // Determine if the file is saved or unsaved
            const char *info = (fourthByte == 0) ? UNSAVED_MARKER : SAVED_MARKER;
            printf("\n[%d] %s (%s)\n\n", ++count, entry->d_name, info);

            FILE *file = fopen(filepath, "rb");
            if (file == NULL) {
                perror("\n[-] Error opening file");
                continue;
            }

            // Inspect the file based on its type
            if (fourthByte == 1) {
                inspectSavedFile(file, fourthByte);
            } else {
                inspectUnsavedFile(file);
            }

            fclose(file);
        }
    }

    closedir(dir);
    return 0;
}