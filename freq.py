#!/bin/env python
import sys

def usage():
    print "Usage: %s MIN MAX NBITS" % (sys.argv[0])
    print "Where:"
    print "\tMIN is the minimum period."
    print "\tMAX is the maximum period."
    print "\tNBITS is the number of bits per symbol."
    sys.exit()

# Generates the frequency lookup table
if len(sys.argv) < 4:
    usage()

MIN = int(sys.argv[1])
MAX = int(sys.argv[2])
NBITS = int(sys.argv[3])

print "#include <stdint.h>"
print """
/* MIN = %i
   MAX = %i
   NBITS = %i */
""" % (MIN,MAX,NBITS)
print "const uint16_t period_lut[] = \n{"

NFREQ = (1 << NBITS) + 2
F_STEP = (MAX - MIN)/(NFREQ - 1)

for x in xrange(0, NFREQ):
    freq = MIN + F_STEP * x

    print "\t%i" % freq ,
    if x < (1 + (1<<NBITS)):
        print ","
    else:
        print ""

print "};"
