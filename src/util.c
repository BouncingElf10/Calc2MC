#include <string.h>
#include <ti/screen.h>

const int BAUD_RATE = 115200;
const int NUM_COLS = 28;

void centerPrintText(char str[]) {
    const int length = strlen(str);
    const int pos = (NUM_COLS - length) / 2;
    os_SetCursorPos(0, pos);
    os_PutStrFull(str);
}