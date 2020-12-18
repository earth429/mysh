#include <stdio.h>
#include <sys/stat.h>
#include <fts.h>

int main()
{
    FTS *ftsp;
    FTSENT *p;
    char *dotav[] = {".", NULL};

    ftsp = fts_open(dotav, FTS_PHYSICAL, NULL);

    while ((p = fts_read(ftsp)) != NULL) {
        printf("fts_info: %d\n", p->fts_info);
        switch (p->fts_info) {
            case FTS_F: // 普通のファイルの時
                printf("filename: %s, uid: %d\n", 
                    p->fts_name, p->fts_statp->st_uid);
                break;
            /*case FTS_D: // 
                printf("filename: %s, uid: %d\n", 
                    p->fts_name, p->fts_statp->st_uid);
                break;*/
        }
    }
}