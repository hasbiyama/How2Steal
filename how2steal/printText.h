/*
    author: 
    hasbiyama (@3xploitZero)
    github.com/hasbiyama

*/

#include "noteSnitch\noteSnitch.h"

// Function to print text from a window
void printText(HWND hwnd, const char* windowTitle, int windowNumber, int* notepadTextBoxCount) {
    // Get the length of the text in the window
    int textLength = SendMessageA(hwnd, WM_GETTEXTLENGTH, 0, 0);

    // If there's no text and this is not the first window, exit
    if (textLength == 0) {
        if (*notepadTextBoxCount != 0) {
            noteSnitch(); // Function call indicating a special case
            exit(0);
        }
        return;
    }

    // Allocate memory to store the text
    char* text = (char*)malloc((textLength + 1) * sizeof(char));
    if (text == NULL) {
        printf("Memory allocation failed.\n"); // Inform if memory allocation fails
        return;
    }

    // Get the text from the window
    SendMessageA(hwnd, WM_GETTEXT, textLength + 1, (LPARAM)text);
    int calculatedLength = textLength; // Length of the text

    // Print the text along with window information
    if (*notepadTextBoxCount == 0) {
        // If it's the first window
        printf("\n[%d] %s (%d characters):\n\n%s\n\n", windowNumber, windowTitle, calculatedLength, text);
    } else {
        // If it's not the first window
        printf("\n[%d] %s [[__OPENED_TAB__]] (%d characters):\n\n%s\n\n", *notepadTextBoxCount, windowTitle, calculatedLength, text);
    }

    // Free the allocated memory for text
    free(text);
}