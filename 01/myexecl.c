#include <unistd.h>

main()
{
    execl("/usr/bin/cal", "/usr/bin/cal", "12", "2011", (char *)0);
}
