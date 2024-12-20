#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    char buf[1] = {" "};

    if (fork() == 0) {
        // child
        read(p1[0], buf, 1);
        close(p1[0]);
        close(p1[1]);

        int pid = getpid();
        printf("%d: received ping\n", pid);

        write(p2[1], buf, 1);
        close(p2[1]);
        close(p2[0]);
        exit(0);

    } else {
        // parent
        write(p1[1], buf, 1);
        close(p1[1]);
        close(p1[0]);
        
        wait((int *) 0);
        read(p2[0], buf, 1);
        close(p2[0]);
        close(p2[1]);

        int pid = getpid();
        printf("%d: received pong\n", pid);


    }
    exit(0);
}