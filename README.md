# dynamic-programming-job-optimization

### Background
Given a sequence of n weeks, a plan is specified by a choice of “low-stress”, “high-stress”, or “none” for each of the n weeks,
with the property that if “high-stress” is chosen for week i > 1, then “none” has to be chosen for week i − 1. (It is okay to
choose a high-stress job in week 1.) The value of the plan is determined in the natural way: for each i, you add li to the value if
you choose “low-stress” in week i, and you add hi to the value if you choose “high-stress” in week i. (You add 0 if you choose
“none” in week i.)
Given a set of values l1, l2, . . . , ln and h1, h2, . . . , hn, find a plan of maximum value. (Such a plan will be called optimal.)

### Input
*input.txt*
The input file contains multiple sets of inputs. The input file begins with a single positive integer on a line by itself
indicating the number of cases following, each of them as described below. This line is followed by a blank line, and there
is also a blank line between consecutive inputs.
The first line of each input contains an integer n, which represents the number of weeks for this instance. The next next
line contains n integers, separated by spaces. The i
th integer in this line represents the revenue for selecting a low-stress
job on day i (i.e., li). The next next line contains n integers, separated by spaces. The i
th integer in this line represents
the revenue for selecting a high-stress job on day i (i.e., hi).

### Output
*Enzinna.txt*
The output file should contain two lines for each input.
(a) The first output line should indicate the value of the plan
(b) The second output line should indicate the plan. This line should have n characters with one space between each
character. The i
th character represents the choice made for week i, where L indicates low-stress job, H indicates
high-stress job, and N indicates no job.
There should be a blank line between consecutive outputs.
Note that if more than one plan yields the the optimal solution then any one will do.

## Running
To compile the code simply type “make” and an executable name main will be created. It is possible to compile this code without the use of a makefile, but to avoid any confusion a makefile is provided.
 
`“g++ main.cpp -o main”`

## Date Completed
Spring 2014 - University of South Florida
