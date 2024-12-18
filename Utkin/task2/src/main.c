#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "pipe.h"

#define MAX_SIZE 4096

int sendfile(char* files[], Pipe* pipe2, char buf[]) {
    pid_t pid = fork();
    if (pid == 0) {
        int filer = open(files[1], O_RDONLY);
        int filew = open(files[2], O_WRONLY | O_CREAT, 0666);
        if (filer == -1 || filew == -1) {printf("Error opened files\n"); return 1;}
        size_t c = 0;
        close(pipe2->fd_direct[1]); close(pipe2->fd_back[0]);

        while (c = read(filer, buf, MAX_SIZE)) {
            printf("size send: %d\n", c);
            pipe2->data = buf; pipe2->len = c;
            pipe2->actions.snd(pipe2);
            if (c = pipe2->actions.rcv(pipe2)) {
                pipe2->len = c;
                write(filew, buf, c);
                pipe2->len = MAX_SIZE;
            }
        }
        printf("sended\n");
        close(pipe2->fd_back[1]);
        while (c = pipe2->actions.rcv(pipe2)) {
            pipe2->len = c;
            write(filew, buf, c);
            pipe2->len = MAX_SIZE;
        }
        close(filer); close(filew);
    } else {
        close(pipe2->fd_direct[0]); close(pipe2->fd_back[1]);
        size_t c = MAX_SIZE;
        pipe2->len = MAX_SIZE; pipe2->data = buf;
        while (c = pipe2->actions.rcv(pipe2)) {
            pipe2->len = c;
            printf("Size recall: %d\n", c);
            pipe2->actions.snd(pipe2);
            pipe2->len = MAX_SIZE;
        }
        printf("Recooled\n");
        printf("Closed pipes\n"); close(pipe2->fd_direct[1]); close(pipe2->fd_back[0]);
    }
}

int main(int argc, char* argv[]) {
    Pipe* pipe2 = c_pipe2();
    char buf[MAX_SIZE];
    sendfile(argv, pipe2, buf);
    
    free(pipe2);
    return 0;
}