#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>

extern int BAUD_RATE;
extern char CONFIRM_BYTE;
extern char CONNECT_BYTE;

extern bool isConnected;

void centerPrintText(const char str[], int row);
void printText(const char str[], int row);
void clearMenu();

#endif