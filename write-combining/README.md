## Write combining
This example demonstrates write combining. CPUs have write combining buffers that try to fill a whole
cache line before sending it for a write into memory to reduce bus bandwidth stress/usage.
This only happens for special types of writes (for example writes to videocard memory or non-temporal stores
that do not go through the cache and write directly to memory).

The number of WC buffers is limited, so you get their benefit only when writing to a small number of 
memory streams concurrently. For example if you have 4 WC buffers, you can write to 4 different arrays
repeatedly and the writes should be buffered by the WC buffers. If you write to 5 arrays at once,
one of the writes will not benefit from the write combining.

Usage:
```bash
$ write-combining <array-count> <write-increment>
```

The program will create `array-count` large arrays of 8-bytes integers and will repeatedly write to all of their elements
using non-temporal stores. `write-increment` specifies how many arrays will be written at once in a loop from zero to the array size.

Example:
```
# creates 6 arrays, first writes to all elements of array 0, then writes to all elements of array 1 etc.
write-combining 6 1

# creates 6 arrays, first writes to all elements of arrays 0,1,2, then writes to all elements of arrays 3,4,5
write-combining 6 3

# creates 6 array, writes to all elements of arrays 0,1,2,3,4,5
write-combining 6 6 
```

Modern Intel CPUs have around 10 WC buffers. When you allocate 16 arrays and use increments 1-16, you should see
a large performance drop at some point. The first few increments should be slower, since they are executing more loops
and thus they will are doing more work (iterating through a loop introduces some overhead, namely incrementing the
index variable, branching and jumping).


You can use the provided `benchmark.py` script to test various `array-count/write-increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

Write combining explanation:
https://mechanical-sympathy.blogspot.com/2011/07/write-combining.html
