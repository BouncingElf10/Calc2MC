#include "util.h"
#include "device.h"
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <srldrvce.h>
#include <stdbool.h>
#include <tice.h>
#include <usbdrvce.h>

int main(void){
    os_ClrHome();

    char message[] = "Hello, World!";
    centerPrintText(message);

    os_ClrHome();
    os_PutStrFull("waiting for device...");

    const usb_standard_descriptors_t *desc = srl_GetCDCStandardDescriptors();
    usb_Init(usb_handler, NULL, desc, USB_DEFAULT_INIT_FLAGS);

    while (!has_srl_device) {
        usb_HandleEvents();
        if (os_GetCSC() == sk_Clear) {
            usb_Cleanup();
            return 0;
        }
    }

    os_ClrHome();
    os_PutStrFull("connected! press any key to send...");
    while (!os_GetCSC()) {
        usb_HandleEvents();
    }

    uint8_t data[] = "Hello World!";
    srl_Write(&srl, data, sizeof(data) - 1);

    for (int i = 0; i < 10000; i++) {
        usb_HandleEvents();
    }

    os_ClrHome();
    os_PutStrFull("sent");
    while (!os_GetCSC()) {
        usb_HandleEvents();
    }

    usb_Cleanup();
    return 0;
}