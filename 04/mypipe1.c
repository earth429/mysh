#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>

int main()
{
    int fd, pipe_fd[2];
    char buf[140];

    fd = open("mypipe1.c", O_RDONLY);
    pipe(pipe_fd);

    read(fd, buf, sizeof(buf)); // fdを使って、mypipe1.cからbufにデータをいれる
    write(pipe_fd[1], buf, sizeof(buf)); // pipe_bufferにfdからとってきたデータを流す
    read(pipe_fd[0], buf, sizeof(buf)); // pipe_bufferからデータとってくる
    write(1, buf, sizeof(buf)); // 1は標準出力
}