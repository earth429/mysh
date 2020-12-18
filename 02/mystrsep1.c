#include <stdio.h>
#include <string.h>

int main(){
    char str[128] = "This is a pen.";
    char *str_p;

    str_p = str;
    printf("%s\n", strsep(&str_p, " "));
    printf("%s\n", strsep(&str_p, " "));
    printf("%s\n", strsep(&str_p, " "));
    printf("%s\n", strsep(&str_p, " "));
}