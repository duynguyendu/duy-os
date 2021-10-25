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
#define KEY_BACKSPACE 0x66

#define KEY_F1 0x05
#define KEY_F2 0x06
#define KEY_F3 0x04
#define KEY_F4 0x0C
#define KEY_F5 0x03
#define KEY_F6 0x0B
#define KEY_F7 0x83
#define KEY_F8 0x0A
#define KEY_F9 0x01
#define KEY_F10 0x09
#define KEY_F11 0x78
#define KEY_F12 0x07
enum special_key { F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12 };

#define CONTROL_MASK 1
#define SHIFT_MASK 2
#define ALT_MASK 4
#define RELEASE_MASK 8

void keyboard_init(uint8_t i);
void keyboard_set_scan_code_set(uint8_t scan_code_set);
#endif // !__KEYBOARD_H__
