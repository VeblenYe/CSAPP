

#include <csapp.h>
#define N 2

int main() {
    int status, i;
    pid_t pid;

    for (i = 0; i < 2; ++i)
        if ((pid = Fork()) == 0)
            exit(100 + i);

    while ((pid = wait(&status)) > 0) {
        if (WIFEXITED(status)) {
            printf("child %d terminated normally with exit status=%d\n", pid,
                   WEXITSTATUS(status));
        } else {
            printf("child %d terminated abnormally\n", pid);
        }
    }

    if (errno != ECHILD) {
        unix_error("wait error");
    }

    exit(0);
}
