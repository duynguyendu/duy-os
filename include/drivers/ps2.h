#ifndef __PS2_H__
#define __PS2_H__

#include <types.h>
#include <stdbool.h>

/* Ports */
#define PS2_CMD 0x64
#define PS2_DATA 0x60

/* Commands */
#define DISABLE_FIRST_PORT 0xAD
#define DISABLE_SECOND_PORT 0xA7
#define ENABLE_FIRST_PORT 0xAE
#define ENABLE_SECOND_PORT 0xA8
#define CONTROLLER_CONFIG 0x20
#define WRITE_CONTROLLER_CONFIG 0x60
#define SELF_TEST 0xAA
#define FIRST_PORT_TEST 0xAB
#define SECOND_PORT_TEST 0xA9
#define RESET 0xFF
#define SEND_TO_SECOND_PORT 0xD4
#define DISABLE_SCANNING 0xF5
#define ENABLE_SCANNING 0xF4
#define IDENTIFY 0xF2

/* Return result */
#define SELF_TEST_PASS 0x55
#define PORT_PASS 0x00
#define RESET_SUCCESS 0xFA
#define PS2_ACK 0xFA
#define PS2_DEV_RESET_ACK 0xAA

/* Config big */
#define FIRST_PS2_PORT_INTERRUPT 1
#define SECOND_PS2_PORT_INTERRUPT 2
#define SYSTEM_FLAG 4
#define SHOULD_BE_ZERO 8
#define FIRST_PS2_CLOCK 16
#define SECOND_PS2_CLOCK 32
#define FIRST_PS2_TRANSLATION 64
#define MUST_BE_ZERO 128

/* Number of iterations to wait */
#define PS2_TIMEOUT 500

enum device_type {
    standar_ps2_mouse,
    mouse_with_scroll_wheel,
    mouse_button5,
    mf2_keyboard_with_translation,
    mf2_keyboard,
    unknown_device
};

/* Function */
#define PS2_SEND_CMD(byte) ps2_write(PS2_CMD, byte)
#define PS2_SEND_DATA(byte) ps2_write(PS2_DATA, byte)
#define PS2_READ_DATA() ps2_read(PS2_DATA)
#define PS2_READ_STATUS() ps2_read(PS2_CMD)

void ps2_init();
bool ps2_expect_ack();

uint8_t ps2_read(uint8_t port);
bool ps2_write(uint8_t port, uint8_t data);

void ps2_write_device(uint8_t device, uint8_t data);
uint8_t ps2_read_device(uint8_t device);

#endif // !__PS2_H__
