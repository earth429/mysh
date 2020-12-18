#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void sigint_catcher() { // SIGINTシグナルを検知したとき
    kill(getpid() + 1, SIGINT); // 子プロセスに送るので+1
}

int main()
{
    int i;
    int childpid;
    int status;
    char buf[BUFSIZ];
    char dirbuf[BUFSIZ];
    char arrow[4] = "> "; // 表示用
    char **ap, *argv[10], *inputstring;
    getcwd(dirbuf, BUFSIZ); // カレントワーキングディレクトリ名の取得
    printf("%s", dirbuf);   // ここでパスを表示
    printf("%s", arrow);
    signal(SIGINT, sigint_catcher); // SIGINTのシグナルをキャッチ
    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // '\0'を取り除く
        inputstring = buf;
        // inputstringの文字列をstrsepで分割してargvに格納
        for (ap = argv; (*ap = strsep(&inputstring, " \t")) != NULL;)
            if (**ap != '\0')
                if (++ap >= &argv[10])
                    break;
        
        if (strcmp(argv[0], "k") == 0) { // kという内部コマンドの実装
            kill(atoi(argv[1]), SIGINT); // 指定したPIDにSIGINTを送る
        } else if (strcmp(argv[0], "s") == 0) { // sという内部コマンドの実装
            kill(atoi(argv[1]), SIGSTOP); // 指定したPIDにSIGSTOPを送る
        } else if (strcmp(argv[0], "c") == 0) { // cという内部コマンドの実装
            kill(atoi(argv[1]), SIGCONT); // 指定したPIDにSIGINTを送る
        } else if (strcmp(argv[0], "cd") == 0) { // cdという内部コマンドの実装
            if (argv[1] == NULL) { // ディレクトリを指定してない時
                chdir(getenv("HOME")); // ホームディレクトリに移動
            } else {
                chdir(argv[1]); // 指定したディレクトリに移動
            }
        } else if (strcmp(argv[0], "exit") == 0) { // exitという内部コマンドの実装(オリジナル)
            exit(0); // myshを終了
        } else if ((childpid = fork()) == 0) { // プロセス作成(外部コマンド)
            /* ここは子プロセス */
            execv(argv[0], argv); // argv[0]にはコマンドのフルパス、argvにはフルパス+引数
            exit(0); // 子プロセス終了
        } else {
            /* ここは親プロセス */
            wait(&status); // 子プロセスが死ぬとstatusに値が入る
            printf("status:%04x\n", status); // status情報を表示(死に方)
        }
        getcwd(dirbuf, BUFSIZ); // カレントワーキングディレクトリ名の取得
        printf("%s", dirbuf);   // ここでパスを表示
        printf("%s", arrow);
    }
}