#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void catcher()
{
    printf("caught\n");
    exit(0);
}
void catcher2()
{
    printf("caught2\n");
    exit(0);
}
int main(int argc, char **argv)
{
    signal(SIGINT, catcher);
    signal(SIGQUIT, catcher2); // topでPID確認後 kill -SIGQUIT 〇〇
    while (1) {
        printf("%s\n", argv[1]);
    }
}