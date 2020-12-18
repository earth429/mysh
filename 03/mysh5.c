#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void sigint_catcher() { // SIGINTシグナルを検知したとき
    printf("-----SIGINT was sent to the childprocess!-----\n");
    kill(getpid() + 1, SIGINT); // 子プロセスに送るので+1
}

int main()
{
    int i;
    int childpid;
    int status;
    char buf[BUFSIZ];
    char prompt[16] = "prompt> ";
    char **ap, *argv[10], *inputstring;
    printf("%s", prompt);
    signal(SIGINT, sigint_catcher); // SIGINTのシグナルをキャッチ
    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // '\0'を取り除く
        inputstring = buf;
        // inputstringの文字列をstrsepで分割してargvに格納
        for (ap = argv; (*ap = strsep(&inputstring, " \t")) != NULL;)
            if (**ap != '\0')
                if (++ap >= &argv[10])
                    break;
        
        if ((childpid = fork()) == 0) { // プロセス作成(外部コマンド)
            /* ここは子プロセス */
            execv(argv[0], argv); // argv[0]にはコマンドのフルパス、argvにはフルパス+引数
            exit(0); // 子プロセス終了
        } else {
            /* ここは親プロセス */
            wait(&status); // 子プロセスが死ぬとstatusに値が入る
            printf("status:%04x\n", status); // status情報を表示(死に方)
        }
        printf("%s", prompt);
    }
}