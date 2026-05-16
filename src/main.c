#include "util.h"
#include "control.h"
#include "device.h"
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <srldrvce.h>
#include <stdbool.h>
#include <usbdrvce.h>

int main(void) {
    clearMenu();

    const usb_standard_descriptors_t *desc = srl_GetCDCStandardDescriptors();
    usb_Init(usb_handler, NULL, desc, USB_DEFAULT_INIT_FLAGS);

    while (true) {
        usb_HandleEvents();

        const char *status = stateToString(currentState);
        printText(status, 2);

        if (os_GetCSC() == sk_Clear) {
            fatal("user quit");
            return 0;
        }
        if (!has_srl_device && isConnected) {
            fatal("device disconnected");
            return 1;
        }
        if (shouldQuit()) {
            fatal("client disconnected");
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
