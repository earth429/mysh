#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void catcher()
{
    printf("caught\n");
    exit(0);
}
int main(int argc, char **argv)
{
    signal(SIGINT, catcher);
    while(1) {
        printf("%s\n", argv[1]);
    }
}