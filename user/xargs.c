#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char buf[512];
    int pos = 0;
    char *argbuf[MAXARG];

    int i;
    for (i = 0; argv[i] != 0; i++) {
        argbuf[i] = argv[i];
    }

    for (;;) {
        char c;
        int n;
        n = read(0, &c, 1);
        if (n == 0)
            break;
        if (c == '\n') {
            buf[pos++] = '\0'; 

            argbuf[i] = malloc(pos + 1); 
            strcpy(argbuf[i], buf);
            memset(buf, '\0', pos);
            pos = 0;
            
            if (fork() == 0) {    
                exec(argbuf[1], argbuf + 1);
                exit(0);
            }
            wait((int *)0);

        }
        else {
            buf[pos++] = c;
        }
    }


    for (int j = 0; j < i; j++) {
        free(argbuf[j]); 
    }
    exit(0);


}