#include "builtin_functions.h"

void __name(char **argv) {
    printf("Duy\n");
}

void __shell(char **argv) {
    printf("DeShell\n");
}

void __slam(char **argv) {
    // Echo
    char **current = argv+1;
    while (*current != NULL) {
        // Print environment variable
        // if ((*current)[0] == '$') {

        // }
        if (equal(*current, "$PATH")) {
            // // Join paths with :
            // printf("%s", paths[0]);
            // int i = 1;
            // while (paths[i] != NULL) {
            //     printf(":%s", paths[i]);
            //     i++;
            // }
        } else {
            // Remove double quote in double-quoted word
            if (*current[0] == '"') {
                (*current)++;
                (*current)[strlen(*current)-1] = '\0';
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

void __exit(char **argv) {
    exit(0);
}

void __cowsay(char **argv) {
    if (argv[1] == NULL) {
        printf("cowsay needs something to say\n");
    }  else {
        printf("*** .-. ***\n");
        printf("%s\n", argv[1]);
    }
}

void __version(char **argv) {
    printf("1.0\n");
}
