
#ifndef __SIGHANDLER_H__
#define __SIGHANDLER_H__

#include "myshell_job.h"
#include <csapp.h>

void sigint_handler(int sig) {
    // printf("sigtint_handler called\n");
    int olderrno = errno;
    int i;
    for (i = 0; i < MAXJOBS; ++i) {
        if (joblist[i] != NULL)
            kill(joblist[i]->pid, SIGINT);
    }
    errno = olderrno;
}

void sigtstp_handler(int sig) {
    // printf("sigtstp_handler called\n");
    int olderrno = errno;
    int i;
    for (i = 0; i < MAXJOBS; ++i) {
        if (joblist[i] != NULL) {
            joblist[i]->status = STOPPING;
            kill(joblist[i]->pid, SIGTSTP);
        }
    }
    errno = olderrno;
}

void sigchld_handler(int sig) {
    // printf("sigchld_hander called\n");
    int olderrno = errno;

    sigset_t mask, prev;
    Sigfillset(&mask);
    pid_t pid;
    int status;
    int i;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        Sigprocmask(SIG_BLOCK, &mask, &prev);
        i = findjob(pid);
        if (WIFEXITED(status)) {
            printf("Job %d exit: %d\n", i, WEXITSTATUS(status));
            deletejob_by_pid(pid);
        } else if (WIFSIGNALED(status)) {
            printf("Job %d terminated by signal: %d\n", i, WTERMSIG(status));
            deletejob_by_index(i);
        } else if (WIFSTOPPED(status)) {
            printf("Job %d %d stopped by signal: %d\n", i, pid,
                   WSTOPSIG(status));
        }
        Sigprocmask(SIG_SETMASK, &prev, NULL);
    }

    errno = olderrno;
}
#endif