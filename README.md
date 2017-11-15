# Sorting by Reversals for Reversal Distance Problem on Bioinformatics
Implement the algorithms SimpleReversalSort, BreakpointReversalSort and ImprovedBreakpointReversalSort .

## Data
/k-input.data

## Structure
<pre>
	/srs.cpp 	- SimpleReversalSort
	/srs-benchmark.cpp - SimpleReversalSort Benchmark
	/brs.cpp - BreakpointReversalSort
	/ibrs.cpp - ImprovedBreakpointReversalSort
</pre>

## Compilation (x64 Native Tools Command Prompt for VS 2017 or g++)
cl srs.cpp
g++ srs.cpp -O3 -o srs

## Execution
srs.exe < input.data
srs-benchmark.exe 100000

![](img/srs.png)
![](img/srs-benchmark.png)


## Downloads
- All: https://github.com/nextco/sorting-by-reversals/archive/master.zip
