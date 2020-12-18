#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_catcher() { // SIGINTシグナルを検知したとき
    kill(getpid() + 1, SIGINT); // 子プロセスに送るので+1
}

int main()
{
    int i;
    int childpid, status; // forkに用いる変数
    int pipe_fd[2], pipe_position; // pipeに用いる変数
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
            if (strcmp(argv[i], "|") == 0) { // パイプ記号を発見
                pipe_position = i; // パイプ位置を記憶
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
                if (pipe(pipe_fd) == -1) { //パイプバッファを作る                          
                    perror("pipe");
                    exit(1);
                }
                if ((chpid1 = fork()) == 0) { // child1生成
                    /* ここはchild1プロセス */
                    close(pipe_fd[0]); // 使わない
                    close(1); // 標準出力を閉じる(こうするとdupで勝手につなげてくれる)
                    dup(pipe_fd[1]); // pipe_fd[1]を標準出力につなげる
                    execvp(argv[0], &argv[0]); // argv[0]にはパイプの左側のコマンド
                    exit(0); // child1プロセス終了
                } else if ((chpid2 = fork()) == 0) { // child2生成
                    /* ここはchild2プロセス */
                    close(pipe_fd[1]); // 使わない
                    close(0); // 標準入力を閉じる(こうするとdupで勝手につなげてくれる)
                    dup(pipe_fd[0]); // pipe_fd[0]を標準入力につなげる
                    execvp(argv[pipe_position + 1], &argv[pipe_position + 1]); // argv[pipe_position + 1]にはパイプの右側のコマンド
                    exit(0); // child2プロセス終了
                } else {
                    /* ここはparentプロセス */
                    close(pipe_fd[0]); // 使わない
                    close(pipe_fd[1]); // 使わない
                    waitpid(chpid1, &status1, 0);    // child1が死ぬとstatus1に値が入る
                    waitpid(chpid2, &status2, 0);    // child2が死ぬとstatus2に値が入る
                    printf("status1:%04x\n", status1); // status情報を表示(死に方)
                    printf("status2:%04x\n", status2); // status情報を表示(死に方)
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