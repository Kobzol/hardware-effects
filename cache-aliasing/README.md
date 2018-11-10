## Cache aliasing
This example demonstrates aliasing in the CPU cache.
CPU caches are basically hardware hash tables, so they suffer from hash table conflicts.

If you access memory in specific address intervals, you can observe significant
slow down, because the cache lines will be mapped onto the same cache bucket.

Usage:
```bash
$ cache-aliasing <count> <increment>
```

The program will write to `count` 4-byte integers many times, each of which will be `increment` bytes apart.

You can use the provided `benchmark.py` script to test various `count/increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

If you have a recent Intel x86-64 CPU, chances are that you have a
8-way associative L1 cache with 64-byte cache lines and 32KiB total size.

In that case you should see a significant slowdown with increment 4096 (0x1000) when you go with `count` over 8.
Because the cache is 8-way associative, with 9 or more accesses the cache lines will cause thrashing.
In general any increment that will keep the first index bits of an address constant will cause degraded performance.

Cache explanation:
https://manybutfinite.com/post/intel-cpu-caches
