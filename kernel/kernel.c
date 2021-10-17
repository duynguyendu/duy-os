#include "../drivers/screen.h"
#include "low_level.h"

void main() {
    char *videomem = (char *)0xb8000;
    /* char *message = "Hello"; */
    /* print(message); */
    /* print_char('X', -1, -1, 0); */
    /* unsigned int offset = get_cursor(); */

    unsigned int offset = get_cursor();
    videomem[offset] = 'X';

    offset += 2;

    offset = get_cursor();
    videomem[offset] = 'L';
}
