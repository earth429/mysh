#include <stdio.h>
#include <string.h>
#include <unistd.h>

main()
{
    char str[128];
    
    while(1){
        printf("prompt>");
        fgets(str, 128, stdin);
        str[strlen(str)] = '\0';
        printf("%s", str);
    }
}
