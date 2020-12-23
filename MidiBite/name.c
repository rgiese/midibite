// To give the device a unique name, this code must be in a stand-alone .c file.

#include "usb_names.h"

struct usb_string_descriptor_struct usb_string_product_name = {
    (8 + 1) * 2,  // size of L"MidiBite" including terminator as uint16_t wide string
    3,
    {'M', 'i', 'd', 'i', 'B', 'i', 't', 'e'}};
