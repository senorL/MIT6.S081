#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *name) {
    int fd;
    char buf[512];
    char *real_path;
    strcpy(buf, path);

    real_path = buf + strlen(buf);
    *real_path++ = '/';


    struct stat st;
    struct dirent de;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        fprintf(2, "find: must input a dir\n");
        close(fd);
        return;
    }


    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

        memmove(real_path, de.name, strlen(de.name));
        real_path[strlen(de.name)] = '\0'; 

        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }


        if (st.type == T_DEVICE || st.type == T_FILE) {
            // file
            if (strcmp(de.name, name) == 0) {
                printf("%s\n", buf);
            }
        } 
        if (st.type == T_DIR) {
            //dir
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
                continue;
            
            memmove(real_path, de.name, strlen(de.name));
            real_path[strlen(de.name)] = '\0';
            find(buf, name);
        }
    }
    close(fd);


}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(2, "Usage: find [dir] [file name]\n");
        exit(0);
    }

    char *current_dir = argv[1];
    char *file_name = argv[2];

    find(current_dir, file_name);

    exit(0);
    


}