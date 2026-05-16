#include <string.h>
#include <ti/screen.h>

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