#include "tusb.h"

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESCRIPTOR_LEN + TUD_HID_DESC_LEN)
static tusb_desc_device_t const device_desc = {
    .bLength = sizeof(tusb_desc_device_t), .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200, .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON, .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE, .idVendor = 0xCafe,
    .idProduct = 0x4010, .bcdDevice = 0x0100, .iManufacturer = 1,
    .iProduct = 2, .iSerialNumber = 3, .bNumConfigurations = 1
};
uint8_t const *tud_descriptor_device_cb(void) { return (uint8_t const *)&device_desc; }
static uint8_t const config_desc[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x80, 100),
    TUD_HID_DESCRIPTOR(0, 4, HID_ITF_PROTOCOL_KEYBOARD, 1, 0x81, 8, 10)
};
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) { (void)index; return config_desc; }
static char const *strings[] = { (const char[]){0x09,0x04}, "jayis1", "Page Stepper", "0001", "Page keys" };
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    static uint16_t desc[32];
    uint8_t count = 0;
    if (index == 0) { desc[1] = 0x0409; count = 1; }
    else {
        if (index >= sizeof(strings)/sizeof(strings[0])) return NULL;
        const char *s = strings[index];
        while (s[count] && count < 31) { desc[1 + count] = (uint8_t)s[count]; ++count; }
    }
    desc[0] = (TUSB_DESC_STRING << 8) | (2 * count + 2);
    return desc;
}
uint8_t const hid_report_desc[] = { TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1)) };
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) { (void)instance; return hid_report_desc; }
