## Write combining
This example demonstrates write combining present in modern processors.
CPUs have write combining buffers that try to fill a whole cache line before sending it
for a write into cache/memory to reduce bus bandwidth stress/usage.

The number of WC buffers is of course limited, so you get their benefit only when writing to a small number of 
memory streams concurrently. For example if you have 4 WC buffers, you can write to 4 different arrays
repeatedly and the writes should be buffered by the WC buffers. If you write to 5 arrays at once,
one of the writes will not benefit from the write combining.

Usage:
```bash
$ write-combining <array-count> <write-increment>
```

The program will create `array-count` large arrays of 8-bytes integers and will repeatedly write to all of their elements.
`write-increment` specifies how many arrays will be written at once in a loop from zero to the array size.

Example:
```
# creates 6 arrays, first writes to all elements of array 0, then writes to all elements of array 1 etc.
write-combining 6 1

# creates 6 arrays, first writes to all elements of arrays 0,1,2, then writes to all elements of arrays 3,4,5
write-combining 6 3

# creates 6 array, writes to all elements of arrays 0,1,2,3,4,5
write-combining 6 6 
```

It is tricky to demonstrate this effect, but I think that this program's slowdown
can be explained by it. On my computer `count 6, increment 3` is consistently faster than `count 6, increment 6`
even though it does more work (increments the loop counter twice instead of once per each element) and
has more LLC and L1i cache misses (measured by `perf stat`, YMMV). The effect depends on the
number of write combine buffers in your CPU, common number is somewhere between 4 and 10.
I suggest trying combinations `6/3, 6/6, 8/4, 8/8` and so on.
However I suspect that this slowdown could also have something to do with the number of streams that the hardware
prefetcher can follow.


You can use the provided `benchmark.py` script to test various `array-count/write-increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

Write combining explanation:
https://mechanical-sympathy.blogspot.com/2011/07/write-combining.html
