#include "builtin_functions.h"
#include "parser.h"
#include "util.h"
#include <setjmp.h>

#define MAX_CMD_LINE 1024
#define MAX_ARGS 1024

char **environ;
char pathstr[1024] = "/bin:/home/duy/.local/bin:";
char *paths[5];

jmp_buf promt_input;

void eval(char *cmdline);
void process_path(char *pathstr, char **paths);
void sig_int_handler(int signum) {
    longjmp(promt_input, 1);
}
void sig_int_from_command(int signum) {
    longjmp(promt_input, 2);
}

int main() {
    char cmdline[MAX_CMD_LINE];
    int cmdlen;
    // TODO process path
    extern char *paths[5];
    process_path(pathstr, paths);

    /*
    TODO support history
    TODO support upper arrow and down arrow
    TODO pipeline
    TODO redirect input output
    TODO CTRL+D CTRL+Z CTRL+C handling
    TODO expand ~
    TODO PS1
    TODO alias
    TODO coloring
    TODO add more builtin command
    TODO make test
    */

    /* Setup signaling */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_int_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; 

    while (1) {
        sigaction(SIGINT, &sa, NULL);
        int jmp_return = sigsetjmp(promt_input, -1);
        if (jmp_return == 1) {
            /* SIGINT when in promt input */
            printf("\b\b  \n");
        } else if (jmp_return == 2) {
            /* SIGINT when running command */
            printf("\n");
        }

        /* Print red $ and get user input */
        printf("\033[0;31mDuy $ \033[0m");
        fgets(cmdline, MAX_CMD_LINE, stdin);
        if (feof(stdin)) {
            exit(0);
        }
        eval(cmdline);
    }
    return 0;
}

void eval(char *cmdline) {
    char *argv[MAX_ARGS];
    int bg = parse_line(cmdline, argv);
    pid_t pid;

    if (argv[0] == NULL) {
        return;
    }

    if (!builtin(argv)) {
        signal(SIGINT, sig_int_from_command);
        if ((pid = fork()) == 0) {
            if (execve(argv[0], argv, environ) < 0) {
                char **path_name = paths;
                while (*path_name != NULL) {
                    char command_name[1024];
                    command_name[0] = '\0';
                    strcat(command_name, *path_name);
                    strcat(command_name, "/");
                    strcat(command_name, argv[0]);
                    if (!(execve(command_name, argv, environ) < 0)) {
                    }
                    path_name++;
                }
                fprintf(stderr, "%s: Command not found\n", argv[0]);
                exit(0);
            }
        }

        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                fprintf(stderr, "Error: waitpid error\n");
            }
        } else {
            printf("%d %s", pid, cmdline);
        }
    }
}

void process_path(char *pathstr, char **paths) {
    char *path_delim;
    int p_total = 0;
    while ((path_delim = strchr(pathstr, ':'))) {
        paths[p_total++] = pathstr;
        *path_delim = '\0';
        pathstr = path_delim + 1;
    }
    paths[p_total] = NULL;
}
