#include <string.h>
#include <ti/screen.h>

const int BAUD_RATE = 115200;
const int NUM_COLS = 28;
const char CONFIRM_BYTE = 0x67; // haha
const char CONNECT_BYTE = 0x69; // even funnier the second time I do it!

bool isConnected = false;

void centerPrintText(const char str[], int row) {
    const int length = strlen(str);
    const int pos = (NUM_COLS - length) / 2;
    os_SetCursorPos(row, pos);
    os_PutStrFull(str);
}

void printText(const char str[], int row) {
    os_SetCursorPos(row, 0);
    os_PutStrFull(str);
}

void clearMenu() {
    os_ClrHome();

    const char message[] = "Welcome to Calc2MC!";
    centerPrintText(message, 0);
}