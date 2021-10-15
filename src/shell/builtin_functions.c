#include "builtin_functions.h"

void __name(char **argv) { printf("Duy\n"); }

void __shell(char **argv) { printf("DeShell\n"); }

void __slam(char **argv) {
    char **current = argv + 1;
    while (*current != NULL) {
        if ((*current)[0] == '$') {
            /* Environment variable */
            char *e = getenv(*current + 1);
            if (e != NULL) {
                printf("%s ", e);
            }
        } else {
            /* Remove double quote in double-quoted word */
            if (*current[0] == '"') {
                (*current)++;
                (*current)[strlen(*current) - 1] = '\0';
            }
            printf("%s ", *current);
        }
        current++;
    }
    printf("\n");
}

void __pwd(char **argv) {
    if (argv[1] != NULL) {
        printf("pwd: I don't need that many arguments\n");
    } else {
        char cwd[1024];
        getcwd(cwd, 1024);
        printf("%s\n", cwd);
    }
}

void __exit(char **argv) { exit(0); }

void __cowsay(char **argv) {
    if (argv[1] == NULL) {
        printf("cowsay needs something to say\n");
    } else {
        printf("*** .-. ***\n");
        printf("%s\n", argv[1]);
    }
}

void __version(char **argv) { printf("1.0\n"); }
