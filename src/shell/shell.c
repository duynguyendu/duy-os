#include "builtin_functions.h"
#include "parser.h"
#include "util.h"

#define MAX_CMD_LINE 1024
#define MAX_ARGS 1024

char **environ;
char pathstr[1024] = "/bin:/home/duy/.local/bin:";
char *paths[5];

void eval(char *cmdline);
void process_path(char *pathstr, char **paths);

int main() {
    char cmdline[MAX_CMD_LINE];
    int cmdlen;
    // TODO process path
    extern char *paths[5];
    process_path(pathstr, paths);

    // TODO support history
    // TODO support upper arrow and down arrow
    // TODO pipeline
    // TODO redirect input output
    // TODO stderr
    // TODO CTRL+D CTRL+Z CTRL+C handling
    // TODO expand ~
    // TODO PS1
    // TODO coloring
    // TODO add more builtin command
    // TODO make test
    while (1) {
        printf("\033[0;31m$ \033[0m");
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
