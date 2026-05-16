#include "control.h"
#include "device.h"
#include "util.h"
#include <usbdrvce.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdbool.h>
#include <tice.h>
#include <time.h>

State currentState = WAITING;

const char *stateToString(const State state) {
    switch (state) {
        case WAITING: return "Waiting for USB to be connected...";
        case HANDSHAKE: return "Connected! Waiting to connect to Minecraft...";
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

void handleConnected() {
    const uint8_t keyInt = os_GetCSC();
    if (!keyInt) return;
    if (keyInt == sk_Clear) fatal("user quit");

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

