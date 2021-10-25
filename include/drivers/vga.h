#ifndef __VGA_H__
#define __VGA_H__

#include <types.h>

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

#define VIDEO_MEMORY 0xb8000
#define MAX_ROW 25
#define MAX_COL 80
#define VIDEO_MEMORY_SIZE 2000
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

static inline uint8_t vga_color_entry(enum vga_color fg, enum vga_color bg) {
    /* 3 bits indicate bg, 4 bits indicate fg */
    return bg << 4 | fg;
}

static inline uint16_t vga_entry(char c, uint8_t attr) {
    /* Each cell require 2 bytes, 1 for ascii char, 1 for fg, bg */
    return (uint16_t)attr << 8 | (uint16_t)c;
}

/* Print a character at row, col or at cursor current position */
void vag_put_at(char character, int row, int col, char attribute_byte);
void vga_put(char c);
void vga_print_at(char *message, int row, int col);
void vga_print(char *message);
void vga_clear_screen();
void vga_backspace();

void enable_cursor();
void disable_cursor();

#endif // !__VGA_H__
