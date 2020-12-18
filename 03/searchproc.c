#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main()
{
    int i;
    for (i = 0; i < 10000;i++) {
        if (kill(i, 0) == 0) { // 0のシグナルを送りプロセスIDの存在確認
            printf("This PID process exists:%d\n", i);
        }
    }
}