
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

extern char **environ;

int mysystem(char *command) {
    pid_t pid;
    int status;
    if ((pid = fork()) == 0) {
        char *argv[4] = {"bin/sh", "-c", command, NULL};
        if ((status = execve(argv[0], argv, environ)) < 0) {
            printf("execlp error");
            return status;
        }
    }

    if (waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status);
        }
    }
}
