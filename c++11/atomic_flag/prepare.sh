#!/bin/bash

umount $HOME/tmp/journal
rm -rf $HOME/tmp/journal

mkdir -p $HOME/tmp/journal
mount -t tmpfs tmpfs $HOME/tmp/journal
mkdir -p $HOME/tmp/journal/data0
mkdir -p $HOME/tmp/journal/data1
chown -R kzfm1024.kzfm1024 $HOME/tmp/journal

./build/lf-journald
