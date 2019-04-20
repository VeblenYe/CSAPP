#define _POSIX_SOURCE

#include <csapp.h>

sigjmp_buf buf;

void handler(int sig) { siglongjmp(buf, 1); }

char *tfgets(char *s, int n, FILE *in) {
    char *result;

    if (!sigsetjmp(buf, 1)) {
        alarm(5);
        if (Signal(SIGALRM, handler) == SIG_ERR) {
            unix_error("set sigalrm handler error");
        }
        return fgets(s, n, in);
    } else {
        return NULL;
    }
}
