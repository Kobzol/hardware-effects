## Cache conflicts
This example demonstrates address conflicts that can occur in the CPU cache.
CPU caches are basically hardware hash tables, so they suffer from hash table conflicts.
They are usually implemented as N-way associative caches, which means
that they have place for N cache lines in each hash bucket.

If you access memory in specific address intervals and repeatedly put
more cache lines into the bucket than it can hold, you can observe significant slowdown since
the cache will thrash and evict a cache line on every access.

If you're on Linux, you can get the associativity of your cache (the `N`) by using getconf.
```
$ getconf -a | grep CACHE
LEVEL1_DCACHE_SIZE                 32768
LEVEL1_DCACHE_ASSOC                8
LEVEL1_DCACHE_LINESIZE             64
```

If you have a recent Intel x86-64 CPU, chances are that you have an
8-way associative L1 cache with 64-byte cache lines and 32KiB total size.

In that case you should see a significant slowdown with increment 4096 (0x1000) when you start putting more than
8 cache lines into a cache bucket. Because the cache is 8-way associative, 9 or more lines put into the bucket
will cause the cache to thrash.

4096 is a bad increment for this type of cache, because it keeps the first 12 bits of the address constant and
therefore it maps into the same cache bucket.
In general any increment that will keep the first bits of an address constant should cause degraded performance.
You can see a graphical explanation of cache associativity here: http://csillustrated.berkeley.edu/PDFs/handouts/cache-3-associativity-handout.pdf.


Usage:
```bash
$ cache-conflicts <count> <increment>
```

The program will write many times to `count` 4-byte integers, each of which will be `increment` bytes apart.
On my Intel CPU I can observe a significant slowdown when going with `count` over 8 and using `increment` 4096.

You can observe this effect by checking the `l1d.replacement` performance counter:

```bash
$  perf stat -e l1d.replacement cache-conflicts 8 4096
80038
        54 327    l1d.replacement

$  perf stat -e l1d.replacement cache-conflicts 9 4096
186503
    18 505 589    l1d.replacement
```

You can use the provided `benchmark.py` script to test various `count/increment` combinations
and plot their relative speeds.

```bash
$ python3 benchmark.py <path-to-executable>
```

Cache explanation:
https://manybutfinite.com/post/intel-cpu-caches
