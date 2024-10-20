#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int c2p[2], p2c[2];
    pipe(c2p);
    pipe(p2c);
    int pid = fork();
    char buf;
    char msg = 'a';

    if (pid == 0) {
        close(c2p[0]);
        close(p2c[1]);

        read(p2c[0], &buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(c2p[1], &msg, 1);

        close(c2p[1]);
        close(p2c[0]);
    }
    else if (pid > 0) {
        close(c2p[1]);
        close(p2c[0]);

        write(p2c[1], &msg, 1);
        read(c2p[0], &buf, 1);
        fprintf(1, "%d: received pong\n", getpid());

        close(c2p[0]);
        close(p2c[1]);
    }
    else {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    exit(0);
}