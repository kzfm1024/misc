#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "lf-journal.h"

void* create_data(const char* path)
{
    int val = 0;
    FILE* fp = fopen(path, "w");
    fwrite(&val, sizeof(val), 1, fp);
    fclose(fp);

    return (void*)0;
}

void* create_lfdata()
{
    lfdata lfd;

    FILE* fp = fopen(LF_JOURNAL_LFDATA, "w");
    fwrite(&lfd, sizeof(lfd), 1, fp);
    fclose(fp);

    return (void*)0;    
}

int main()
{
    create_lfdata();
    create_data(LF_JOURNAL_DATA0_0);
    create_data(LF_JOURNAL_DATA0_1);
    create_data(LF_JOURNAL_DATA0_2);
    create_data(LF_JOURNAL_DATA0_3);
    create_data(LF_JOURNAL_DATA1_4);
    create_data(LF_JOURNAL_DATA1_5);
    create_data(LF_JOURNAL_DATA1_6);
    create_data(LF_JOURNAL_DATA1_7);

    lf_journal_init();
}
