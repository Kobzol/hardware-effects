## Branch misprediction
This example demonstrates mispredictions in the CPU branch-predictor.
Modern CPUs have predictors that try to guess whether a branch will be taken or not.
This information is vital for performance because the CPU instruction pipeline may be flushed if a wrong guess was made.

Usage:
```bash
$ branch-misprediction <sort>
$ branch-target-misprediction <sort>
```

The `branch-misprediction` program will fill a large array with uniformly distributed random integers from 1 to 100.
Then it will repeatedly go through the whole array and sum elements that are smaller than 50. If `sort` equals 1,
the array will be sorted in ascending order at the beginning of the program. If `sort` equals 0 the array will not be sorted.

If the array is sorted, the branch predictor will learn very quickly that it should expect the `array[i] < 50`
condition to be true in the first half of the array and false in the second half. I disabled inlining of the
condition body so that the compiler can't do tricks like using CMOV so a misprediction costs a lot.
If you don't observe a large performance difference between the sort/nosort scenarios, make sure that your
compiler is not inlining the `handle_sum` function call.

The branch miss count can be easily observed with perf. Here's how it looks on my computer:
```bash
$  perf stat -e branch-misses branch-misprediction 0
2304
209 499 279      branch-misses

$  perf stat -e branch-misses branch-misprediction 1
460
70 238 390      branch-misses
```
The program has more than two times more branch mispredictions without the sort and runs almost six times slower.
It's actually much faster even when counting the time to sort.

The `branch-target-misprediction` demonstrates the branch target predictor, which guesses where an indirect
jump will go. It creates a large array of objects that overload a virtual method. Then it repeatedly goes over
the array and calls the virtual method on all objects. The effect should be
similar to the `branch-misprediction`, so if the array is not sorted, the performance should degrade.

You can use the provided `benchmark.py` script to plot the relative speeds of the sort/nosort variant.

```bash
$ python3 benchmark.py <path-to-executable>
```

Branch prediction explanation:
https://en.wikipedia.org/wiki/Branch_predictor
