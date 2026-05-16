#ifndef DEVICE_H
#define DEVICE_H

#include "device.h"
#include <srldrvce.h>
#include <stdbool.h>
#include <usbdrvce.h>

extern bool has_srl_device;
extern srl_device_t srl;

usb_error_t usb_handler(usb_event_t event, void *event_data, usb_callback_data_t *callback_data __attribute__((unused)));

#endif