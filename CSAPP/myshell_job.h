
#ifndef __JOB_H__
#define __JOB_H__
#include <csapp.h>

#define MAXJOBS 10
#define RUNNING 1
#define STOPPING 0

struct job {
    int job;	// ��ҵ��
    pid_t pid;	// ��ҵpid
    int status;	// ��ҵ״̬
    int bg;		// ��ҵ�Ƿ�Ϊ��̨
    char *cmdline;	// ��ҵ��
};

// ��ҵ����
struct job *joblist[MAXJOBS];

void initjobs() {
    int i;
    for (i = 0; i < MAXJOBS; ++i) {
        if (joblist[i] != NULL) {
            Free(joblist[i]);
            joblist[i] = NULL;
        }
    }
}

int addjob(pid_t pid, char *cmdline, int bg) {
    int i;
    for (i = 0; i < MAXJOBS; ++i) {
        if (joblist[i] == NULL) {
			// ע��Ҫmalloc����
            joblist[i] = (struct job *)Malloc(sizeof(struct job));
            joblist[i]->pid = pid;
            joblist[i]->job = i;
            joblist[i]->bg = bg;
            joblist[i]->status = RUNNING;
            joblist[i]->cmdline = (char *)Malloc(strlen(cmdline));
            strcpy(joblist[i]->cmdline, cmdline);
            return i;
        }
    }
    printf("no space to run job");
    return -1;
}

int findjob(pid_t pid) {
    int i;
    for (i = 0; i < MAXJOBS; ++i)
        if (joblist[i] != NULL && joblist[i]->pid == pid)
            return i;

    return -1;
}

int deletejob_by_pid(pid_t pid) {
    int i = findjob(pid);
    if (i != -1) {
        Free(joblist[i]->cmdline);
        Free(joblist[i]);
        joblist[i] = NULL;
        return i;
    } else {
        printf("can't find job");
        return -1;
    }
}

int deletejob_by_index(int i) {
    if (joblist[i] != NULL) {
        Free(joblist[i]->cmdline);
        Free(joblist[i]);
        joblist[i] = NULL;
        return i;
    }
    return -1;
}

void job_info(struct job *_job) {
    printf("job [%d] pid: %d status: %d bg: %d cmdline: %s", _job->job,
           _job->pid, _job->status, _job->bg, _job->cmdline);
}

#endif
