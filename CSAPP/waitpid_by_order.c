

#include <csapp.h>

#define N 2

int main() {
    pid_t pid[N], retpid;
    int i;
    for (i = 0; i < N; ++i) {
        if ((pid[i] = Fork()) == 0) {
            exit(100 + i);
        }
    }

    int status;
    i = 0;
    while ((retpid = waitpid(pid[i++], &status, 0)) > 0) {
        if (WIFEXITED(status)) {
            printf("child %d terminated normally with wait status=%d\n", retpid,
                   WEXITSTATUS(status));
        } else {
            printf("child %d terminated abnormally\n", retpid);
        }
    }

    if (errno != ECHILD) {
        unix_error("waitpid error");
    }

    exit(0);
}
