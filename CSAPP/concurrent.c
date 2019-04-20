#define _POSIX_SOURCE

#include <csapp.h>

void handler(int sig) {

    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) {
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        // deletejob();
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }

    if (errno != ECHILD) {
        Sio_error("waitpid error");
    }

    errno = olderrno;
}

int main(int argc, char **argv) {
    sigset_t mask_one, mask_all, prev_all;

    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);
    Signal(SIGCHLD, handler);
    // initjobs();

    while (1) {
        Sigprocmask(SIG_BLOCK, &mask_one, &prev_all);
        pid_t pid;
        if ((pid = Fork()) == 0) {
            Sigprocmask(SIG_SETMASK, &prev_all, NULL);
            Execve("/bin/date", argv, NULL);
        }

        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        // addjob(pid);
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }

    exit(0);
}
