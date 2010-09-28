#!/usr/bin/env python

import sys
from xml.etree import ElementTree

exit_code = 0

def usage():
    sys.stderr.write("Usage: " + sys.argv[0] + " file\n")
    sys.exit(2)

if len(sys.argv) != 2:
    usage()

rss = ElementTree.parse(sys.argv[1]).getroot()
sys.exit(exit_code)
