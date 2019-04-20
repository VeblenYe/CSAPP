#define _POSIX_SOURCE

#include <csapp.h>

volatile pid_t pid;

void sigchld_handler(int sig) {
    int olderrno = errno;
    pid = Waitpid(-1, NULL, 0);
    errno = olderrno;
}

int main() {
    sigset_t mask, prev;
    Signal(SIGCHLD, sigchld_handler);

    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while (1) {
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        if (Fork() == 0) {
            exit(0);
        }

        pid = 0;
        while (!pid) {
            Sigsuspend(&prev);
        }

        Sigprocmask(SIG_SETMASK, &prev, NULL);

        printf(".");
    }

    exit(0);
}
