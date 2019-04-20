#define _POSIX_SOURCE

#include "myshell_job.h"
#include "myshell_sighandler.h"
#include <csapp.h>

#define MAXARGS 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main() {
    char cmdline[MAXLINE];

	// 设置自己的信号处理函数
    Signal(SIGINT, sigint_handler);
    Signal(SIGTSTP, sigtstp_handler);
    Signal(SIGCHLD, sigchld_handler);
    initjobs();

    sigset_t mask, prev;
    Sigfillset(&mask);
    while (1) {
        // Sigprocmask(SIG_BLOCK, &mask, &prev);
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) {
            exit(0);
        }
        // Sigprocmask(SIG_SETMASK, &prev, NULL);
        eval(cmdline);
    }
}

void eval(char *cmdline) {
    char buf[MAXLINE];
    strcpy(buf, cmdline);
    char *argv[MAXARGS];
    int bg = parseline(buf, argv);
    if (argv[0] == NULL) {
        return;
    }

    if (!builtin_command(argv)) {
        // printf("non-builtin command\n");
		// 同步流，阻塞所有信号，防止产生addjob和deletejob的竞争
        sigset_t mask, prev;
        Sigaddset(&mask, SIGCHLD);
        Sigprocmask(SIG_BLOCK, &mask, &prev);

        pid_t pid;
        if ((pid = Fork()) == 0) {
			// 解除子进程中不需要的阻塞
            Sigprocmask(SIG_SETMASK, &prev, NULL);
            Signal(SIGINT, SIG_DFL);
            Signal(SIGTSTP, SIG_DFL);
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s command not found\n", argv[0]);
                exit(0);
            }
        }
        int job = addjob(pid, cmdline, bg);
        if (!bg) {
			// 显式等待子进程退出
            Sigsuspend(&prev);
        } else {
            printf("[%d] %d %s\n", job, pid, cmdline);
        }
        Sigprocmask(SIG_SETMASK, &prev, NULL);
    }
}

int builtin_command(char **argv) {
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }
    if (!strcmp(argv[0], "&")) {
        return 1;
    }
    if (!strcmp(argv[0], "jobs")) {
        int i;
        for (i = 0; i < MAXJOBS; ++i) {
            if (joblist[i] != NULL) {
                job_info(joblist[i]);
            }
        }
        return 1;
    }
    if (!strcmp(argv[0], "fgjobs")) {
        int i;
        for (i = 0; i < MAXJOBS; ++i) {
            if (joblist[i] != NULL && joblist[i]->bg == 0) {
                job_info(joblist[i]);
            }
        }
        return 1;
    }
    if (!strcmp(argv[0], "bgjobs")) {
        int i;
        for (i = 0; i < MAXJOBS; ++i) {
            if (joblist[i] != NULL && joblist[i]->bg == 1) {
                job_info(joblist[i]);
            }
        }
        return 1;
    }
    if (!strcmp(argv[0], "bg")) {
        int i;
        if (argv[1] != NULL) {
            if (argv[1][0] == '%') {
                argv[1][0] = ' ';
                i = atoi(argv[1]);
            } else if (isdigit(argv[1])) {
                int pid = atoi(argv[1]);
                int i = findjob(pid);
            } else {
                printf("bg error\n");
            }
        } else {
            printf("command error\n");
        }
        if (joblist[i] != NULL) {
            joblist[i]->status = RUNNING;
            kill(joblist[i]->pid, SIGCONT);
        } else {
            printf("job not found\n");
        }
        return 1;
    }
    if (!strcmp(argv[0], "fg")) {
        int i;
        if (argv[1] != NULL) {
            if (argv[1][0] == '%') {
                argv[1][0] = ' ';
                i = atoi(argv[1]);
            } else if (isdigit(argv[1])) {
                int pid = atoi(argv[1]);
                int i = findjob(pid);
            } else {
                printf("fg error\n");
            }
        } else {
            printf("command error\n");
        }

        if (joblist[i] != NULL) {
            job_info(joblist[i]);
            joblist[i]->bg = 0;
            joblist[i]->status = RUNNING;
            kill(joblist[i]->pid, SIGCONT);
            waitpid(joblist[i]->pid, NULL, 0);
        } else {
            printf("job not found\n");
        }

        return 1;
    }
    return 0;
}

int parseline(char *buf, char **argv) {
    buf[strlen(buf) - 1] = ' ';
    while (*buf && *buf == ' ') {
        ++buf;
    }

    int argc = 0;
    char *delim;
    while (delim = strchr(buf, ' ')) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = ++delim;
        while (*buf && *buf == ' ')
            ++buf;
    }
    argv[argc] = NULL;

    if (argc == 0) {
        return 1;
    }

    int bg;
    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }

    return bg;
}
