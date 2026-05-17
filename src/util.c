#include <keypadc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ti/getcsc.h>
#include <ti/screen.h>
#include "control.h"

const int BAUD_RATE = 115200;

const int SCREEN_COLS = 28;
const int SCREEN_ROWS = 9;
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

const char CONFIRM_BYTE = 0x67; // haha
const char CONNECT_BYTE = 0x69; // even funnier the second time I do it!
const char QUIT_BYTE = 0x42; // fuck i ran out of them

bool isConnected = false;

void centerCenterText(const char str[], int yOffset) {
    os_FontSelect(os_LargeFont);
    int x = (SCREEN_WIDTH - os_FontGetWidth(str)) / 2;
    int y = (SCREEN_HEIGHT - os_FontGetHeight()) / 2;
    os_FontDrawText(str, x, y + yOffset);
}

void centerPrintText(const char str[], int row) {
    const int length = strlen(str);
    const int pos = (SCREEN_COLS - length) / 2;
    os_SetCursorPos(row, pos);
    os_PutStrFull(str);
}

void printText(const char *str, int row) { // yeah ts was gippty, aint bothered to write word wrapping
    int col = 0;

    while (*str) {
        const char *word = str;
        int len = 0;

        while (str[len] && str[len] != ' ')len++;

        if (col + len > SCREEN_COLS) { row++; col = 0; }

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

const char *keyToString(uint8_t key) {
    switch (key) {
        case sk_Down: return "Down";
        case sk_Left: return "Left";
        case sk_Right: return "Right";
        case sk_Up: return "Up";

        case sk_Enter: return "Enter";
        case sk_2nd: return "2nd";
        case sk_Clear: return "Clear";
        case sk_Alpha: return "Alpha";

        case sk_Add: return "+";
        case sk_Sub: return "-";
        case sk_Mul: return "*";
        case sk_Div: return "/";

        case sk_Graph: return "Graph";
        case sk_Trace: return "Trace";
        case sk_Zoom: return "Zoom";
        case sk_Window: return "Window";
        case sk_Yequ: return "Y=";

        case sk_Mode: return "Mode";
        case sk_Del: return "Delete";

        case sk_Store: return "Sto->";
        case sk_Ln: return "Ln";
        case sk_Log: return "Log";
        case sk_Square: return "x^2";
        case sk_Recip: return "x^-1";
        case sk_Math: return "Math";

        case sk_0: return "0";
        case sk_1: return "1";
        case sk_2: return "2";
        case sk_3: return "3";
        case sk_4: return "4";
        case sk_5: return "5";
        case sk_6: return "6";
        case sk_7: return "7";
        case sk_8: return "8";
        case sk_9: return "9";

        case sk_Comma: return ",";
        case sk_Sin: return "Sin";
        case sk_Apps: return "Apps";
        case sk_GraphVar: return "X,T,theta,n";

        case sk_DecPnt: return ".";
        case sk_LParen: return "(";
        case sk_Cos: return "Cos";
        case sk_Prgm: return "Prgm";
        case sk_Stat: return "Stat";

        case sk_Chs: return "(-)";
        case sk_RParen: return ")";
        case sk_Tan: return "Tan";
        case sk_Vars: return "Vars";
        case sk_Power: return "^";

        default: return "None";
    }
}

kb_lkey_t skToKbKey(const uint8_t sk) {
    switch (sk) {
        case sk_Down: return kb_KeyDown;
        case sk_Left: return kb_KeyLeft;
        case sk_Right: return kb_KeyRight;
        case sk_Up: return kb_KeyUp;

        case sk_Enter: return kb_KeyEnter;
        case sk_2nd: return kb_Key2nd;
        case sk_Clear: return kb_KeyClear;
        case sk_Alpha: return kb_KeyAlpha;

        case sk_Add: return kb_KeyAdd;
        case sk_Sub: return kb_KeySub;
        case sk_Mul: return kb_KeyMul;
        case sk_Div: return kb_KeyDiv;

        case sk_Graph: return kb_KeyGraph;
        case sk_Trace: return kb_KeyTrace;
        case sk_Zoom: return kb_KeyZoom;
        case sk_Window: return kb_KeyWindow;
        case sk_Yequ: return kb_KeyYequ;

        case sk_Mode: return kb_KeyMode;
        case sk_Del: return kb_KeyDel;

        case sk_Store: return kb_KeySto;
        case sk_Ln: return kb_KeyLn;
        case sk_Log: return kb_KeyLog;
        case sk_Square: return kb_KeySquare;
        case sk_Recip: return kb_KeyRecip;
        case sk_Math: return kb_KeyMath;

        case sk_0: return kb_Key0;
        case sk_1: return kb_Key1;
        case sk_2: return kb_Key2;
        case sk_3: return kb_Key3;
        case sk_4: return kb_Key4;
        case sk_5: return kb_Key5;
        case sk_6: return kb_Key6;
        case sk_7: return kb_Key7;
        case sk_8: return kb_Key8;
        case sk_9: return kb_Key9;

        case sk_Comma: return kb_KeyComma;
        case sk_Sin: return kb_KeySin;
        case sk_Apps: return kb_KeyApps;
        case sk_GraphVar: return kb_KeyGraphVar;

        case sk_DecPnt: return kb_KeyDecPnt;
        case sk_LParen: return kb_KeyLParen;
        case sk_Cos: return kb_KeyCos;
        case sk_Prgm: return kb_KeyPrgm;
        case sk_Stat: return kb_KeyStat;

        case sk_Chs: return kb_KeyChs;
        case sk_RParen: return kb_KeyRParen;
        case sk_Tan: return kb_KeyTan;
        case sk_Vars: return kb_KeyVars;
        case sk_Power: return kb_KeyPower;

        default: return 0;
    }
}

void printSmallText(const char *str, int y) {
    os_FontSelect(os_SmallFont);
    os_FontDrawText(str, 0, y);
}

void clearMenu() {
    os_ClrHome();

    const char message[] = "Welcome to Calc2MC!";
    centerPrintText(message, 0);

    printSmallText("Made   by   BouncingElf10", SCREEN_HEIGHT - os_FontGetHeight());
}

char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *getMovingDots() {
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
    if (hasMadeInput && currentState == CONFIRMED) {
        os_FontSelect(os_LargeFont);
        centerCenterText(keyToString(lastKeyPressed), os_FontGetHeight());
    }

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
        // I have no FUCKING idea why this slows down the program
        // ADDITIONALLY if the sting is empty, it just fucking kills itself
        // so you're just gonna have to deal with the text ig.

        // if (hasMadeInput) {
        //     printText("Keypress:", 2);
        //     return;
        // }
        printText(stateToString(currentState), 2);
    }
}