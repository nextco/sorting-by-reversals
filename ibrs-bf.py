#!/usr/bin/env python
# Creditos: https://www.cs.helsinki.fi/u/tpkarkka/opetus/14s/AfB/exercise2_problem3.py
# Improved breakpoint reversal sort - Bruteforce all vs all posibles combinations
# Example:  ./prob3.py 8 2 7 6 5 1 4 3
import sys

if len(sys.argv) <= 2:
    sys.exit("usage: " + sys.argv[0] + " [number 1] [number 2] ...")


# Returns the number of breakpoins.
# Equal to b() in the course book.
# Requires linear time.
def b(u):
    assert u[0] == 0 and u[len(u)-1] == len(u) - 1
    sum = 0
    for i in range(len(u) - 1):
        if u[i] != u[i+1] + 1 and u[i] != u[i+1] - 1:
            # print "Breakpoint:", u[i], u[i+1] 
            sum += 1
    return sum

# Reverses a range of given array in place.
# Simulates the p(i,j) function from course book.
# Requires linear time.
def rev_range(u, i, j):
    assert i <= j and i >= 0 and j >= 0
    assert i < len(u) and j < len(u)
    n = j - i + 1
    for k in range(n/2): 
        # Swap values of k and (n-k-1) in range [i,j]
        u[i+k], u[i+(n-k-1)] = u[i+(n-k-1)], u[i+k]
    return u

# Returns true if given array has a decreasing strip.
# Strips of length one are decreasing by definition (see course book).
# Requires linear time.
def has_decreasing(u):
    assert u[0] == 0 and u[len(u)-1] == len(u) - 1
    for i in range(1, len(u)-1):
        # Element i belongs to a decreasing strip 
        # if the element does not belong to an increasing strip.
        if u[i-1] + 1 != u[i] and u[i] + 1 != u[i+1]:
            # print "posible strip decreciente en: ", u[i-1], " ", u[i], " ", u[i + 1]
            return True # u[i] is in decreasing stip
    return False

# Returns the segment whose reversal minimizes 
# the number of breakpoints.
# Brute-force search over all reversals.
# Since b() and rev_range() require linear time,
# this function requires in total O(n^2) time.
def choose_reversal(u):
    assert u[0] == 0 and u[len(u)-1] == len(u) - 1
    max = 0
    max_seg = (0,0)
    br = b(u) # Original number of breakpoints
    for i in range(1, len(u)-2):
        for j in range(i+1, len(u)-1):
            # Evaluate the reversal p(i,j):
            # First check that at least one breakpoint is removed.
            # This check is not necessary but improves time complexity
            # because there are at most 2b reversals that pass the test.
            # This constant time check is performed O(n^2) times.
            if abs(u[i]-u[j+1]) == 1 or abs(u[i-1]-u[j]) == 1:
                # Count how many breakpoints are removed by reversal p(i,j)
                # Passing a shallow copy so that array u does not get modified.
                # This takes O(n) time but is executed at most 2b times.
                a = rev_range(u[:], i, j)            
                if br - b(a) > max:
                    max = br - b(a)
                    max_seg = (i,j)
                    if max == 2:   # Best we can find
                        return (i,j)
    return max_seg

# Returns a segment containing an increasing strip.
# Chooses always the leftmost increasing strip
# (excluding the special elements u[0] and u[len(u)-1]).
# Requires linear time.
def choose_increasing(u):
    assert u[0] == 0 and u[len(u)-1] == len(u) - 1
    i = 1
    while i < len(u)-1 and u[i] + 1 != u[i+1]:
        i += 1
    assert i < len(u)-1
    j = i + 1
    while j < len(u)-2 and u[j] + 1 == u[j+1]:
        j += 1
    return (i, j)

# Implements the improved breakpoint reversal sort.
# The while-loop iterates O(b) times where b is the number of
# breakpoints. The function choose_reversal() requires
# O(n^2) time, thus, the total time complexity
# is O(b \times n^2) where n is the number of elements.
def improvedBreakpointReversalSort(u):
    while b(u) > 0:
        if has_decreasing(u):
            (i, j) = choose_reversal(u)
            rev_range(u, i, j)
        else:
            (i, j) = choose_increasing(u)
            rev_range(u, i, j)
        print u, "b(u) =", b(u)
    return u

# Convert to integers (because sys.argv contains strings)
ints = [int(i) for i in sys.argv[1:]]

# print "ints =", ints

# Sanity check:
for i in ints:
    assert i > 0 and i <= len(ints)

# Add begin and end markers 0 and n+1
u = [0] + ints + [len(ints) + 1]
print u, "b(u) =", b(u)
improvedBreakpointReversalSort(u)
