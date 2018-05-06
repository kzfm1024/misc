#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

def read_tokens(file):
    with open(file) as f:
        return [c for c in f.read() if c in {'>' , '<' , '+', '-', '.', ',', '[', ']'}]

def analyze_jumps(tokens):
    jumps = {}
    starts = []
    for i, c in enumerate(tokens):
        if c == '[':
            starts.append(i)
        elif c == ']':
            st = starts.pop()
            to = i
            jumps[st] = to
            jumps[to] = st
    return jumps

def run(tokens):
    jumps = analyze_jumps(tokens)
    tape = [0]
    pc = 0
    cur = 0

    while pc < len(tokens):
        c = tokens[pc]
        if c == '+':
            tape[cur] += 1
        elif c == '-':
            tape[cur] -= 1
        elif c == '>':
            cur += 1
            if cur >= len(tape):
                tape.append(0)
        elif c == '<':
            cur -= 1
        elif c == '.':
            c = tape[cur]
            sys.stdout.write(chr(c))
        elif c == ',':
            tape[cur] = int(sys.stdin.read(1))
            pass
        elif c == '[':
            if tape[cur] == 0:
                pc = jumps[pc]
        elif c == ']':
            if tape[cur] != 0:
                pc = jumps[pc]
        pc += 1

    sys.stdout.write('\n')

def run_brainf_ck(file):
    tokens = read_tokens(file)
    run(tokens)
    
if __name__ == '__main__':
    for file in sys.argv[1:]:
        run_brainf_ck(file)
