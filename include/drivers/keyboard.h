#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <isr.h>
#include <types.h>

#define LED 0xED
#define ENABLE_SCANNING 0xF4
#define SET_DEFAULT_PARAMETERS 0xF6
#define RESEND_LAST_BYTE 0xFE

#define KEY_RELEASE 0xF0
#define KEY_CONTROL_LEFT 0x14
#define KEY_SHIFT_LEFT 0x12
#define KEY_ALT_LEFT 0x11

#define CONTROL_MASK 1
#define SHIFT_MASK 2
#define ALT_MASK 4
#define RELEASE_MASK 8

void keyboard_init(uint8_t i);
void keyboard_set_scan_code_set(uint8_t scan_code_set);
#endif // !__KEYBOARD_H__
