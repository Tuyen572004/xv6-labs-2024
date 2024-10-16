#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char* fmtname(char * path) {
    char* p = 0;
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    return ++p;
}

void find(char* path, char* fileName) {
    char buf[1024], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(3, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(3, "find: cannot stat %s\n", path);
        return;
    }

    switch(st.type) {
        case T_FILE:
            if (strcmp(fmtname(path), fileName) == 0) {
                fprintf(1,"%s\n", path);
            }
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                fprintf(3, "find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 ||
                    strcmp(de.name, ".") == 0 ||
                    strcmp(de.name, "..") == 0
                ) continue;

                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, fileName);
            }
            break;
    }

    close(fd);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <target file>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
}