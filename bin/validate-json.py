#!/usr/bin/env python

import sys
import json

def usage():
    sys.stderr.write("Usage: " + sys.argv[0] + " file\n")
    sys.exit(2)

def main():
    if len(sys.argv) != 2:
        usage()

    f = open(sys.argv[1], 'r')
    j = json.load(f)
    f.close()
    sys.exit(0)

if __name__ == '__main__':
    main()
