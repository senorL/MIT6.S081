#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void sieve(int p_num) {
    int p_sieve[2];
    pipe(p_sieve);

    int p;
    int t = read(p_num, &p, sizeof(int));

    if (t == 0) {
        close(p_sieve[0]);
        close(p_sieve[1]);
        close(p_num);
        return;
    }

    printf("prime %d\n", p);

    if (fork() == 0){
        close(p_sieve[0]);
        int n;
        while(read(p_num, &n, sizeof(int))) {
            if (n % p != 0) {
                write(p_sieve[1], &n, sizeof(int));
            }
        }
        close(p_sieve[1]);
        close(p_num);
        exit(0);
    }
    close(p_sieve[1]);
    close(p_num);
    sieve(p_sieve[0]);

    close(p_sieve[0]);

    wait((int *)0);

}

int main(int argc, char *argv[])
{
    int p_num[2];
    pipe(p_num);


    if (fork() == 0) {
        close(p_num[0]); 
        for (int num = 2; num <= 280; num++) {
            write(p_num[1], &num, sizeof(num));
        }
        close(p_num[1]); 
        exit(0);
    }

    close(p_num[1]);
    sieve(p_num[0]);

    close(p_num[0]);
    wait((int *)0);
    exit(0);
}