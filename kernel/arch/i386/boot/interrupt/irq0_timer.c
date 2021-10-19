#include <asm/ports.h>
#include <idt.h>
#include <interrupt/irq0_timer.h>
#include <isr.h>
#include <stdio.h>
#include <string.h>

uint32_t tick = 0;

static void timer_callback(registers_t r) {
    tick++;
    printf("Tick ");
    char temp_tick[20];
    int_to_ascii(tick, temp_tick);
    printf(temp_tick);
    printf("\n");
    send_eoi(r);
}

void init_timer(uint32_t frequency) {
    register_interrupt_handler(IRQ0, timer_callback);
    uint32_t divisor = 1193180 / frequency;

    port_byte_out(PIT_CMD, REPEAT_MODE);

    /* Lower byte */
    port_byte_out(PIT_DATA0, (uint8_t)divisor & 0xFF);
    /* Higher byte */
    port_byte_out(PIT_DATA0, (uint8_t)(divisor >> 8) & 0xFF);
}
