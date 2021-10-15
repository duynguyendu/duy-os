#ifndef __BUILT_IN__
#define __BUILT_IN__

#include "util.h"

typedef void (*builtin_func_t) (char **argv);

extern char builtin_func_name[20][20];
extern builtin_func_t builtin_func[20];
extern char **environ;

void __name(char **argv);
void __shell(char **argv);
void __slam(char **argv);
void __exit(char **argv);
void __pwd(char **argv);
void __cowsay(char **argv);
void __version(char **argv);

#endif /* __BUILT_IN__ */
