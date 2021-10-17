#include "screen.h"
#include "drivers/low_level.h"

/* Print a character at row, col or at cursor current position */
void print_char(char character, int row, int col, char attribute_byte) {
    /* Pointer to the start of screen */
    unsigned char *vidmem = (unsigned char *)VIDEO_MEMORY;

    /* Foreground and Background, use default if unspecify */
    if (!attribute_byte) {
        attribute_byte = (char)WHITE_ON_BLACK;
    }

    /* Video memory offset for screen location */
    unsigned int offset;
    /* Calculate offset based on row, col or use the current cursor position */
    if (row >= 0 && col >= 0) {
        offset = get_screen_offset(row, col);
    } else {
        offset = get_cursor();
    }

    /* If we see newline, set offset to the end of current row
    So the next one will advance to the first col of next line */
    if (character == '\n') {
        int rows = offset / (2 * MAX_COL); // Multiple by 2 because each cell
                                           // requires 2 bytes for a character
        offset = get_screen_offset(rows, 79);
    } else {
        /* First byte is the character, second byte is the color */
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
        offset += 2;
    }

    /* Update date offset to next cell */
    /* Make scrolling when we reach bottom of screen */
    // TODO
    /* offset = handle_scrolling(offset); */
    /* Update cursor position */
    set_cursor(offset);
}

unsigned int get_screen_offset(int row, int col) {
    return (unsigned int)(row * MAX_COL + col) * 2;
}

unsigned int get_cursor() {
    /* Reg 14 stores high bytes
    Reg 15 stores low bytes */
    port_byte_out(REG_SCREEN_CTRL, 14);
    unsigned int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor(unsigned int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

// TODO
/* unsigned int handle_scrolling(unsigned int offset); */

void print_at(char *message, int row, int col) {
    if (row >= 0 && col >= 0) {
        set_cursor(get_screen_offset(row, col));
    }

    int i = 0;
    while (message[i] != '\0') {
        print_char(message[i], row, col, 0);
        i++;
    }
}

void print(char *message) { print_at(message, -1, -1); }

void clear_screen() {
    int screen_size = MAX_COL * MAX_ROW;
    int i;
    char *screen = (char *)VIDEO_MEMORY;

    for (i = 0; i < screen_size; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor(get_screen_offset(0, 0));
}
