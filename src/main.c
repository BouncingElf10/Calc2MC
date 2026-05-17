#include "util.h"
#include "control.h"
#include "device.h"
#include <ti/getcsc.h>
#include <srldrvce.h>
#include <stdbool.h>
#include <stdlib.h>
#include <usbdrvce.h>

int main(void) {
    clearMenu();

    const usb_standard_descriptors_t *desc = srl_GetCDCStandardDescriptors();
    usb_Init(usb_handler, NULL, desc, USB_DEFAULT_INIT_FLAGS);

    while (true) {
        usb_HandleEvents();

        printInfoText();

        if (os_GetCSC() == sk_Clear) {
            fatal("User has quit the program.");
            return 0;
        }
        if (!has_srl_device && isConnected) {
            fatal("Device has disconnected.");
            return 1;
        }
        if (currentState == CONFIRMED && has_srl_device && shouldQuit()) {
            fatal("Client has disconnected.");
            return 1;
        }

        switch (currentState) {
            case WAITING: handleWaiting(); break;
            case HANDSHAKE: handleHandshake(); break;
            case CONFIRMED: handleConnected(); break;
            case QUIT: return 0;
        }
    }
}
