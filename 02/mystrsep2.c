#include <stdio.h>
#include <string.h>

int main()
{
    int i;
    char buf[BUFSIZ];
    char prompt[16] = "prompt> ";
    char **ap, *argv[10], *inputstring;
    printf("%s", prompt);
    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // '\0'を取り除く
        inputstring = buf;
        for (ap = argv; (*ap = strsep(&inputstring, " \t")) != NULL;)
            if (**ap != '\0')
                if (++ap >= &argv[10])
                    break;
        for (i = 0; i < 10;i++) {
            printf("argc[%d]:%s\n", i, argv[i]); // 入力された文字を表示
            if(argv[i] == NULL)
                break;
        }
        printf("%s", prompt);
    }
}