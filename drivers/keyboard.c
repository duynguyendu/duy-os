#include <asm/ports.h>
#include <drivers/keyboard.h>
#include <drivers/ps2.h>
// TODO remove when done debugging
#include <isr.h>
#include <stdio.h>
#include <string.h>
#include <tty.h>

uint8_t dev_id = 0;

#define SEND_CMD(byte) ps2_write_device(dev_id, byte)
#define SEND_DATA(byte) ps2_write_device(dev_id, byte)
#define READ_KEY() ps2_read_device(dev_id)

#define IS_RELEASE() mask &RELEASE_MASK
#define SET_RELEASE() mask |= RELEASE_MASK
#define UNSET_RELEASE() mask &= ~RELEASE_MASK

#define SET_CONTROL() mask |= CONTROL_MASK
#define UNSET_CONTROL() mask &= ~CONTROL_MASK

#define IS_SHIFT() mask &SHIFT_MASK
#define SET_SHIFT() mask |= SHIFT_MASK
#define UNSET_SHIFT() mask &= ~SHIFT_MASK

#define SET_ALT() mask |= ALT_MASK
#define UNSET_ALT() mask &= ~ALT_MASK

char translate_key_table[] = {
    0,   'q', '1',  0,   0,   0,   'z', 's', 'a', 'w', '2',  0,   0,   'c', 'x',
    'd', 'e', '4',  '3', 0,   0,   ' ', 'v', 'f', 't', 'r',  '5', 0,   0,   'n',
    'b', 'h', 'g',  'y', '6', 0,   0,   0,   'm', 'j', 'u',  '7', '8', 0,   0,
    ',', 'k', 'i',  'o', '0', '9', 0,   0,   '.', '/', 'l',  ';', 'p', '-', 0,
    0,   0,   '\'', 0,   '[', '=', 0,   0,   0,   0,   '\n', ']', 0,   '\\'};

char translate_key_table_shift[] = {
    0,   'Q', '!', 0,   0,   0,   'Z', 'S', 'A', 'W', '@',  0,   0,   'C', 'X',
    'D', 'E', '$', '#', 0,   0,   ' ', 'V', 'F', 'T', 'R',  '%', 0,   0,   'N',
    'B', 'H', 'G', 'Y', '^', 0,   0,   0,   'M', 'J', 'U',  '&', '*', 0,   0,
    '<', 'K', 'I', 'O', ')', '(', 0,   0,   '>', '?', 'L',  ':', 'P', '_', 0,
    0,   0,   '"', 0,   '{', '+', 0,   0,   0,   0,   '\n', '}', 0,   '|'};

/* 0: Control, 1: Shift, 2: Alt, 3: Released */
// TODO left, right modify key
uint8_t mask = 0;
void keyboard_interrupt_handler(registers_t r) {
    // TODO buffering
    uint8_t keycode = READ_KEY();
    if (keycode == KEY_RELEASE) {
        SET_RELEASE();
    } else if (keycode == KEY_CONTROL_LEFT) {
        if (IS_RELEASE()) {
            UNSET_CONTROL();
        } else {
            SET_CONTROL();
        }
    } else if (keycode == KEY_SHIFT_LEFT) {
        if (IS_RELEASE()) {
            UNSET_SHIFT();
        } else {
            SET_SHIFT();
        }
    } else if (keycode == KEY_ALT_LEFT) {
        if (IS_RELEASE()) {
            UNSET_ALT();
        } else {
            SET_ALT();
        }
    } else if (0x15 <= keycode && keycode <= 0x5D) {
        if (IS_RELEASE()) {
            UNSET_RELEASE();
        } else {
            char key;
            if (IS_SHIFT()) {
                key = translate_key_table_shift[keycode - 0x14];
            } else {
                key = translate_key_table[keycode - 0x14];
            }
            send_eoi(r);
            recieve_key(key, mask);
        }
    }
    send_eoi(r);
}

void keyboard_init(uint8_t i) {
    dev_id = i;
    // TODO Change code set to 2
    printf("[Keyboard] Starting keyboard driver\n");
    printf("[Keyboard] Switching to scan code set 2\n");
    SEND_CMD(ENABLE_SCANNING);

    keyboard_set_scan_code_set(0x2);
    register_interrupt_handler(IRQ1, keyboard_interrupt_handler);
}

void keyboard_set_scan_code_set(uint8_t scan_code_set) {
    SEND_CMD(0xF0);
    READ_KEY();
    SEND_DATA(scan_code_set);
    READ_KEY();
}
