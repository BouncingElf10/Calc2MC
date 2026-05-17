#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>

extern int BAUD_RATE;
extern char CONFIRM_BYTE;
extern char CONNECT_BYTE;
extern char QUIT_BYTE;

extern int SCREEN_COLS;
extern int SCREEN_ROWS;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern bool isConnected;

void centerPrintText(const char str[], int row);
void printText(const char str[], int row);
void clearMenu();
char* concat(const char *s1, const char *s2);
char* getMovingDots();
void printInfoText();

#endif