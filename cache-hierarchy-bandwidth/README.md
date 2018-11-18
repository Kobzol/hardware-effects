## Cache/memory bandwidth
CPUs contain a hierarchy of memories, ranging from small and fast (registers) to large and slow (RAM).
Each additional level of memory can hold more data, but has higher access latency.

This example demonstrates the bandwidths of accessing L1/L2/L3 cache and memory. When accessing
memory it is desirable to have good locality of reference (both spatial and temporal).
Spatial locality means that nearby memory locations are accessed in a succession (so that prefetching can help).
Temporal locality means that when a memory location is accessed, it will be accessed again in the near future.
This should help the memory latency because the data will already be located in the cache.

Usage:
```bash
$ cache-hierarchy-bandwidth <size>
```

This program will create an array of numbers of `size` bytes. It will then repeatedly write to all elements of
the array and report the achieved bandwidth (how many MiB/s were transferred). You should see drops in bandwidth
when the array size goes over the capacity of the individual caches (L1, L2, L3).

If you're on Linux, you can get the size of your caches by using getconf.
```
$ getconf -a | grep CACHE
LEVEL1_DCACHE_SIZE                    32768
LEVEL2_CACHE_SIZE                    262144
LEVEL3_CACHE_SIZE                   6291456
```

You can run the Python benchmark script to plot how the bandwidth depends on the array size.
```bash
$ python3 benchmark.py <path-to-executable>
```

You can find more about memory hierarchy here:
https://en.wikipedia.org/wiki/Memory_hierarchy
