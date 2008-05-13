#!/usr/bin/env python
import sys

if len(sys.argv) < 2:
    print "Use me right!"
    sys.exit()

NBITS = int(sys.argv[1])

ps = 8/NBITS

if (8 % NBITS) > 0:
    ps = ps + 1

print ps

