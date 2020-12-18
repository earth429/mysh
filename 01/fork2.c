#include <stdio.h>
#include <unistd.h>

main()
{
    int childpid;
    int status;

    if((childpid = fork()) == 0) {
        printf("child:childpid = %d, pid = %d, parent pid = %d\n", childpid, getpid(), getppid());
        exit(0);
    }
    else {
        printf("parent:childpid = %d, pid = %d, parent pid = %d\n", childpid, getpid(), getppid());
        wait(&status);
    }
}
