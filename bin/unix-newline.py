#!/usr/bin/env python

import sys
from itertools import imap

def usage():
    sys.stderr.write("Usage: " + sys.argv[0] + " file\n")
    sys.exit(2)

def main():
    if len(sys.argv) != 2:
        usage()
    with open(sys.argv[1]) as f:
        txt = '\n'.join(imap(lambda line:line .rstrip('\r\n'), f))
    with open(sys.argv[1], 'wb') as f:
        f.write(txt)
    sys.exit(0)

if __name__ == '__main__':
    main()
