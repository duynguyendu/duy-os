#ifndef __SCREEN_H__
#define __SCREEN_H__

#define VIDEO_MEMORY 0xb8000
#define MAX_ROW 25
#define MAX_COL 80
#define WHITE_ON_BLACK 0x0f
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Print a character at row, col or at cursor current position */
void print_char(char character, int row, int col, char attribute_byte);
unsigned int get_screen_offset(int row, int col);
unsigned int get_cursor();
void set_cursor(unsigned int offset);
unsigned int handle_scrolling(unsigned int offset);
void print_at(char *message, int row, int col);
void print(char *message);
void clear_screen();

#endif // !__SCREEN_H__
