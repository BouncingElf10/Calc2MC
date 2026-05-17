#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ti/screen.h>

#include "control.h"

const int BAUD_RATE = 115200;
const int NUM_COLS = 28;
const char CONFIRM_BYTE = 0x67; // haha
const char CONNECT_BYTE = 0x69; // even funnier the second time I do it!
const char QUIT_BYTE = 0x42; // fuck i ran out of them

bool isConnected = false;

void centerPrintText(const char str[], int row) {
    const int length = strlen(str);
    const int pos = (NUM_COLS - length) / 2;
    os_SetCursorPos(row, pos);
    os_PutStrFull(str);
}

void printText(const char *str, int row) { // yeah ts was gippty, aint bothered to write word wrapping
    int col = 0;

    while (*str) {
        const char *word = str;
        int len = 0;

        while (str[len] && str[len] != ' ')len++;

        if (col + len > NUM_COLS) { row++; col = 0; }

        char temp[29];
        memcpy(temp, word, len);
        temp[len] = '\0';

        os_SetCursorPos(row, col);
        os_PutStrFull(temp);

        col += len;
        str += len;

        if (*str == ' ') {
            col++;
            str++;
        }
    }
}

void clearMenu() {
    os_ClrHome();

    const char message[] = "Welcome to Calc2MC!";
    centerPrintText(message, 0);
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* getMovingDots() {
    const int index = (int) floor(clock() / CLOCKS_PER_SEC) % 4;
    switch (index) {
        case 0: return "   ";
        case 1: return ".  ";
        case 2: return ".. ";
        case 3: return "...";
        default: return "";
    }
}

void printInfoText() {
    if (currentState != CONFIRMED) {
        static bool hasCleared = false;
        const char *dots = getMovingDots();
        if (strcmp(dots, "   ") == 0 && !hasCleared) {
            clearMenu();
            hasCleared = true;
        } else if (strcmp(dots, "   ") != 0) {
            hasCleared = false;
        }

        const char *status = stateToString(currentState);
        char *text = concat(status, dots);
        printText(text, 2);
        free(text);
    } else {
        const char* status = stateToString(currentState);
        printText(status, 2);
    }
}