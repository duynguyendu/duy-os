#include <asm/ports.h>
#include <drivers/vga.h>
#include <memory.h>

static inline uint16_t get_screen_offset(uint8_t row, uint8_t col);
uint16_t get_cursor();
void set_cursor(uint16_t offset);
uint16_t handle_scrolling(uint16_t offset);
uint8_t get_row(uint16_t offset);
uint8_t get_col(uint16_t offset);

void vga_put_at(char character, int row, int col, uint8_t attribute_byte) {
    uint16_t *vidmem = (uint16_t *)VIDEO_MEMORY;

    if (!attribute_byte) {
        attribute_byte = vga_color_entry(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    }

    uint16_t offset;
    if (row >= 0 && col >= 0) {
        offset = get_screen_offset(row, col);
    } else {
        offset = get_cursor();
    }

    /* If we see newline, set offset to the end of current row
    So the next one will advance to the first col of next line */
    if (character == '\n') {
        offset = get_screen_offset(get_row(offset) + 1, 0);
    } else {
        vidmem[offset] = vga_entry(character, attribute_byte);
        offset += 1;
    }

    if (offset >= get_screen_offset(MAX_ROW, MAX_COL)) {
        offset = handle_scrolling(offset);
    }

    set_cursor(offset);
}

static inline uint16_t get_screen_offset(uint8_t row, uint8_t col) {
    return (uint16_t)((uint16_t)row * MAX_COL + col);
}

uint16_t get_cursor() {
    /* Reg 14 stores high bytes
    Reg 15 stores low bytes */
    port_byte_out(REG_SCREEN_CTRL, 14);
    uint16_t offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset;
}

uint8_t get_row(uint16_t offset) { return offset / MAX_COL; }

uint8_t get_col(uint16_t offset) { return offset % MAX_COL; }

void set_cursor(uint16_t offset) {
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

// TODO
uint16_t handle_scrolling(uint16_t offset) {
    uint8_t row = get_row(offset);
    uint16_t *video_mem = (uint16_t *)VIDEO_MEMORY;
    for (uint8_t i = 0; i < row; i++) {
        /* Move each row up */
        memcpy(video_mem, video_mem + MAX_COL,
               MAX_COL * 2);
        video_mem += MAX_COL;
    }
    offset -= MAX_COL;
    /* Clear the rest */
    memset(video_mem + offset, 0,
           MAX_COL * MAX_ROW * 2 - offset * 2);
    return offset;
}

void vga_print_at(char *message, int row, int col) {
    if (row >= 0 && col >= 0) {
        set_cursor(get_screen_offset(row, col));
    }

    int i = 0;
    while (message[i] != '\0') {
        vga_put_at(message[i], row, col, 0);
        i++;
    }
}

void vga_put(char c) { vga_put_at(c, -1, -1, 0); }

void vga_print(char *message) { vga_print_at(message, -1, -1); }

void vga_clear_screen() {
    int screen_size = MAX_COL * MAX_ROW;
    uint16_t *screen = (uint16_t *)VIDEO_MEMORY;
    uint8_t color = vga_color_entry(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    /* Iterate through all cells and set it to space */
    for (int i = 0; i < screen_size; i++) {
        screen[i] = vga_entry(' ', color);
    }
    /* Reset cursor to beginning */
    set_cursor(get_screen_offset(0, 0));
}
