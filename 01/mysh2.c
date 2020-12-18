#include <stdio.h>
#include <string.h>
#include <unistd.h>

main()
{
    char str[128];
    int childpid;
    int status;
    
    while(1){
        printf("prompt>");
        fgets(str, 128, stdin);
        str[strlen(str)] = '\0';

        if((childpid = fork()) == 0) {
            execl("%s", "%s", str, str);
            exit(0);
        }
        else {
            wait(&status);
        }

    }
}
