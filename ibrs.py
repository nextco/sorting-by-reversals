#!/usr/bin/env python
# Improved breakpoint reversal sort
# Example:  python ibrs.py 1 2 3 4 5 6 9 7 8
# Credits: http://www.csbio.unc.edu/mcmillan/Media/breakpointReversalSort.txt
# import random
import sys

def hasBreakpoints(seq):
    """ returns True if sequnces in not strictly increasing by 1 """
    for i in xrange(1, len(seq)):
        if (seq[i] != seq[i-1] + 1):
            return True
    return False

def getStrips(seq):
    """ find contained intervals where sequence is ordered, and return intervals
    in as lists, increasing and decreasing. Single elements are considered
    decreasing. "Contained" excludes the first and last interval. """
    deltas = [seq[i+1] - seq[i] for i in xrange(len(seq)-1)]
    # print "deltas -> ", deltas
    increasing = list()
    decreasing = list()
    start = 0

    # print "enumerate(deltas) -> ", enumerate(deltas)
    for i, diff in enumerate(deltas):
        if (abs(diff) == 1) and (diff == deltas[start]):
            continue
        if (start > 0):
            if deltas[start] == 1:
                increasing.append((start, i+1))
            else:
                decreasing.append((start, i+1))
        start = i+1
    return increasing, decreasing

def pickReversal(seq, strips):
    """ test each decreasing interval to see if it leads to a reversal that
    removes two breakpoints, otherwise, return a reversal that removes only one """
    reversal = (-1, None)

    left = [i for i, j in strips]    
    right = [j for i, j in strips]
    
    print "strips -> ", strips
    print "left -> ", left
    print "right -> ", right


    for i in left:
        for j in right:
            print "(i, j) = ", i , " ",  j
            if (i >= j-1):
                # skip invalid intervals and
                # those with only one element
                continue
            breakpointsRemoved = 0
            if (abs(seq[j-1] - seq[i-1]) == 1):
                # reversal will remove left breakpoint
                breakpointsRemoved += 1
            if (abs(seq[j] - seq[i]) == 1):
                # reversal will remove right breakpoint
                breakpointsRemoved += 1
            if (breakpointsRemoved > reversal[0]):
                reversal = (breakpointsRemoved, (i,j))
    print "%d:" % reversal[0],
    return reversal[1]

def doReversal(seq,(i,j)):
    return seq[:i] + [element for element in reversed(seq[i:j])] + seq[j:]

def improvedBreakpointReversalSort(seq):
    while hasBreakpoints(seq):
        increasing, decreasing = getStrips(seq)

        # print "increasing ", increasing
        # print "decreasing ", decreasing

        if len(decreasing) > 0:
            reversal = pickReversal(seq, increasing + decreasing)
        else:
            print "0:",
            reversal = increasing[0]
        print seq, "reversal", reversal
        seq = doReversal(seq,reversal)
    print "s:", seq, "Sorted"
    return

if len(sys.argv) <= 2:
    sys.exit("usage: " + sys.argv[0] + " [number 1] [number 2] ...")

# Convert to integers (because sys.argv contains strings)
ints = [int(i) for i in sys.argv[1:]]

# print "ints =", ints

# Sanity check:
for i in ints:
    assert i > 0 and i <= len(ints)

# Add begin and end markers 0 and n+1
u = [0] + ints + [len(ints) + 1]
print "o:", u
improvedBreakpointReversalSort(u)
