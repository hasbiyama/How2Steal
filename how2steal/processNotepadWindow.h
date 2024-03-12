/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "printText.h"

// Function to process a Notepad window
void processNotepadWindow(HWND notepadWnd, int* windowNumber, int* notepadTextBoxCount) {
    char windowTitle[MAX_PATH];
    GetWindowTextA(notepadWnd, windowTitle, sizeof(windowTitle));

    HWND editWnd = FindWindowExA(notepadWnd, NULL, "Edit", NULL);
    if (editWnd != NULL) {
        // If "Edit" control is found, print the text
        (*windowNumber)++;
        (*notepadTextBoxCount) = 0;
        printText(editWnd, windowTitle, *windowNumber, notepadTextBoxCount);
    } else {
        // If "Edit" control is not found, try finding "RichEditD2DPT" under "NotepadTextBox" class
        HWND notepadTextBoxWnd = FindWindowExA(notepadWnd, NULL, "NotepadTextBox", NULL);
        while (notepadTextBoxWnd != NULL) {
            // Count the number of "NotepadTextBox" instances
            (*notepadTextBoxCount)++;
            HWND richEditWnd = FindWindowExA(notepadTextBoxWnd, NULL, "RichEditD2DPT", NULL);
            if (richEditWnd != NULL) {
                // If "RichEditD2DPT" control is found, print the text
                (*windowNumber)++;
                printText(richEditWnd, windowTitle, *windowNumber, notepadTextBoxCount);
            }
            // Find the next "NotepadTextBox" instance
            notepadTextBoxWnd = FindWindowExA(notepadWnd, notepadTextBoxWnd, "NotepadTextBox", NULL);
        }
    }
}