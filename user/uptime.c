#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int t = uptime();
    printf("time: %d\n", t);
    exit(0);
}