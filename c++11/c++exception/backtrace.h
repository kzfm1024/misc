#pragma once

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

inline void print_backtrace(void)
{
    static const int SIZE = 128;
    void *buffer[SIZE];

    int nptrs = backtrace(buffer, SIZE);
    char **strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        return;
    }

    for (int j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
}

inline void write_backtrace(int fd)
{
    static const int SIZE = 128;
    void *buffer[SIZE];

    int nptrs = backtrace(buffer, SIZE);
    backtrace_symbols_fd(buffer, nptrs, fd);
}
