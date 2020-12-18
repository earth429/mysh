#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int time = atoi(argv[1]);
    alarm(time);
    pause();
}