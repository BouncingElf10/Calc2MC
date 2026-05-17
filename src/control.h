#ifndef CONTROL_H
#define CONTROL_H
#include <stdbool.h>

typedef enum {
    WAITING,
    HANDSHAKE,
    CONFIRMED,
    QUIT
} State;

extern State currentState;

char* stateToString(State state);

void handleWaiting();
void handleHandshake();
void handleConnected();

void fatal(const char *msg);
bool shouldQuit();

#endif