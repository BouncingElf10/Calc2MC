#include "control.h"
#include "device.h"
#include "util.h"
#include <usbdrvce.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdbool.h>
#include <tice.h>
#include <time.h>
#include <keypadc.h>

State currentState = WAITING;
bool hasMadeInput = false;
bool displayMessage = true;
uint8_t lastKeyPressed = 0;

char *stateToString(const State state) {
    switch (state) {
        case WAITING: return "Waiting for USB to be connected";
        case HANDSHAKE: return "Connected! Waiting to connect to Minecraft";
        case CONFIRMED: return "Everything connected successfully!";
        case QUIT: return "";
    }
    return "Something went wrong!";
}

void handleWaiting() {
    if (has_srl_device) {
        currentState = HANDSHAKE;
        isConnected = true;
        clearMenu();
    }
}

void handleHandshake() {
    static uint32_t last_send = 0;

    uint8_t byte;
    if (srl_Read(&srl, &byte, 1) == 1) {
        if (byte == CONFIRM_BYTE) {
            currentState = CONFIRMED;
            clearMenu();
            return;
        }
    }

    uint32_t now = clock();
    if (now - last_send >= CLOCKS_PER_SEC) {
        last_send = now;
        uint8_t connect_packet[] = { CONNECT_BYTE };
        srl_Write(&srl, connect_packet, sizeof(connect_packet));
    }
}

bool getKeyEvent(uint8_t *key, bool *pressed) {
    kb_Scan();
    const uint8_t keyInt = os_GetCSC();

    if (keyInt && lastKeyPressed != keyInt) {
        *key = keyInt;
        *pressed = true;
        lastKeyPressed = keyInt;
        return true;
    }

    if (lastKeyPressed != 0) {
        const kb_lkey_t lkey = skToKbKey(lastKeyPressed);
        if (!lkey || !kb_IsDown(lkey)) {
            *key = lastKeyPressed;
            *pressed = false;
            lastKeyPressed = 0;
            return true;
        }
    }

    return false;
}

void handleConnected() {
    uint8_t keyInt;
    bool pressed;
    if (!getKeyEvent(&keyInt, &pressed)) return;

    if (keyInt == sk_Clear) fatal("User has quit the program.");

    clearMenu();
    if (hasMadeInput == false) hasMadeInput = true;
    if (!pressed) {
        keyInt = keyInt | 0b10000000; // depressed bit
    }

    const uint8_t key[] = { keyInt };
    srl_Write(&srl, key, sizeof(key));
}


void fatal(const char *msg) {
    os_ClrHome();
    printText("ERROR:", 0);
    printText(msg, 1);
    printText("Press clear to quit.", 3);
    while (os_GetCSC() != sk_Clear) usb_HandleEvents();

    currentState = QUIT;
    usb_Cleanup();
}

bool shouldQuit() {
    uint8_t byte;
    if (srl_Read(&srl, &byte, 1) == 1) {
        if (byte == QUIT_BYTE) {
            return true;
        }
    }
    return false;
}

