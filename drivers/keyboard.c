#include <drivers/keyboard.h>
#include <asm/ports.h>
#include <drivers/ps2.h>
// TODO remove when done debugging
#include <string.h>
#include <stdio.h>
#include <tty.h>
#include <isr.h>

#define SEND_CMD(byte) port_byte_out(KEYBOARD_CMD, byte)
#define SEND_DATA(byte) port_byte_out(KEYBOARD_DATA, byte)
#define READ_KEY() port_byte_in(KEYBOARD_DATA)

uint8_t last_cmd = 0;
char translate_to_ascii(uint8_t key);

void keyboard_interrupt_handler(registers_t r) {
    // TODO buffering
    uint8_t keycode = READ_KEY();
    if (keycode >= 0) {
        send_eoi(r);
        char temp[5];
        int_to_ascii(keycode, temp);
        printf(temp);
        recieve_key(translate_to_ascii(keycode));
    }
}

void keyboard_init(uint8_t i) {
    // TODO Change code set to 2
    printf("[Keyboard] Starting keyboard driver\n");
    register_interrupt_handler(IRQ1, keyboard_interrupt_handler);
}

char translate_to_ascii(uint8_t key) {
    // TODO scan code 3
    return 'a';
}
