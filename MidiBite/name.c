// To give the device a unique name, this code must be in a stand-alone .c file.

#include "usb_names.h"

// Edit these lines to create your own name.  The length must
// match the number of characters in your custom name.

#define MIDI_NAME { 'M', 'i', 'd', 'i', 'B', 'i', 't', 'e' }
#define MIDI_NAME_LEN 8

// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};
