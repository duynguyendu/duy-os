#include "util.h"
#include "builtin_functions.h"

#define MAX_CMD_LINE 1024
#define MAX_ARGS 1024

char **environ;
char pathstr[1024] = "/bin:/home/duy/.local/bin:";
char *paths[5];

// A mapping table for the name and its function
int num_of_builtin = 7;
char builtin_func_name[20][20] = {"name", "shell",  "exit",   "slam",
                                  "pwd",  "cowsay", "version"};
builtin_func_t builtin_func[20] = {__name, __shell,  __exit,   __slam,
                                   __pwd,  __cowsay, __version};

void eval(char *cmdline);
int builtin(char **argv);
void process_path(char *pathstr, char **paths);

int main() {
    char cmdline[MAX_CMD_LINE];
    int cmdlen;
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
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }

        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                printf("Error: waitpid error\n");
            }
        } else {
            printf("%d %s", pid, cmdline);
        }
    }
}

int builtin(char **argv) {
    for (int i = 0; i < num_of_builtin; i++) {
        if (equal(argv[0], builtin_func_name[i])) {
            builtin_func[i](argv);
            return 1;
        }
    }
    return 0;
}

int parse_line(char *buf, char **argv) {
    int buflen = strlen(buf);
    buf[buflen - 1] = ' ';
    char *delim;
    int bg;
    int dq = 0;
    int argc = 0;

    // Remove trailing white space
    while (*buf && (*buf == ' ')) {
        buf++;
    }

    // TODO handle double quote
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
        // Delimenate between bufing by replacing ' ' with '\0'
        // And having a pointer points to beginning of buf
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;

        // Remove redundant white space
        while (*buf && (*buf == ' ')) {
            buf++;
        }
    }

    // The last should be null
    argv[argc] = NULL;
    if (argc == 0) {
        return 1;
    }

    // Check if running in background
    if ((bg = (*argv[argc - 1] == '&'))) {
        argv[--argc] = NULL;
    }
    return bg;
}

void process_path(char *pathstr, char **paths) {
    char *path_delim;
    int p_total = 0;
    // pathstr[strlen(pathstr)-1] = ':';
    while ((path_delim = strchr(pathstr, ':'))) {
        paths[p_total++] = pathstr;
        *path_delim = '\0';
        pathstr = path_delim + 1;
    }
    paths[p_total] = NULL;
}
