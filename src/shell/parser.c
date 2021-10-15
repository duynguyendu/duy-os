#include "parser.h"

int parse_line(char *buf, char **argv) {
    int buflen = strlen(buf);
    buf[buflen - 1] = ' ';
    char *delim;
    int bg;
    int dq = 0;
    int argc = 0;

    /* Remove trailing white space */
    while (*buf && (*buf == ' ')) {
        buf++;
    }

    /* TODO handle double quote */
    delim = buf;
    while (*delim) {
        if (*delim == '"') {
            dq = !dq;
        } else if (*delim == ' ' && !dq) {
            argv[argc++] = buf;
            *delim = '\0';
            buf = delim + 1;
        }
        delim++;
    }
    while ((delim = strchr(buf, ' '))) {
        /* Deliminate string with whitespace ' ' */
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;

        /* Remove redundant white space */
        while (*buf && (*buf == ' ')) {
            buf++;
        }
    }

    /* The last is null to indicate the end */
    argv[argc] = NULL;
    if (argc == 0) {
        return 1;
    }

    /* Check if running in background */
    if ((bg = (*argv[argc - 1] == '&'))) {
        argv[--argc] = NULL;
    }
    return bg;
}
