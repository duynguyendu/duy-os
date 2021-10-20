#include <asm/interrupt.h>
#include <asm/ports.h>
#include <drivers/keyboard.h>
#include <drivers/ps2.h>
#include <stdbool.h>
#include <stdio.h>
// TODO mouse driver

static void ps2_emtpy_buffer();
static void ps2_switch_device_1();
static bool ps2_wait_read();
static bool ps2_wait_write();
static enum device_type identify_device(uint8_t first, uint8_t second);

void ps2_init() {
    bool devices[] = {true, true};
    printf("PS2 Initialize\n");
    CLI();

    /* Disable 2 channel */
    PS2_SEND_CMD(DISABLE_FIRST_PORT);
    PS2_SEND_CMD(DISABLE_SECOND_PORT);

    /* Flush the buffer, read data and ignore it */
    /* PS2_READ_DATA(); */
    // TODO 
    port_byte_in(PS2_DATA);

    /* Read config */
    PS2_SEND_CMD(CONTROLLER_CONFIG);
    uint8_t config = PS2_READ_DATA();

    if (!(config & SECOND_PS2_CLOCK)) {
        printf("[PS2] No device on port 2 found\n");
        devices[1] = false;
    }

    config |= SYSTEM_FLAG;
    if (config & MUST_BE_ZERO) {
        // TODO color error
        printf("[PS2] Invalid bit in controller configuration\n");
        config |= ~MUST_BE_ZERO;
    }

    /* Disable interrupt and scan code translation */
    config &= ~(FIRST_PS2_PORT_INTERRUPT | SECOND_PS2_PORT_INTERRUPT |
                FIRST_PS2_TRANSLATION);

    /* Write config back */
    PS2_SEND_CMD(WRITE_CONTROLLER_CONFIG);
    PS2_SEND_DATA(config);

    /* Self test */
    PS2_SEND_CMD(SELF_TEST);
    uint8_t self_test_result = PS2_READ_DATA();

    if (self_test_result != SELF_TEST_PASS) {
        // TODO color error
        printf("[PS2] Self test fail\n");
        devices[0] = false;
        devices[1] = false;

        return;
    } else {
        printf("[PS2] Self test pass\n");
    }

    /* Interface test */
    PS2_SEND_CMD(FIRST_PORT_TEST);
    uint8_t port_test_result = PS2_READ_DATA();
    if (port_test_result != PORT_PASS) {
        // TODO color error
        printf("[PS2] Port 1 failed\n");
        devices[0] = false;
    }

    PS2_SEND_CMD(SECOND_PORT_TEST);
    port_test_result = PS2_READ_DATA();
    if (port_test_result != PORT_PASS) {
        // TODO color error
        printf("[PS2] Port 2 failed\n");
        devices[1] = false;
    }

    /* Enable devices and reset */
    if (devices[0]) {
        printf("[PS2] Enable device 1\n");
        PS2_SEND_CMD(ENABLE_FIRST_PORT);

        ps2_write_device(0, RESET);
        uint8_t ack = PS2_READ_DATA();
        if (ack != RESET_SUCCESS || PS2_READ_DATA() != PS2_DEV_RESET_ACK) {
            // TODO color error
            printf("[PS2] Reset device 1 failed\n");
            devices[0] = false;
        } else {
            config |= FIRST_PS2_PORT_INTERRUPT;
        }
    }
    if (devices[1]) {
        printf("[PS2] Enable device 2\n");
        PS2_SEND_CMD(ENABLE_SECOND_PORT);

        ps2_write_device(1, RESET);
        uint8_t ack = PS2_READ_DATA();
        if (ack != RESET_SUCCESS || PS2_READ_DATA() != PS2_DEV_RESET_ACK) {
            // TODO color error
            printf("[PS2] Reset device 2 failed\n");
            devices[1] = false;
        } else {
            config |= SECOND_PS2_PORT_INTERRUPT;
        }

        /* Sometimes mouse also send another byte */
        ps2_emtpy_buffer();
    }

    /* Write config back */
    PS2_SEND_CMD(WRITE_CONTROLLER_CONFIG);
    PS2_SEND_DATA(config);

    /* Init drivers */
    for (uint8_t i = 0; i < 2; i++) {
        if (devices[i]) {
            ps2_write_device(i, DISABLE_SCANNING);
            ps2_expect_ack();
            ps2_write_device(i, IDENTIFY);
            ps2_expect_ack();
            uint8_t first = PS2_READ_DATA();
            uint8_t second = PS2_READ_DATA();

            switch (identify_device(first, second)) {
            case standar_ps2_mouse:
            case mouse_with_scroll_wheel:
            case mouse_button5:
                printf("[PS2] Mouse not supported yet\n");
                devices[i] = false;
                break;
            case mf2_keyboard_with_translation:
            case mf2_keyboard:
                printf("[PS2] Keyboard\n");
                keyboard_init(i);
                ps2_write_device(i, ENABLE_SCANNING);
                break;
            case unknown_device:
                printf("[PS2] unknown_device\n");
            }
        }
    }
    STI();
}

bool ps2_expect_ack() {
    uint8_t status = PS2_READ_DATA();
    if (!(status == PS2_ACK)) {
        printf("[PS2] Failed to acknowledge command\n");
        return false;
    }
    return true;
}

static enum device_type identify_device(uint8_t first, uint8_t second) {
    if (first == 0x00) {
        return standar_ps2_mouse;
    } else if (first == 0x03) {
        return mouse_with_scroll_wheel;
    } else if (first == 0x04) {
        return mouse_button5;
    } else if (first == 0xAB) {
        if (second == 0x41 || second == 0xC1) {
            return mf2_keyboard_with_translation;
        } else {
            return mf2_keyboard;
        }
    } else {
        return unknown_device;
    }
}

static void ps2_emtpy_buffer() {
    uint8_t buffer = PS2_READ_STATUS();
    if (buffer & 0x1) {
        PS2_READ_DATA();
    }
}

static void ps2_switch_device_1() { PS2_SEND_CMD(SEND_TO_SECOND_PORT); }

uint8_t ps2_read(uint8_t port) {
    uint16_t timer = PS2_TIMEOUT;
    /* While output buffer empty */
    while (!(port_byte_in(PS2_CMD) & 1) && timer > 0) {
        timer--;
        __asm__("pause");
    }
    if (timer == 0) {
        printf("[PS2] Read failed\n");
        return -1;
    } else {
        return port_byte_in(port);
    }
}

bool ps2_write(uint8_t port, uint8_t data) {
    uint16_t timer = PS2_TIMEOUT;
    /* While input buffer full */
    while ((port_byte_in(PS2_CMD) & 2) && timer > 0) {
        timer--;
        __asm__("pause");
    }
    if (timer == 0) {
        printf("[PS2] Write failed\n");
        return false;
    } else {
        port_byte_out(port, data);
        return true;
    }
}

void ps2_write_device(uint8_t device, uint8_t data) {
    if (device != 0) {
        ps2_switch_device_1();
    }
    PS2_SEND_DATA(data);
}

uint8_t ps2_read_device(uint8_t device) {
    if (device != 0) {
        ps2_switch_device_1();
    }
    return PS2_READ_DATA();
}
