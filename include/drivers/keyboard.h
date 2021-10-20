#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <isr.h>
#include <types.h>

#define ACK 0xFA
#define RESEND 0xFE

#define LED 0xED
#define ENABLE_SCANNING 0xF4
#define SET_DEFAULT_PARAMETERS 0xF6
#define RESEND_LAST_BYTE 0xFE

#define KEYBOARD_CMD 0x64
#define KEYBOARD_STATUS 0x64
#define KEYBOARD_DATA 0x60

void keyboard_init(uint8_t i);
#endif // !__KEYBOARD_H__
