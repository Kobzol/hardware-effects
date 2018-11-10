## Write combining
This example demonstrates write combining present in modern processors.
CPUs have write combining buffers that try to fill a whole cache line before sending it for a write into cache/memory
to reduce bus bandwidth/usage.

The number of WC buffers is of course limited, so you get their benefit only when writing to a small number of 
memory streams concurrently.

This effect is a bit hard to observe directly, but I'm confident that this program can demonstrate it,
because for example `count 6, increment 3` is consistently faster than `count 6, increment 6` on my CPU
even though it has more LLC and L1i cache misses (measured by `perf stat`, YMMV). The effect depends on the
number of write combine buffers in your CPU, common number is somewhere between 4 and 10.
I suggest trying combinations `6/3, 6/6, 8/4, 8/8` and so on.

Usage:
```bash
$ write-combining <array-count> <write-increment>
```

The program will create `array-count` large arrays of 8-bytes integers and will repeatedly write to all of their elements.
`write-increment` specifies how many arrays will be written at once in a loop from zero to the array size.

Example:
```
# creates 6 arrays, first writes to all elements of arrays 0,1,2 and then writes to all elements of arrays 3,4,5
write-combining 6 3

# creates 6 array, writes to all elements of arrays 0,1,2,3,4,5
write-combining 6 6 
```

You can use the provided `benchmark.py` script to test various `array-count/write-increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

Write combining explanation:
https://mechanical-sympathy.blogspot.com/2011/07/write-combining.html
