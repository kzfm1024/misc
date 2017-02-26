#!/usr/bin/python
# -*- coding: utf-8 -*-

class textfile:
    ntfiles = 0 # テキストファイルの数
    def __init__(self,fname):
        textfile.ntfiles += 1
        self.name = fname # ファイル名
        self.fh = open(fname)
        self.nlines = 0 # 行数
        self.nwords = 0 # 語数
        self.npars = 0 # 段落数
        self.lines = self.fh.readlines()
        self.wordlineparcount()
    def wordlineparcount(self):
        "ファイルの中の行と語の数を数える"
        self.nlines = len(self.lines)
        inparagraph = 0
        for l in self.lines:
            w = l.split()
            self.nwords += len(w)
            if l == '\n':
                if inparagraph:
                    inparagraph = 0
            elif not inparagraph:
                self.npars += 1
                inparagraph = 1

    def grep(self,target):
        "targetを含むすべての行を表示する"
        for l in self.lines:
            if l.find(target) >= 0:
                print l
        print i

def main():
    t = textfile('test.txt')
    print t.nwords, t.nlines, t.npars

if __name__ == '__main__':
    main()
