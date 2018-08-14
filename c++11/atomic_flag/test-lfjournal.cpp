#include "lf-journal.h"

int main()
{
    lf_journal_open();

    for (int i = 0; i < 100000000; i++) {
        lf_journal_increment();
    }
    
    lf_journal_close();
}
