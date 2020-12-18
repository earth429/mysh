#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char buf[BUFSIZ];
    time_t tval;
    struct tm *pTime;

    tval = time(NULL);
    pTime = localtime(&tval);

    strftime(buf, sizeof(buf), "%Y %B %e (%A) %r, %Z", pTime);
    printf("%s\n", buf);
}
