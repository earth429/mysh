#include <unistd.h>

main(int argc, char *argv[])
{
    execv(argv[1], &argv[1]);
}

