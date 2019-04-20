
#include <csapp.h>

int main(int argc, char **argv, char **envp) {
    int i;
    printf("command-lines arguments:\n");
    for (i = 0; i < argc; ++i) {
        printf("\targv[%2d]: %s\n", i, argv[i]);
    }
    printf("Environment variables:\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("\tenvp[%2d]: %s\n", i, envp[i]);
    }
    exit(0);
}
