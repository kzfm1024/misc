#pragma once

#include <atomic>

/*
  /home/kzfm1024/tmp/journal/lfdata

  /home/kzfm1024/tmp/journal/data0/0.journal
  /home/kzfm1024/tmp/journal/data0/1.journal
  /home/kzfm1024/tmp/journal/data0/2.journal
  /home/kzfm1024/tmp/journal/data0/3.journal

  /home/kzfm1024/tmp/journal/data1/4.journal
  /home/kzfm1024/tmp/journal/data1/5.journal
  /home/kzfm1024/tmp/journal/data1/6.journal
  /home/kzfm1024/tmp/journal/data1/7.journal
*/

#define LF_JOURNAL_LFDATA  "/home/kzfm1024/tmp/journal/lfdata"
#define LF_JOURNAL_DATA0_0 "/home/kzfm1024/tmp/journal/data0/0.journal"
#define LF_JOURNAL_DATA0_1 "/home/kzfm1024/tmp/journal/data0/1.journal"
#define LF_JOURNAL_DATA0_2 "/home/kzfm1024/tmp/journal/data0/2.journal"
#define LF_JOURNAL_DATA0_3 "/home/kzfm1024/tmp/journal/data0/3.journal"
#define LF_JOURNAL_DATA1_4 "/home/kzfm1024/tmp/journal/data1/4.journal"
#define LF_JOURNAL_DATA1_5 "/home/kzfm1024/tmp/journal/data1/5.journal"
#define LF_JOURNAL_DATA1_6 "/home/kzfm1024/tmp/journal/data1/6.journal"
#define LF_JOURNAL_DATA1_7 "/home/kzfm1024/tmp/journal/data1/7.journal"


struct lfdata {
    std::atomic_flag state;
    int index0[4];
    int top0;
    int index1[4];
    int top1;
    int data0[4];
};

extern "C" {
    int lf_journal_init();
    int lf_journal_open();
    int lf_journal_increment();
    int lf_journal_print();
    int lf_journal_close();
};
