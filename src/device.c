#include <srldrvce.h>
#include <stdbool.h>
#include <usbdrvce.h>

#include "util.h"

bool has_srl_device = false;
srl_device_t srl;
static uint8_t srl_buf[512];

usb_error_t usb_handler(usb_event_t event, void *event_data, usb_callback_data_t *callback_data __attribute__((unused))) {
    usb_error_t err;
    if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS)
        return err;

    if (event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE)) {
        usb_ResetDevice(event_data);
    }

    if (event == USB_HOST_CONFIGURE_EVENT ||(event == USB_DEVICE_ENABLED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))) {
        if (has_srl_device) return USB_SUCCESS;

        const usb_device_t device = event == USB_HOST_CONFIGURE_EVENT ? usb_FindDevice(NULL, NULL, USB_SKIP_HUBS): (usb_device_t) event_data;
        if (!device) return USB_SUCCESS;
        if (srl_Open(&srl, device, srl_buf, sizeof srl_buf, SRL_INTERFACE_ANY, BAUD_RATE)) return USB_SUCCESS;
        has_srl_device = true;
    }

    if (event == USB_DEVICE_DISCONNECTED_EVENT) {
        if ((usb_device_t)event_data == srl.dev) {
            srl_Close(&srl);
            has_srl_device = false;
        }
    }

    return USB_SUCCESS;
}