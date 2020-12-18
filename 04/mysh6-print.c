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
    int childpid, status; // forkに用いる変数
    int fd, pipe_fd[2]; // pipeに用いる変数
    int chpid1, chpid2, status1, status2; // 2回forkするときに必要
    int pipe_flag = 0; // pipeがあるかチェックする用
    char dirbuf[BUFSIZ]; // 今いるディレクトリのパスを保存
    char arrow[4] = "> "; // 表示用
    char buf[BUFSIZ]; // 文字を読み込むとき使う
    char **ap, *argv[10], *inputstring;
    getcwd(dirbuf, BUFSIZ); // カレントワーキングディレクトリ名の取得
    printf("%s", dirbuf);   // ここでパスを表示
    printf("%s", arrow);
    signal(SIGINT, sigint_catcher); // SIGINTのシグナルをキャッチ
    while (fgets(buf, BUFSIZ, stdin) != NULL) { // 入力されたコマンドを一行読む
        buf[strlen(buf) - 1] = '\0'; // 末尾の改行コードを削除
        inputstring = buf;
        // inputstringの文字列をstrsepで分割してargvに格納
        for (ap = argv; (*ap = strsep(&inputstring, " \t")) != NULL;)
            if (**ap != '\0')
                if (++ap >= &argv[10])
                    break;

        /* mysh6より追加 */
        i = 0;
        pipe_flag = 0;
        // パイプ記号があるかを探索
        while (argv[i] != NULL) {
            puts("me");
            if (strcmp(argv[i], "|") == 0) { // パイプ記号を発見
            puts("neko");
                argv[i] = '\0'; // パイプ記号をNULLに
                pipe_flag++; // パイプ記号があるときはflagが1に
                break;
            }
            i++;
        }

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
        } else {  // プロセス作成(外部コマンド)
            if (pipe_flag != 0) {
                /* パイプ記号ありの時の処理 */
                puts("me!");
                pipe_flag = 0;
                for (i = 0; i < 10; i++) {
                    if (argv[i] == NULL && pipe_flag != 0) {
                        exit(0); // とりあえず終了
                    } else if (argv[i] == NULL) {
                        pipe_flag++; //最初のNULLはpipe記号の分なので
                        printf("\n");
                    } else {
                        printf("argv[%d]: %s\n", i, argv[i]);
                    }
                }
            } else {
                /* パイプ記号なしの時の処理 */
                if ((childpid = fork()) == 0) {
                    /* ここは子プロセス */
                    execvp(argv[0], &argv[0]); // argv[0]にはコマンド、&argv[0]はargv[0]のアドレス
                    exit(0); // 子プロセス終了
                } else {
                    /* ここは親プロセス */
                    wait(&status); // 子プロセスが死ぬとstatusに値が入る
                    printf("status:%04x\n", status); // status情報を表示(死に方)
                }
            }
        }
        getcwd(dirbuf, BUFSIZ); // カレントワーキングディレクトリ名の取得
        printf("%s", dirbuf);   // ここでパスを表示
        printf("%s", arrow);
    }
}