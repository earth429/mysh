#include <stdio.h>
#include <unistd.h>

main()
{
    int childpid;
    int status;

    if((childpid = fork()) == 0) {
        printf("child\n");
        exit(0);
    }
    else {
        printf("parent\n");
        wait(&status);
    }
}
