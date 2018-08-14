#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "lf-journal.h"
#include <new>

int g_fd;
lfdata* g_lfdata_ptr;

static bool push(int i)
{
    std::atomic_flag* sp = &(g_lfdata_ptr->state);
    while (sp->test_and_set(std::memory_order_acquire)) {}
    {
        g_lfdata_ptr->top0++;
        g_lfdata_ptr->index0[g_lfdata_ptr->top0] = i;
    }
    sp->clear(std::memory_order_release);

    return true;
}

static bool pop(int& i)
{
    std::atomic_flag* sp = &(g_lfdata_ptr->state);
    while (sp->test_and_set(std::memory_order_acquire)) {}
    {
        i = g_lfdata_ptr->index0[g_lfdata_ptr->top0];
        g_lfdata_ptr->top0--;
    }
    sp->clear(std::memory_order_release);

    return true;
}

int lf_journal_init()
{
    g_fd = open(LF_JOURNAL_LFDATA, O_RDWR);    
    void* addr = mmap(NULL, sizeof(lfdata), PROT_READ|PROT_WRITE, MAP_SHARED, g_fd, 0);
    g_lfdata_ptr = (lfdata*)addr;
    void* p = &(g_lfdata_ptr->state);
    std::atomic_flag* sp = new(p) std::atomic_flag(ATOMIC_FLAG_INIT);
    g_lfdata_ptr->top0 = -1;
    g_lfdata_ptr->top1 = -1;
    push(0);
    push(1);
    push(2);
    push(3);
    for (int i = 0; i < 4; i++) g_lfdata_ptr->data0[i] = 0;
    munmap(addr, sizeof(lfdata));
    close(g_fd);

    return 0;
}

int lf_journal_open()
{
    g_fd = open(LF_JOURNAL_LFDATA, O_RDWR);    
    void* addr = mmap(NULL, sizeof(lfdata), PROT_READ|PROT_WRITE, MAP_SHARED, g_fd, 0);
    g_lfdata_ptr = (lfdata*)addr;
    return 0;
}

int lf_journal_increment()
{
    int index;
    pop(index);
    g_lfdata_ptr->data0[index]++;
    push(index);
    return 0;
}

int lf_journal_print()
{
    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += g_lfdata_ptr->data0[i];
        printf("data0[%d] %d\n", i, g_lfdata_ptr->data0[i]);
    }
    printf("sum %d\n", sum);
    return 0;
}

int lf_journal_close()
{
    munmap(g_lfdata_ptr, sizeof(lfdata));
    close(g_fd);
    return 0;
}
